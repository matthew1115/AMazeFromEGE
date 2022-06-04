#define SHOW_CONSOLE
#include "Screen.h"
#include "Draw.h"

constexpr auto ROW_NUM = 50;
constexpr auto COL_NUM = 50;

int main()
{
	auto& Screen = screen::Screen::getScreen();
	basic::Draw draw({ 0,0 }, ROW_NUM, COL_NUM, 520, 520);
	basic::RoundRectButtons tmp(1200, 200, 200, 200, 20, 10, { "重新生成","显示路径","退出" },
		{
		[&] { draw.remake(); },
		[&] { draw.solve(); },
		[]  { exit(0); }
		}
	);

	for (; ege::is_run(); ege::delay_fps(60))
	{
		tmp.show();
	}
	return 0;
}

