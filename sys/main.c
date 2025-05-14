#include <stdio.h>
#include <ui.h>
#include <page_manager.h>

vim /home/fxy/imx6ull/100ask_imx6ull-sdk/Linux-4.9.88/arch/arm/boot/dts/100ask_imx6ull-14x14.dts

# 编译设备树
make dtbs -j8 -C /home/fxy/imx6ull/100ask_imx6ull-sdk/Linux-4.9.88/

# 设备树文件决定路径
cp /home/fxy/imx6ull/100ask_imx6ull-sdk/Linux-4.9.88/arch/arm/boot/dts/100ask_imx6ull-14x14.dtb /home/book/nfs_rootfs/


int main()
{
    // extern void test(void);
    // test();

    PageSystemInit();   //UI_Init()已经在内部实现

    Page("main")->Run(NULL);    //Page函数会返回名字与"main"相同的页面的地址，
                                //其实也就是main页面的结构体变量，再执行Run方法，让main页面运行起来

    //不断刷新图形界面、响应输入事件、执行动画等
    while(1) {     
        lv_timer_handler();  // LVGL的核心函数，处理所有图形事件
        usleep(5000);
    }

    return 0;
}