#pragma once
/*****************************************************************//**
 * \file   Maze.h
 * \brief  迷宫基础类，同时提供打印到标准输出流的接口
 * 
 * \date   April 2022
 *********************************************************************/
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <string>
#include <stack>
#include <functional>
#include <algorithm>
//#include <cassrt>
namespace maze {
	using blockpic = std::vector<std::string>;
	const blockpic block{
			"▉","  ","※"
	};
	class locat {
	public:
		using rowType = size_t;
		using calType = size_t;

		locat(rowType r = 0, calType c = 0)
			:loc(r, c) {}

		rowType x(void)const { return loc.first; }	//返回一维坐标	
		rowType x(const rowType& r) { loc.first = r; return loc.first; }//修改并返回一维坐标		
		calType y(void)const { return loc.second; }		//返回二维坐标
		calType y(const calType& c) { loc.second = c; return loc.second; }//修改并返回二维坐标
		locat up(void)const { return { loc.first - 1, loc.second }; }
		locat down(void)const { return { loc.first + 1, loc.second }; }
		locat left(void)const { return { loc.first, loc.second - 1 }; }
		locat right(void)const { return { loc.first, loc.second + 1 }; }
		locat& operator()(const rowType& r, const calType& c) {
			x(r), y(c);
			return *this;
		}
		locat& operator()(const locat& oth) {
			return (*this)(oth.x(), oth.y());
		}
		bool operator<(const locat& oth)const {
			return x() == oth.x() ? y() < oth.y() : x() < oth.x();
		}
		bool operator==(const locat& oth)const {
			return x() == oth.x() && y() == oth.y();
		}
		friend std::ostream& operator<<(std::ostream& o, const locat& l)
		{
			o << '(' << l.x() << ',' << l.y() << ')';
			return o;
		}
	private:
		std::pair<rowType, calType> loc;
	};

	class Maze
	{
	public:
		using rowType = locat::rowType;
		using calType = locat::calType;
		using locats = std::vector<locat>;

		enum blockType {
			wall,
			road,
			way
		};

		Maze(const locat& l) :xyMsg(l), Map(l.x(), mazeLine(l.y(), blockType::wall)) {}
		Maze(rowType row, calType cal);		// 随机生成一个迷宫，采用Prim算法

		std::function<locat(const locat& l)> operat[4]{
			[](const locat& l) {return l.up(); },
			[](const locat& l) {return l.down(); },
			[](const locat& l) {return l.left(); },
			[](const locat& l) {return l.right(); },
		};

		auto& operator()(const rowType& r, const calType& c) {
			return Map[r][c];
		}
		auto& operator()(const locat& p) {
			return (*this)(p.x(), p.y());
		}

		rowType row(void) { return xyMsg.x(); }	// 返回迷宫的行数
		calType cal(void) { return xyMsg.y(); }	// 返回迷宫的列数
		locat& start(void) { return _start; }
		locat& end(void) { return _end; }
		void show(const blockpic pic = block);	// 打印迷宫

	private:
		using mazeLine = std::vector<blockType>;		// 单行迷宫
		using mazeMap = std::vector<mazeLine>;			// 迷宫图

		locats findWall(const locat& p);	//返回一个路周围的墙
		locats findRoad(const locat& p);	//返回一个墙周围的路

		locat xyMsg;
		mazeMap Map;
		locat _start, _end;
	};

	//给出迷宫问题的解决方案
	class Solute
		:public Maze
	{
	public:
		Solute(const rowType& r, const calType& c)
			:Maze(r, c) {
		}

		bool isIn(const locat& p) {
			return p.x() < row() && p.y() < cal();
		}
		void solve();
		bool solve(const locat& p);
		void showWay(void) {
			if (!ans.empty()) {
				std::cout << ans.top();
				ans.pop();
				if (!ans.empty())
					std::cout << " -> ";
				showWay();
			}
		};
	private:
		Maze mark{ locat{row(),cal()} };
		std::stack<locat> ans{};
	};
}