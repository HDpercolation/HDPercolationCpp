#pragma once

#include "Types.h"
#include <deque>

#define GRAPH			_Graph_x
#define GET_INDEX		get_index2
#define LINKED_INDEX	linked_index2

namespace PCfunction
{
	typedef std::vector<std::vector<long>> _datas;
	// 用以存储数据


	void get_position_q(long, long, long, _position *);
	inline long get_linked_index(GRAPH *, _position *, int); // 获得连边的编号
	long get_index2(GRAPH *, _position *);// 获得点编号
	long linked_index2(GRAPH *, long, int);

	//============================================================

	

	inline long linked_index2(GRAPH *g, long index, int bond_id) {
		long exp_1, exp_2, low_part, change_part, high_part;
		int bond_id_2;


		if (bond_id < g->d) {
			bond_id_2 = g->d - bond_id - 1;
		}
		else {
			bond_id_2 = 2 * g->d - bond_id - 1;
		}

		exp_1 = POWER(g->m, bond_id_2 % g->d);
		exp_2 = exp_1 * g->m;

		low_part = index % exp_1;
		change_part = index / exp_1;
		change_part %= g->m;

		high_part = index / exp_2;
		high_part *= exp_2;


		//========================================
		change_part += g->m;

		if (bond_id < g->d) {
			// 若为增加之方向
			change_part++;
			change_part = change_part % g->m;
		}
		else {
			// 若为减少之方向
			change_part--;
			change_part = change_part % g->m;
		}

		return high_part + low_part + change_part * exp_1;
	}

	void get_position_q(long index, long d, long m, _position *p) {
		auto $index = index;
		int k = d - 1;

		// (*p)[0]是最高位
		while (k >= 0) {
			(*p)[k] = $index % m;
			$index = $index / m;

			k--;
		}
	}

	long get_index2(GRAPH *g, _position *p) {
		long temp = (*p)[0];
		long m = g->m;
		int size = (*p).size();

		// (*p)[0] 是最高位
		for (int i = 1; i < size; ++i) {
			temp *= m;
			temp += (*p)[i];
		}

		return temp;
	}

	inline long get_linked_index(GRAPH *g, _position *p, int link) {
		(*p)[link] = ((*p)[link] + 1) % (g->m);
		return GET_INDEX(g, p);
	}
}