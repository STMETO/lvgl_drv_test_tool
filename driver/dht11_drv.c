#include<linux/delay.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>

static int major=0;	//主设备号
static struct class *dht11_class;	//类
static struct gpio_desc *dht11_data;

static void dht11_start(void)
{
	gpiod_direction_output(dht11_data, 1);
	mdelay(30);
	gpiod_set_value(dht11_data, 0);
	mdelay(25); //大于18ms
	gpiod_set_value(dht11_data, 1);
	udelay(35); //20-40us
	gpiod_direction_input(dht11_data);	//准备读取数据
}

static int dht11_wait_ready(void)
{
	int timeout_us = 20000;

	//现在是高电平
	//等待低电平
	while(gpiod_get_value(dht11_data) && --timeout_us)
	{
		udelay(1);
	}
	if(!timeout_us)	//如果因为超时退出
	{
		return -1;
	}

	//现在是低电平
	//等待高电平
	timeout_us = 200;
	while(!gpiod_get_value(dht11_data) && --timeout_us)
	{
		udelay(1);
	}
	if(!timeout_us)	//如果因为超时退出
	{
		return -1;
	}

	//现在是高电平
	//等待低电平
	timeout_us = 200;
	while(gpiod_get_value(dht11_data) && --timeout_us)
	{
		udelay(1);
	}
	if(!timeout_us)	//如果因为超时退出
	{
		return -1;
	}

	//正常返回0
	return 0;
	
}

static int dht11_read_bye(unsigned char *dht11_buf)
{
	int i;
	int timeout_us = 200;
	unsigned char data=0;

	for(i=0; i<8; i++)
	{
		//现在是低电平
		//等待高电平
		timeout_us = 400;
		while(!gpiod_get_value(dht11_data) && --timeout_us)
		{
			udelay(1);
		}
		if(!timeout_us)	//如果因为超时退出
		{
			return -1;
		}

		//一般认为超过35*1.6us的时间算数据“1”，（udelay延时函数不准，为1.6us）
		udelay(35);

		if(gpiod_get_value(dht11_data))
		{
			data = (data << 1) | 1;

			//一定要等这个高电平结束
			//现在是高电平
			//等待低电平
			timeout_us = 400;
			while(gpiod_get_value(dht11_data) && --timeout_us)
			{
				udelay(1);
			}
			if(!timeout_us)	//如果因为超时退出
			{
				return -1;
			}
		}
		else
		{
			data = (data << 1) | 0;
		}
	}
	(*dht11_buf) = data;
	return 0;
}

//用户调用 read() 系统调用时进入此函数,实现了从 DHT11 传感器读取温湿度数据并写回用户空间
/*
	local_irq_save()：关中断，防止DHT11时序被打断
	dht11_start()：拉低 GPIO 发起DHT11起始信号（拉低25ms）
	dht11_wait_ready()：等待DHT11应答（电平翻转时序）
	dht11_read_bye()：逐字节读取 5 字节数据（湿度/温度整数+小数+校验和）
	校验和判断：如果 dht11_data[4] ≠ 其他四项之和，判定错误
	copy_to_user()：将前4字节传给用户空间
*/
static ssize_t dht11_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	unsigned long flags;
	int err;
	unsigned char dht11_data_buf[5];
	int i;

	//关中断
	local_irq_save(flags);
	
	//发出起始信号
	dht11_start();

	//等待dht11回复
	if(dht11_wait_ready() == -1)	//异常时返回-1
	{
		local_irq_restore(flags);
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	//开始接收数据
	for(i=0; i<5; i++)
	{
		if(dht11_read_bye(&dht11_data_buf[i]) == -1)
		{
			local_irq_restore(flags);
			printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}
	}

	//开中断
	local_irq_restore(flags);
	
	if(dht11_data_buf[4] != (dht11_data_buf[0]+dht11_data_buf[1]+dht11_data_buf[2]+dht11_data_buf[3]) )
	{
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	err = copy_to_user(buf, dht11_data_buf, 4);
	return 4;
}
 
static unsigned int dht11_poll(struct file *file, struct poll_table_struct *poll)
{
	return 0;
}

//当用户 open("/dev/xf_dht11") 时调用
static int dht11_open(struct inode *inode, struct file *file)
{
	return 0;
}


static struct file_operations dht11_ops = {
	.owner	=THIS_MODULE,
	.open 	=dht11_open,
	.read 	=dht11_read,
	.poll 	=dht11_poll,
};


//多个设备资源共用的驱动
static const struct of_device_id dht11s[] = {
	{.compatible = "xf,dht11_t"},
	{ },
};


//匹配回调函数
//匹配设备树:获取GPIO资源，创建设备节点
static int dht11_probe(struct platform_device *pdev)
{
	//设备树中有定义
	dht11_data = gpiod_get(&pdev->dev, NULL, GPIOD_OUT_HIGH);	//从设备树获取 GPIO
	if(IS_ERR(dht11_data)){
		dev_err(&pdev->dev, "GPIO data ERR!");
		return PTR_ERR(dht11_data);
	}

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	//device_create(dht11_class, NULL, MKDEV(major, 0), NULL, "mydht11"); 

	device_create(dht11_class, NULL, MKDEV(major, 0), NULL, "xf_dht11"); //创建 /dev/xf_dht11 设备节点

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	return 0;
}

static int dht11_remove(struct platform_device *pdev)
{
	//删除设备信息
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(dht11_class, MKDEV(major, 0));
	gpiod_put(dht11_data);
	return 0;
}


//驱动程序注册结构体
static struct platform_driver dht11_driver = {
	.probe		=dht11_probe,
	.remove		=dht11_remove,
	.driver		={
		.name	="dht11_xf",
		.of_match_table = dht11s,
	},
};

//模块加载:注册字符设备 + 创建类 + 注册platform_driver
static int __init dht11_init(void)
{
	int err;

	//打印运行到这里的代码段行数，用于调试
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	//注册设备
	major = register_chrdev(0, "xf_dht11", &dht11_ops);//0表示自动分配主设备号,"xf_dht11":设备名，将出现在/proc/devices中
														//&dht11_ops：字符设备操作函数集,file_operations结构体

	//创建类，自动分配设备结点信息
	dht11_class = class_create(THIS_MODULE, "xf_dht11_class");//创建一个设备类（class），用于自动生成 /sys/class/xf_dht11_class/ 路径
																//后续 device_create() 会借助它生成 /dev/xf_dht11 节点
	if (IS_ERR(dht11_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "xf_dht11");
		return PTR_ERR(dht11_class);
	}

	//注册驱动结构体
	err = platform_driver_register(&dht11_driver);	//注册一个平台总线驱动 dht11_driver,它会自动匹配设备树中：
													//compatible = "xf,dht11_t";匹配成功后，会调用 dht11_probe()。

	return err;
}

static void __exit dht11_exit(void)	//__exit 是内核的一个宏，告诉内核这个函数只在模块卸载时才使用，不会用于内核启动。
{
	//打印运行到这里的代码段行数，用于调试
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	//卸载驱动结构体
	platform_driver_unregister(&dht11_driver);	
	class_destroy(dht11_class);					
	unregister_chrdev(major, "xf_dht11");	//major 是你在 register_chrdev() 注册时得到的主设备号。
}


module_init(dht11_init);	
module_exit(dht11_exit);	

MODULE_LICENSE("GPL");		


