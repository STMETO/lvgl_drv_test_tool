#include <page_manager.h>
#include <string.h>

//初始化链表头
static PPageAction g_ptPages = NULL;

// 注册页面
void Registered_Page(PPageAction ptPageAction)
{
	// 创建页面
	ptPageAction->Create();
	//头插法
	ptPageAction->ptNext = g_ptPages;
	g_ptPages = ptPageAction;	//当前页面的地址赋值给g_ptPages，即当前页面作为新的链表头

}

//通过名字在链表中找到对应的页面，并返回地址
PPageAction Page(char *name)
{
	PPageAction ptTmp = g_ptPages;	//链表头传给临时变量

	while (ptTmp)
	{
		if (strcmp(name, ptTmp->name) == 0)
			return ptTmp;
		ptTmp = ptTmp->ptNext;	//一直往下查找
	}

	return NULL;
}

//初始化
void PageSystemInit(void)
{
	//LVGL图形库初始化
	UI_Init();

	extern void Main_Page_Registered(void);	//声明是外部函数
	Main_Page_Registered();

	extern void Album_Page_Registered(void);
	Album_Page_Registered();

	extern void Camera_Page_Registered(void);
	Camera_Page_Registered();

	extern void Sr04_Page_Registered(void);
	Sr04_Page_Registered();

	extern void At24_Page_Registered(void);
	At24_Page_Registered();

	extern void DHT11_page_Registered(void);
	DHT11_page_Registered();

	extern void Sg90_page_Registered(void);
	Sg90_page_Registered();
}

