#pragma once

#ifndef COLOR_COUNT
#define COLOR_COUNT

#include <vector>
#include "basic/randomGirdGraph-inl.h"

namespace ColorCount
{
	typedef std::vector<my_long_t> color_list_t;

	color_list_t colorList;

	inline void assignColors(void);
	inline void pushColor(my_long_t);
	inline void pushColor_safe(my_long_t);
	inline color_list_t getColor(void);

	inline void assignColors(void)
	{
		colorList.clear();
		colorList.push_back(-1);
	}

	inline void pushColor(my_long_t $c)
	{
		colorList.push_back($c);
	}

	inline void pushColor_safe(my_long_t $c)
	{
		if (colorList[colorList.size() - 1] != $c)
		{
			pushColor($c);
		}
	}

	inline color_list_t getColor(void)
	{
		color_list_t temp;

		std::swap(temp, colorList);
		assignColors();

		return temp;
	}
}

#endif