#include <page_manager.h>
#include <stdio.h>
#include <ui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

static lv_obj_t * main_page = NULL;

LV_FONT_DECLARE(Icon_album);
LV_FONT_DECLARE(Icon_Camera);
#define  FONTAWESOME_SYMBOL_album     "\xef\x80\xbe"
#define  FONTAWESOME_SYMBOL_camera	  "\xef\x80\xb0" 

//创建主界面
static void MainPageCreate(void)
{
	main_page = lv_obj_create(NULL);    // LVGL 中创建一个新的顶层页面对象
}

//相册回调
static void my_Album_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("album")->Run(NULL);
		LV_LOG_USER("Album btn pressed\n");
	}
}

//摄像头回调
static void my_Camera_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("Camera")->Run(NULL);
		LV_LOG_USER("Camera btn pressed\n");
	}
}

//超声波回调
static void my_Sr04_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("Sr04")->Run(NULL);
	}
}

//at24c02回调
static void my_At24c02_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("At24c02")->Run(NULL);
	}
}

//DHT11回调
static void my_DHT11_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("DHT11")->Run(NULL);
		// LV_LOG_USER("hello");
	}
}

//sg90回调
static void my_Sg90_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);    // 获取当前部件(对象)触发的事件代码

	if(code_e == LV_EVENT_CLICKED)
	{
		Page("sg90")->Run(NULL);
	}
}

//放设计主页面要放置的控件
static void MainPageInit(void *User_data)
{
	//放置背景图在主界面
	LV_IMG_DECLARE(background_01);
	lv_obj_t * img = lv_img_create(main_page);
	lv_img_set_src(img, &background_01);

	//创建一个输入组 g，然后把当前鼠标/触摸输入设备加进去。
	lv_group_t * g = lv_group_create();
	lv_group_set_default(g);			//添加到默认组
	lv_indev_set_group(mouse_indev, g);	//输入设备添加进组

	//创建按钮样式
	static lv_style_t style;         	//创建样式
	lv_style_init(&style);           	//初始化样式
	lv_style_set_radius(&style,8);   	//设置样式的圆角
	lv_style_set_width(&style,80);  	//设置样式的宽度
	lv_style_set_height(&style, 80); 	//设置样式的高度
	lv_style_set_pad_all(&style, 3);
    // lv_style_set_align(&style, LV_ALIGN_CENTER);
	lv_style_set_bg_color(&style, lv_color_hex(0x009688));
	lv_style_set_text_color(&style, lv_color_hex(0xffffff));

	//设置freetype字体大小24
	FT_font_Set(24);
////////////////////////////////////////////////////////////////////////////////////
	//相册
	//创建图标样式
	static lv_style_t style_image;         							//创建样式
	lv_style_init(&style_image);           							//初始化图标样式
	lv_style_set_text_font(&style_image, &Icon_album);				//初始化图标样式字体
	lv_style_set_text_color(&style_image, lv_color_hex(0xffffff));	//初始化图标样式颜色，白色

	lv_obj_t * Btn_album = lv_btn_create(main_page);		//创建相册按钮
	lv_obj_align(Btn_album, LV_ALIGN_TOP_LEFT, 100, 120);	//位置
	lv_obj_add_style(Btn_album,&style,LV_STATE_DEFAULT);	//样式

	lv_obj_t * Label_album = lv_label_create(main_page);			// 使用新添加的图标（symbol）
    lv_label_set_text(Label_album, FONTAWESOME_SYMBOL_album);			//添加图标
	lv_obj_add_style(Label_album,&style_image,LV_STATE_DEFAULT);		//添加样式，白色图标以及字体设置
	lv_obj_align_to(Label_album, Btn_album, LV_ALIGN_CENTER, 0, 0);			//居中
	
	lv_obj_t * Label_album_str = lv_label_create(main_page);				//添加APP名称
	lv_obj_set_style_text_color(Label_album_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
	lv_label_set_recolor(Label_album_str, 1);
	lv_label_set_text_fmt(Label_album_str, "#ffffff 相册#");							//添加名字
	lv_obj_set_style_text_font(Label_album_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_album_str, Btn_album, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
////////////////////////////////////////////////////////////////////////////////////////////////////
	//摄像头
	static lv_style_t style_image_camera;         							//创建样式
	lv_style_init(&style_image_camera);           							//初始化图标样式
	lv_style_set_text_font(&style_image_camera, &Icon_Camera);				//初始化图标样式字体
	lv_style_set_text_color(&style_image_camera, lv_color_hex(0xffffff));	//初始化图标样式颜色，白色

	lv_obj_t * Btn_Camera = lv_btn_create(main_page);
	lv_obj_add_style(Btn_Camera,&style,LV_STATE_DEFAULT);
	lv_obj_align_to(Btn_Camera, Btn_album, LV_ALIGN_CENTER, 180, 0);

	lv_obj_t * Label_Camera = lv_label_create(main_page);
    lv_label_set_text(Label_Camera, FONTAWESOME_SYMBOL_camera);
	lv_obj_add_style(Label_Camera,&style_image_camera,LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Camera, Btn_Camera, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * Label_Camera_str = lv_label_create(main_page);				//添加APP名称
	lv_label_set_recolor(Label_Camera_str, 1);
	lv_label_set_text(Label_Camera_str, "#ffffff 摄像头#");						//添加名字
	lv_obj_set_style_text_font(Label_Camera_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Camera_str, Btn_Camera, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
////////////////////////////////////////////////////////////////////////////////////////////////////
	//超声波
	lv_obj_t * Btn_Sr04 = lv_btn_create(main_page);
	lv_obj_add_style(Btn_Sr04,&style,LV_STATE_DEFAULT);
	lv_obj_align_to(Btn_Sr04, Btn_Camera, LV_ALIGN_CENTER, 180, 0);

	lv_obj_t * Label_Sr04 = lv_label_create(main_page);
    lv_label_set_text(Label_Sr04, FONTAWESOME_SYMBOL_album);
	lv_obj_add_style(Label_Sr04,&style_image,LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Sr04, Btn_Sr04, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * Label_Sr04_str = lv_label_create(main_page);				//添加APP名称 
	lv_label_set_recolor(Label_Sr04_str, 1);
	lv_label_set_text(Label_Sr04_str, "#ffffff 超声波#");						//添加名字
	lv_obj_set_style_text_font(Label_Sr04_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Sr04_str, Btn_Sr04, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
////////////////////////////////////////////////////////////////////////////////////////////////////
	//AT24C02
	lv_obj_t * Btn_At24 = lv_btn_create(main_page);
	lv_obj_add_style(Btn_At24,&style,LV_STATE_DEFAULT);
	lv_obj_align_to(Btn_At24, Btn_Sr04, LV_ALIGN_CENTER, 180, 0);
	
	lv_obj_t * Label_At24 = lv_label_create(main_page);
    lv_label_set_text(Label_At24, FONTAWESOME_SYMBOL_album);
	lv_obj_add_style(Label_At24,&style_image,LV_STATE_DEFAULT);
	lv_obj_align_to(Label_At24, Btn_At24, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * Label_AT24_str = lv_label_create(main_page);				//添加APP名称
	lv_label_set_recolor(Label_AT24_str, 1);
	lv_label_set_text(Label_AT24_str, "#ffffff AT24C02#");						//添加名字
	lv_obj_set_style_text_font(Label_AT24_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_AT24_str, Btn_At24, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
///////////////////////////////////////////////////////////////////////////////////////////////////
	//DHT11
	lv_obj_t * Btn_DHT11 = lv_btn_create(main_page);
	lv_obj_add_style(Btn_DHT11,&style,LV_STATE_DEFAULT);
	lv_obj_align_to(Btn_DHT11, Btn_At24, LV_ALIGN_CENTER, 180, 0);

	lv_obj_t * Label_DHT11 = lv_label_create(main_page);
    lv_label_set_text(Label_DHT11, FONTAWESOME_SYMBOL_album);
	lv_obj_add_style(Label_DHT11,&style_image,LV_STATE_DEFAULT);
	lv_obj_align_to(Label_DHT11, Btn_DHT11, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * Label_DHT11_str = lv_label_create(main_page);				//添加APP名称
	lv_label_set_recolor(Label_DHT11_str, 1);
	lv_label_set_text(Label_DHT11_str, "#ffffff DHT11温湿度#");						//添加名字
	lv_obj_set_style_text_font(Label_DHT11_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_DHT11_str, Btn_DHT11, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
///////////////////////////////////////////////////////////////////////////////////////////////////
	//sg90
	lv_obj_t * Btn_Sg90 = lv_btn_create(main_page);
	lv_obj_add_style(Btn_Sg90,&style,LV_STATE_DEFAULT);
	lv_obj_align_to(Btn_Sg90, Btn_DHT11, LV_ALIGN_CENTER, 0, 200);

	lv_obj_t * Label_Sg90 = lv_label_create(main_page);
	lv_label_set_text(Label_Sg90, FONTAWESOME_SYMBOL_album);
	lv_obj_add_style(Label_Sg90,&style_image,LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Sg90, Btn_Sg90, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * Label_Sg90_str = lv_label_create(main_page);				//添加APP名称
	lv_label_set_recolor(Label_Sg90_str, 1);
	lv_label_set_text(Label_Sg90_str, "#ffffff SG90舵机#");						//添加名字
	lv_obj_set_style_text_font(Label_Sg90_str, ft_info.font, LV_STATE_DEFAULT);
	lv_obj_align_to(Label_Sg90_str, Btn_Sg90, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);	//移动位置到图标下方
///////////////////////////////////////////////////////////////////////////////////////////////////
	//回调函数
	lv_obj_add_event_cb(Btn_album, my_Album_event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Btn_Camera, my_Camera_event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Btn_Sr04, my_Sr04_event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Btn_At24, my_At24c02_event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Btn_DHT11, my_DHT11_event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Btn_Sg90, my_Sg90_event_cb, LV_EVENT_CLICKED, NULL);

}

//main_page运行时的函数
static void MainPageRun(void *pParams)
{
    // 第一步 清除上一个界面的所有内容
	lv_obj_clean(lv_scr_act());
    // 第二步 创建主界面下的各种控件
	MainPageInit(NULL);
    // 第三步 设置主界面为要显示的活动屏幕
	lv_scr_load(main_page);
}

//这里就是对于main_page的PageAction结构体变量的具体实现
static PageAction Main_Page = {
	.name = "main",
	.Create = MainPageCreate,
	.Run  = MainPageRun,
};

void Main_Page_Registered(void)
{
    //将main_page注册进页面链表中
	Registered_Page(&Main_Page);
}


