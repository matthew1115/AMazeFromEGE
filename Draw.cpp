#include "Draw.h"
namespace basic {
	void RoundRectButton::draw() const
	{
		ege::setfillcolor(color);

		ege::ege_fillrect((float)(x + radius), (float)(y),
			(float)(width - 2 * radius), float(height)
		);

		ege::ege_fillrect((float)(x), (float)(y + radius),
			(float)(radius), (float)(height - 2 * radius)
		);

		ege::ege_fillrect((float)(x + width - radius),
			(float)(y + radius),
			(float)(radius), (float)(height - 2 * radius)
		);

		float diameter = 2 * radius;
		float dx = width - diameter;
		float dy = height - diameter;


		ege::ege_fillpie((float)(x + dx), (float)(y + dy), diameter, diameter, 0.0f, 90.0f);
		ege::ege_fillpie((float)(x), (float)(y + dy), diameter, diameter, 90.0f, 90.0f);
		ege::ege_fillpie((float)(x), (float)(y), diameter, diameter, 180.0f, 90.0f);
		ege::ege_fillpie((float)(x + dx), (float)(y), diameter, diameter, 270.0f, 90.0f);

		text.show();
	}

	bool RoundRectButton::isInButton(const Point& p) const
	{
		bool inside = false;
		auto [px, py] = p; // C++17 -- 结构化绑定

		// 点在包围矩形内
		if ((px >= x) && (py >= y)
			&& (px < x + width)
			&& (py < y + height)
			) {

			Point center(x + width / 2.0f, y + width / 2.0f);
			float dx = fabs(px - center.x);
			float dy = fabs(py - center.y);
			float interWidth = width / 2.0f - radius;
			float interHeight = height / 2.0f - radius;

			// 点不在圆角空白处
			if (!((dx > interWidth)
				&& (dy > interHeight)
				&& ((dx - interWidth) * (dx - interWidth) + (dy - interHeight) * (dy - interHeight)
					> radius * radius)
				)
				) {
				inside = true;
			}
		}

		return inside;
	}

	bool RoundRectButton::isClick()
	{
		bool click{};
		while (ege::mousemsg())
		{
			auto msg{ ege::getmouse() };
			return msg.is_left() && msg.is_down() && isInButton(msg.x, msg.y);
		}
		return click;
	}

	void Text::show() const
	{
		ege::setfont(h, w, "宋体");
		ege::setcolor(color);
		ege::setbkmode(TRANSPARENT);								// 设置文字背景色为透明
		ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);	// 设置为中心对齐

		ege::xyprintf(p.x, p.y, text.c_str());
	}

	void RoundRectButtons::show() const
	{
		for (auto& i : buttons)
		{
			i.draw();
		}
		run();
	}

	bool RoundRectButtons::run()const
	{
		while (ege::mousemsg())
		{
			auto msg{ ege::getmouse() };
			if (msg.is_left() && msg.is_down())
			{
				for (auto& i : buttons)
				{
					if (i.isInButton(msg.x, msg.y)) {
						i.run();
						return true;
					}
				}
			}
		}
		return false;
	}

	ege::color_t Block::Color[3]{
		ege::BROWN,
		ege::DARKGRAY,
		ege::YELLOW
	};

	Block::Block(int w, int h)
		:w(w), h(h)
	{


		for (block i = static_cast<block>(block::begin + 1); i != block::end; i = static_cast<block>(i + 1))
		{
			//std::string tmp = std::string("./pic/") + (char)(i + 'A') + std::string(".jpg");
			//std::string tmp = "./pic/" + 'A' + std::string(".jpg");
			//blocks.emplace_back(tmp, w, h);
			//blocks.emplace_back("./pic/A.jpg", w, h);
			//blocks.push_back(ege::newimage(w, h));

			ege::PIMAGE tmp = ege::newimage(w, h);
			ege::ege_enable_aa(true, tmp);
			ege::setfillcolor(Color[i], tmp);
			ege::bar(0, 0, w, h, tmp);
			blocks.push_back(tmp);
			//std::cout << Color[i] << ' '<< ege::getpixel(w / 2, h / 2, blocks[i]) << std::endl;
		}
	}

	void Block::draw(const Point& p, block num)
	{
		blocks[num].show(p.x, p.y);
		//ege::putimage(p.x, p.y, blocks[num]);
	}

	void Block::drawWall(const Point& p)
	{
		draw(p, block::wall);
	}

	void Block::drawRoad(const Point& p)
	{
		draw(p, block::road);
	}

	void Block::drawWay(const Point& p)
	{
		draw(p, block::way);
	}

	void Draw::show()
	{
		Point tmp = p;
		for (size_t i = 0; i < m.row(); i++)
		{
			tmp.x = p.x;
			for (size_t j = 0; j < m.cal(); j++)
			{
				b.draw(tmp, static_cast<Block::block>(m(i, j)));
				tmp.x += b.getW();
			}
			tmp.y += b.getH();
		}
	}
}