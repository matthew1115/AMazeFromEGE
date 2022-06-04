#include "Maze.h"


maze::Maze::Maze(rowType row, calType cal)
	:xyMsg(2 * row + 1, 2 * cal + 1), Map(2 * row + 1, mazeLine(2 * cal + 1, wall))
{
	// 初始化随机数生成器
	static std::random_device rd;
	static std::default_random_engine e(rd());
	static std::uniform_int_distribution<> d;

	std::map<blockType, std::set<locat>> mark{
		{wall,{}},{road,{}}
	};

	for (rowType i = 1; i < row * 2 + 1; i += 2)
	{
		for (calType j = 1; j < cal * 2 + 1; j += 2)
		{
			(*this)(i, j) = road;
		}
	}

	//随机生成起点，把边框分为四段
	auto i = d(e, decltype(d)::param_type{ 0,3 });
	auto j = i % 2 
		? d(e, decltype(d)::param_type{ 0,(int)row - 2 }) 
		: d(e, decltype(d)::param_type{ 0,(int)cal - 2 });
	switch (i)
	{
	case 0:
		_start(j, 0); break;
	case 1:
		_start(0, j - 1); break;
	case 2:
		_start(row - 1, j); break;
	case 3:
		_start(j + 1, cal - 1); break;
	}
	_start(_start.x() * 2 + 1, _start.y() * 2 + 1);	//将起点对应到实际位置
	locats tmpRoad{ _start };
	locats tmpWall = findWall(_start);
	mark[road].insert(tmpRoad.begin(), tmpRoad.end());
	mark[wall].insert(tmpWall.begin(), tmpWall.end());

	while (!mark[wall].empty())
	{
		auto it = mark[wall].begin();
		std::advance(it,
			d(e, decltype(d)::param_type{ 0,(int)mark[wall].size() - 1 }));
		tmpRoad = findRoad(*it);		//随机将一个wall集合中的元素传入findRoad
		auto s1 = mark[road].size();	//插入set前set大小
		bool flag = false;
		for (auto& i : tmpRoad)
		{
			mark[road].insert(i);
			if (s1 != mark[road].size()) {
				s1 = mark[road].size();
				tmpWall = findWall(i);
				mark[wall].insert(tmpWall.begin(), tmpWall.end());
				flag = true;
			}
		}
		//若size有变化，表示此wall周围的road有未标记的，将此wall置为road
		if (flag) {
			mark[road].insert(*it);
			(*this)(*it) = road;
		}
		mark[wall].erase(it);
	}
	_end(tmpRoad.back());
}

void maze::Maze::show(const blockpic pic)
{
	size_t m{}, n{};
	for (const auto& i : Map)
	{
		for (const auto& j : i)
		{
			std::cout << pic[j];
		}
		std::cout << m++ << std::endl;
	}
	for (size_t i = 0; i < cal(); printf("%2zd", i++));
}

maze::Maze::locats maze::Maze::findWall(const locat& p)
{
	locats ret;
	locat tmp;
	if (p.x() != 1) {
		tmp = p.up();
		if ((*this)(tmp) == wall)
			ret.push_back(tmp);
	}
	if (p.y() != 1) {
		tmp = p.left();
		if ((*this)(tmp) == wall)
			ret.push_back(tmp);
	}
	if (p.x() != row() - 2) {
		tmp = p.down();
		if ((*this)(tmp) == wall)
			ret.push_back(tmp);
	}
	if (p.y() != cal() - 2) {
		tmp = p.right();
		if ((*this)(tmp) == wall)
			ret.push_back(tmp);
	}
	return ret;
}

maze::Maze::locats maze::Maze::findRoad(const locat& p)
{
	//assert(p.x() != 0 && p.x() != row() && p.y() != 0 && p.y() != cal());

	locats ret;
	locat tmp;

	for (auto& i : operat)
	{
		tmp = i(p);
		if ((*this)(tmp) == road)
			ret.push_back(tmp);
	}

	return ret;
}

void maze::Solute::solve()
{
	(*this)(end()(1, 1)) = way;
	(*this)(start()(row() - 2, cal() - 2)) = way;
	
	solve(start());
}

bool maze::Solute::solve(const locat& p)
{
	if (p == end()) return true;
	mark(p) = road;
	(*this)(p) = way;
	ans.push(p);

	for (auto& i : operat)
	{
		auto tmp = i(p);
		if (isIn(tmp) && (*this)(tmp) != wall
			&& mark(tmp) != road && solve(tmp)) {
			return true;
		}
	}

	ans.pop();
	mark(p) = wall;
	(*this)(p) = road;
	return false;
}