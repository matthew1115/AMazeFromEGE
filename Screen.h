/*****************************************************************//**
 * \file   Screen.h
 * \brief  维护显示窗口的一些操作
 * 
 * \date   April 2022
 *********************************************************************/
#pragma once
#define SHOW_CONSOLE
#include <ege.h>

namespace screen {
	// 屏幕大小
	enum ScreenParam
	{
		Width = 1940,
		Height = 1080
	};

	class Screen
	{
		Screen();
		~Screen();
	public:
		// Meyers' Singleton, C++11及以后线程安全的单例模式
		static Screen& getScreen()
		{
			static Screen screen;
			return screen;
		}

		// 禁止拷贝
		Screen(const Screen& oth) = delete;
		Screen& operator=(const Screen& oth) = delete;
	};
}