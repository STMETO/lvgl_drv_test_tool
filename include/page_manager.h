#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include <ui.h>

//用于管理页面动作的链表结构体 PageAction，
//用于管理多个页面的注册、切换、运行等行为。
//每一个页面都要实现这些函数
typedef struct PageAction {
	char *name;		//页面名称
	void (*Run)(void *pParams);	//页面运行函数
	void (*Create)(void);		//页面创建函数
	struct PageAction *ptNext;	//指向下一个页面
}PageAction, *PPageAction;

void Registered_Page(PPageAction ptPageAction);	//页面注册函数，将一个PPageAction类型加入链表
void PageSystemInit(void);		//页面初始化
PPageAction Page(char *name);	//通过name属性在链表中查找对应名字的页面，返回这个页面的地址

#endif




