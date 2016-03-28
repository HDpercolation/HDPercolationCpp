#pragma once

#ifndef WRAPPINGCOUNT
#define WRAPPINGCOUNT
#include <vector>

namespace WrappingCount
{
	typedef std::vector<int> counters;

	/*===========================================*/

	counters counter;
	int d, m;

	/*===========================================*/

	inline void assignCounter(int, int);
	inline void pushIn(int, long);
	inline counters getCounter(void);

	/*===========================================*/

	inline void assignCounter(int $d, int $m)
	{
		/* 用途：用以初始化计数器
		 * 参数：$d 维度
		 */
		counter.assign($d, 0);
		d = $d;
		m = $m;
	}

	inline void pushIn(int bond, long move)
	{
		int r = (move + 1) / m;
		if (r <= counter[bond])
		{
			return;
		}
		counter[bond] = r;
	}

	inline counters getCounter(void)
	{
		/*返回已得的数据，并清除旧数据*/
		counters temp_c(d,0);

		std::swap(temp_c, counter);
		assignCounter(d, m);

		return temp_c;
	}
}

#endif // WRAPPINGCOUNT
