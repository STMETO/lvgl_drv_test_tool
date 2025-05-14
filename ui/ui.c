#include <ui.h>

#define DISP_BUF_SIZE (128 * 1024)  //显示缓冲区大小
#define FREETYPE_FONT_FILE ("/etc/simhei.ttf")  //字体文件在开发板上的路径

lv_indev_t *mouse_indev;  //鼠标输入设备对象（后面用来接触屏/鼠标的点击操作）  
lv_ft_info_t ft_info;   //FreeType字体对象信息，用来描述你加载了哪一个字体文件，字体大小是多少。

int UI_Init(void)
{
    /*LittlevGL init*/
    lv_init(); 

    /*Linux frame buffer device init*/
    fbdev_init();   

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];  

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 1024;
    disp_drv.ver_res    = 600;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    mouse_indev = lv_indev_drv_register(&indev_drv_1);

    lv_freetype_init(8,64,64);
    /*FreeType uses C standard file system, so no driver letter is required.*/
    ft_info.name = FREETYPE_FONT_FILE;
    ft_info.weight = 24;
    ft_info.style = FT_FONT_STYLE_NORMAL;
    ft_info.mem = NULL;
    if(!lv_ft_font_init(&ft_info)) {
        printf("create failed.");
    }

    lv_split_jpeg_init();

    return 0;
}

void FT_font_Set(uint8_t size)
{
    ft_info.weight = size;
    lv_ft_font_init(&ft_info);
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
