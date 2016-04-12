#pragma once

#include "Types.h"
#include <deque>

#define GRAPH			_Graph_x
#define GET_INDEX		get_index2
#define LINKED_INDEX	linked_index2

namespace PCfunction
{
	typedef std::vector<std::vector<long>> _datas;
	// ���Դ洢����


	void get_position_q(long, long, long, _position *);
	inline long get_linked_index(GRAPH *, _position *, int); // ������ߵı��
	long get_index2(GRAPH *, _position *);// ��õ���
	long BFS_in(GRAPH *);
	long BFS_point(GRAPH *, long, long);
	void visit_points3(GRAPH *, std::deque<g_size_t> *, long, bool *);
	long linked_index2(GRAPH *, long, int);
	inline bool is_warpping(GRAPH *, _move *, _move *, _move *);

	//============================================================

	inline bool is_warpping(GRAPH *g, _move *m1, _move *m2, _move *m3) {
		/* =============================================================
		* ���ܣ��ж�m1��m2��λ���Ƿ���Թ��ɻ����������ľ���ֵ����m3
		* ���أ�true, false
		* ���ԣ��Ѳ���
		*/


		///////////////////////////////

		for (int i = 0; i < (*m1).size() - 1; ++i) {
			(*m3)[i] = abs((*m1)[i] - (*m2)[i]);
		}

		for (int j = 0; j < (*m1).size() - 1; ++j) {
			if (((*m3)[j] + 1) % g->m == 0 && (*m3)[j] != 0) {
				(*m3)[(*m3).size() - 1] = (*m1)[(*m1).size() - 1];
				// ��Ǵ˻���Ӧ����ɫ
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
			// ��Ϊ����֮����
			change_part++;
			change_part = change_part % g->m;
		}
		else {
			// ��Ϊ����֮����
			change_part--;
			change_part = change_part % g->m;
		}

		return high_part + low_part + change_part * exp_1;
	}

	void visit_points3(GRAPH *g, std::deque<g_size_t> *go_visit, long $color, bool *reported_a_wrappingQ)
	{
		g_size_t index = *(*go_visit).begin();		// ��ǰ�ĵ�
		g_size_t to_visit_index;					// ���õ�
		_move now_move;								// ��ǰ��λ��
		_move total_move(g->d + 1, 0);				// ��¼��λ��


													/*��ʼ��==========================================*/
		(*go_visit).pop_front();

		total_move[g->d] = g->$map[index].color;
		swap(now_move, (g->$map[index]).move);

		/*���ʸ����ӵ�=====================================*/
		for (int bond_index = 0; bond_index < 2 * (g->d); ++bond_index) {

			// ���˱���ͨ��������Ӧ�ı�ţ����н�һ�����㣻��������
			if (!(g->$map)[index].links[bond_index]) {
				continue;
			}
			to_visit_index = LINKED_INDEX(g, index, bond_index);

			// ���˽ڵ�δ���ʣ���Ⱦɫ������������б�֮��
			// ͬʱ�������Ӧ��λ�Ƽ���map֮��
			if (!(g->$map)[to_visit_index].visited) {
				g->$map[to_visit_index].move = now_move;
				(*go_visit).push_back(to_visit_index);						// ��������б�֮��
				(g->$map)[to_visit_index].visited = true;					// ����ѷ���
				(g->$map)[to_visit_index].color = (g->$map)[index].color;	// Ⱦɫ

																			// ����λ���б�:
																			/* ˵����if (bond_index < g->d) ��ζ������index���ߵķ�����������ȼӺ����
																			* ��������index���͵ķ�����������ȼ���ӡ�
																			* ���µ�λ�Ƽ�¼����map֮��
																			*/
				if (bond_index < g->d) {
					g->$map[to_visit_index].move[bond_index]++;
				}
				else {
					g->$map[to_visit_index].move[bond_index - g->d]--;
				}
			}

			// �ж��Ƿ��л��ƣ������֮

			// ���˵��ڱ�Ե�ϣ�����Ƚϣ����Ƿ񹹳ɻ�
			if (!(g->$map[to_visit_index].move.empty()) && !(now_move.empty())) {
				// ��(*moveMap)[index], now_move��Ӧ��λ�ƿ��Թ��ɻ�����洢�˵㣬����
				// ��λ�ƴ���total_move֮�С�
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
		* ���ܣ���һ���㿪ʼ������BFS��������Ȧ��������warpping֮��
		* ������ͼָ�룬���ţ���ɫ
		* ���أ��ع�ģ
		* ================================
		*/
		std::deque<g_size_t> go_lists;

		_move temp_move(g->d + 1, 0); // ���һ����ֵ���Դ洢��ɫ
		bool reported_a_wrappingQ = false;
		temp_move[g->d] = $color;

		g_size_t size = 1;
		g_size_t deep = 0;

		// λ�Ƴ�ʼ�� ==============================================================================================
		swap(g->$map[index].move, temp_move);// ���ô˵�ĳ�ʼֵ
											 // =========================================================================================================

											 // ����ǰ����Ϊ��ʼ�㣺
		go_lists.push_back(index);

		g->$map[index].visited = true;
		g->$map[index].color = $color;

		_links none_link(g->d * 2, false);
		if (g->$map[index].links == none_link) {
			g->$map[index].color = 0;
			return 0;
		}

		// �Ӵ˵㿪ʼ����BFS������Ⱦɫ�����
		while (!go_lists.empty()) {
			// ��ǡ�Ⱦɫ��

			g->$map[*go_lists.begin()].color = $color;

			visit_points3(g, &go_lists, $color, &reported_a_wrappingQ);
		}

		return size;
	}

	long BFS_in(GRAPH *g) {
		long groups = 1;
		// Ⱦɫ���
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

		// (*p)[0]�����λ
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

		// (*p)[0] �����λ
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