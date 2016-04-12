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
	long BFS_in(GRAPH *);
	long BFS_point(GRAPH *, long, long);
	void visit_points3(GRAPH *, std::deque<g_size_t> *, long, bool *);
	long linked_index2(GRAPH *, long, int);
	inline bool is_warpping(GRAPH *, _move *, _move *, _move *);

	//============================================================

	inline bool is_warpping(GRAPH *g, _move *m1, _move *m2, _move *m3) {
		/* =============================================================
		* 功能：判断m1与m2的位移是否可以构成环，并将其差的绝对值存入m3
		* 返回：true, false
		* 测试：已测试
		*/


		///////////////////////////////

		for (int i = 0; i < (*m1).size() - 1; ++i) {
			(*m3)[i] = abs((*m1)[i] - (*m2)[i]);
		}

		for (int j = 0; j < (*m1).size() - 1; ++j) {
			if (((*m3)[j] + 1) % g->m == 0 && (*m3)[j] != 0) {
				(*m3)[(*m3).size() - 1] = (*m1)[(*m1).size() - 1];
				// 标记此环对应的颜色
				return true;
			}
		}

		return false;
	}

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

	void visit_points3(GRAPH *g, std::deque<g_size_t> *go_visit, long $color, bool *reported_a_wrappingQ)
	{
		g_size_t index = *(*go_visit).begin();		// 当前的点
		g_size_t to_visit_index;					// 待访点
		_move now_move;								// 当前的位移
		_move total_move(g->d + 1, 0);				// 记录总位移


													/*初始化==========================================*/
		(*go_visit).pop_front();

		total_move[g->d] = g->$map[index].color;
		swap(now_move, (g->$map[index]).move);

		/*访问各连接点=====================================*/
		for (int bond_index = 0; bond_index < 2 * (g->d); ++bond_index) {

			// 若此边联通，则计算对应的编号，进行进一步计算；否则跳过
			if (!(g->$map)[index].links[bond_index]) {
				continue;
			}
			to_visit_index = LINKED_INDEX(g, index, bond_index);

			// 若此节点未访问，则染色，并加入待访列表之中
			// 同时，将其对应的位移加入map之中
			if (!(g->$map)[to_visit_index].visited) {
				g->$map[to_visit_index].move = now_move;
				(*go_visit).push_back(to_visit_index);						// 存入待访列表之中
				(g->$map)[to_visit_index].visited = true;					// 标记已访问
				(g->$map)[to_visit_index].color = (g->$map)[index].color;	// 染色

																			// 更新位移列表:
																			/* 说明：if (bond_index < g->d) 意味着是向index升高的方向遍历，故先加后减；
																			* 否则是向index降低的方向遍历，是先减后加。
																			* 将新的位移记录存入map之中
																			*/
				if (bond_index < g->d) {
					g->$map[to_visit_index].move[bond_index]++;
				}
				else {
					g->$map[to_visit_index].move[bond_index - g->d]--;
				}
			}

			// 判断是否有环绕，并输出之

			// 若此点在边缘上，则进比较，看是否构成环
			if (!(g->$map[to_visit_index].move.empty()) && !(now_move.empty())) {
				// 若(*moveMap)[index], now_move对应的位移可以构成环，则存储此点，并将
				// 总位移存入total_move之中。
				if (is_warpping(g, &now_move, &(g->$map[to_visit_index].move), &total_move)) {
					SOW_DATA;
					WrappingCount::pushIn(bond_index % g->d, total_move[bond_index % g->d]);
					if (!(*reported_a_wrappingQ))
					{
						ColorCount::pushColor($color);
						(*reported_a_wrappingQ) = true;
					}
				}
			}
		}

		swap(now_move, (g->$map[index]).move);
	}

	long BFS_point(GRAPH *g, long index, long $color) {
		/* ================================
		* 功能：从一个点开始，进行BFS，计算绕圈数，存入warpping之中
		* 参数：图指针，点编号，颜色
		* 返回：簇规模
		* ================================
		*/
		std::deque<g_size_t> go_lists;

		_move temp_move(g->d + 1, 0); // 最后一个数值用以存储颜色
		bool reported_a_wrappingQ = false;
		temp_move[g->d] = $color;

		g_size_t size = 1;
		g_size_t deep = 0;

		// 位移初始化 ==============================================================================================
		swap(g->$map[index].move, temp_move);// 设置此点的初始值
											 // =========================================================================================================

											 // 将当前点设为初始点：
		go_lists.push_back(index);

		g->$map[index].visited = true;
		g->$map[index].color = $color;

		_links none_link(g->d * 2, false);
		if (g->$map[index].links == none_link) {
			g->$map[index].color = 0;
			return 0;
		}

		// 从此点开始进行BFS，进行染色、标记
		while (!go_lists.empty()) {
			// 标记、染色：

			g->$map[*go_lists.begin()].color = $color;

			visit_points3(g, &go_lists, $color, &reported_a_wrappingQ);
		}

		return size;
	}

	long BFS_in(GRAPH *g) {
		long groups = 1;
		// 染色编号
		long group_size = 0;
		std::size_t g_size = g->size;

		for (long pointIndex = 0; pointIndex < g_size; ++pointIndex) {
			if (!g->$map[pointIndex].visited) {
				group_size += BFS_point(g, pointIndex, groups);
				groups++;
			}
		}

		return groups - 1;
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