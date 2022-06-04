#include "Screen.h"

namespace screen {
	Screen::Screen()
	{
		ege::initgraph(Width, Height,ege::INIT_ANIMATION);	//初始化图形界面
		ege::setbkcolor(ege::WHITE);	//设置背景颜色为白色
		ege::ege_enable_aa(true);
		ege::setrendermode(ege::RENDER_MANUAL);
		ege::setcaption("SCRABBLE!!!!!");
	}

	Screen::~Screen()
	{
		ege::closegraph();	//关闭图形界面
	}
}