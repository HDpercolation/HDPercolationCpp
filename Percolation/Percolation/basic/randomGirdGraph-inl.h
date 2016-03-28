//
// Created by Yanbo on 2016/3/15.
//

#ifndef PERCOLATION_RANDOMGIRDGRAPH_INL_H
#define PERCOLATION_RANDOMGIRDGRAPH_INL_H

#include <vector>

typedef int32_t g_size_t;
typedef int32_t my_long_t;

inline long long_pow(int, int);
inline long long_pow2(int, int);

inline long long_pow(int a, int b) {
	long temp_ans = 1;
	for (int i = 0; i < b; ++i) {
		temp_ans *= a;
	}

	return temp_ans;
}

inline long long_pow2(int a, int b) {
	long temp_ans = 1;

	if (b >= 2) {
		long ap = a * a;
		int l_1 = b / 2;
		int l_2 = b % 2;
		for (int i = 0; i < l_1; ++i) {
			temp_ans *= ap;
		}
		for (int j = 0; j < l_2; ++j) {
			temp_ans *= a;
		}
	}
	else {
		for (int i = 0; i < b; ++i) {
			temp_ans *= a;
		}
	}

	return temp_ans;
}

void ClearVector(std::vector<g_size_t> *vt)
{
	std::vector<my_long_t> vtTemp;
	vtTemp.swap(*vt);
}

#endif //PERCOLATION_RANDOMGIRDGRAPH_INL_H
