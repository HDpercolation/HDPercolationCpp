#pragma once

#ifndef WRAPPINGCOUNT
#define WRAPPINGCOUNT
#include <vector>

namespace WrappingCount
{
	typedef std::vector<double> counters;

	/*===========================================*/

	//counters counter;
	long one, more_than_one, more_than_one_total;
	double average;
	int d, m;

	/*===========================================*/

	inline void assignCounter(int, int);
	inline void pushIn(int, long);
	inline counters getCounter(void);

	/*===========================================*/

	inline void assignCounter(int $d, int $m)
	{
		/* ��;�����Գ�ʼ��������
		 * ������$d ά��
		 */
		//counter.assign($d, 0);
		d = $d;
		m = $m;


		average = 0.0;
		one = 0;
		more_than_one = 0;
		more_than_one_total = 0;
	}

	inline void pushIn(int bond, long move)
	{
		int r = (move + 1) / m;
		if (r <= 1)
		{
			one++;
		} else 
			if (r > 1)
			{
				one++;
				more_than_one++;
				more_than_one_total += r;
			}
	}

	inline counters getCounter(void)
	{
		/*�����ѵõ����ݣ������������*/
		counters temp_c(2, 0.0);

		//std::swap(temp_c, counter);
		if (one == 0)
		{
			temp_c[0] == 0.0;
			temp_c[1] == 0.0;
		} else
		{
			temp_c[0] == static_cast<double>(more_than_one) / static_cast<double>(one);			// ����1��wrapping����
			temp_c[1] == static_cast<double>(more_than_one_total) / static_cast<double>(one);	// ��Ȩ�ĳ���1��wrapping����
		}

		return temp_c;
	}
}

#endif // WRAPPINGCOUNT
