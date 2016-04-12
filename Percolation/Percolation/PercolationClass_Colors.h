#pragma once

#include <vector>

namespace colorCount
{
	typedef std::vector<size_t> color_list_t;
}

class colorCounter
{
public:
	colorCounter(void):
		colorList(1, -1)
	{}

	void assignColors(void);
	void pushColor(size_t);
	void pushColor_safe(size_t);
	colorCount::color_list_t getColor(void);
private:
	colorCount::color_list_t colorList;
};

inline void colorCounter::assignColors(void)
{
	colorList.clear();
	colorList.push_back(-1);
}

inline void colorCounter::pushColor(size_t $c)
{
	colorList.push_back($c);
}

inline void colorCounter::pushColor_safe(size_t $c)
{
	if (colorList[colorList.size() - 1] != $c)
	{
		pushColor($c);
	}
}

inline colorCount::color_list_t colorCounter::getColor(void)
{
	colorCount::color_list_t temp;

	std::swap(temp, colorList);
	assignColors();

	return temp;
}