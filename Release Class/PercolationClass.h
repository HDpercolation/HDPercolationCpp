#pragma once
#ifndef PERCOLATIONCLASS
#define PERCOLATIONCLASS

#include "Types.h"
#include <random>
#include "PercolationClass_function.h"
#include "PercolationClass_SowReap.h"
#include "PercolationClass_counter.h"
#include "PercolationClass_Colors.h"

using namespace PCfunction;

class PercolationMap
{
public:
	// Build function
	PercolationMap(int $d, int $m, double $p, long seed = 20) :
		gird_map(_node_x($d), $d, $m, $p),	// Make initial map
		warpping_counter($d, $m),
		colors(),
		random_MTE(seed)
	{
		this->d = $d;
		this->m = $m;
		this->p = $p;
	}

	// Basic information:
	int dim(void);
	int len(void);
	double possible(void);

	// Make function
	void make(void);
	void reMake(double);

	// Working function
	void BFS(void);

	// Results
	std::vector<int> warppingCounterData(void);

private:
	_Graph_x gird_map;

	// Basic data
	int d, m;
	double p;

	// Count data
	_datas data;	// Sow-Reap
	counter warpping_counter;
	colorCounter colors;

	// random
	std::mt19937 random_MTE;

	// inside functions
	int reMakeMap(GRAPH *, unsigned int, unsigned int, double);
	int inital_set_bond(GRAPH *, unsigned int, unsigned int, double);

	long BFS_in(GRAPH *);
	long BFS_point(GRAPH *, long, long);
	void visit_points3(GRAPH *, std::deque<g_size_t> *, long, bool *);
	inline bool is_warpping(GRAPH *, _move *, _move *, _move *);
};


/*==========Information===============*/

inline int PercolationMap::dim()
{
	return this->d;
}

inline int PercolationMap::len()
{
	return this->m;
}

inline double PercolationMap::possible()
{
	return this->p;
}

/*==========Make & Remake===============*/

int PercolationMap::reMakeMap(GRAPH *g, unsigned int $d, unsigned int $m, double p)
{
	for (auto i = 0; i < g->size; ++i)
	{
		g->$map[i].color = 0;
		g->$map[i].move.clear();
		g->$map[i].have_link_Q = false;
		g->$map[i].visited = false;
		for (int j = 0; j < $d * 2; ++j)
		{
			g->$map[i].links[j] = false;
		}
	}

	inital_set_bond(g, $d, $m, p);

	return 0;
}

void PercolationMap::make(void)
{
	inital_set_bond(&gird_map, d, m, p);
}

void PercolationMap::reMake(double $p)
{
	this->p = $p;
	warpping_counter.assignCounter(d, m);
	colors.assignColors();

	reMakeMap(&gird_map, d, m, p);
}

/*==========Working functions============*/

inline void PercolationMap::BFS()
{
	BFS_in(&(this->gird_map));
}


/*==========inside functions=============*/

int PercolationMap::inital_set_bond(GRAPH *g, unsigned int $d, unsigned int $m, double p) {
	long count_link = 0;
	std::bernoulli_distribution RandomRange(p);

	long max = POWER($m, $d);
	for (long i = 0; i < max; ++i) {
		// TODO �˴����ò��ɷֲ� + λ�����Ż�֮
		for (int j = 0; j < $d; ++j) {
			if (random_MTE() < g->p_int) {
				(g->$map)[i].links[j] = true;
				// ���һ�������
				_position p_now(g->d, 0);
				PCfunction::get_position_q(i, g->d, g->m, &p_now);

				long linked_index = PCfunction::get_linked_index(g, &p_now, j);
				(g->$map)[linked_index].links[j + g->d] = true;
				count_link++;
			}
		}
	}

	return count_link;
}

inline bool PercolationMap::is_warpping(GRAPH *g, _move *m1, _move *m2, _move *m3) {
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

void PercolationMap::visit_points3(GRAPH *g, std::deque<g_size_t> *go_visit, long $color, bool *reported_a_wrappingQ)
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
				//SOW_DATA;
				warpping_counter.pushIn(bond_index % g->d, total_move[bond_index % g->d]);
				if (!(*reported_a_wrappingQ))
				{
					colors.pushColor($color);
					(*reported_a_wrappingQ) = true;
				}
			}
		}
	}

	swap(now_move, (g->$map[index]).move);
}

long PercolationMap::BFS_point(GRAPH *g, long index, long $color) {
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

long PercolationMap::BFS_in(GRAPH *g) {
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

/*==========Result functions==============*/

std::vector<int> PercolationMap::warppingCounterData(void)
{
	return (this->warpping_counter).getCounter();
}

#endif