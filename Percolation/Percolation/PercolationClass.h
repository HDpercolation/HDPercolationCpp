#pragma once
#ifndef PERCOLATIONCLASS
#define PERCOLATIONCLASS

#include "Types.h"
#include <random>
#include "PercolationClass_function.h"
#include "PercolationClass_SowReap.h"
#include "PercolationClass_counter.h"

using namespace PCfunction;

class PercolationMap
{
public:
	// Build function
	PercolationMap(int $d, int $m, double $p, long seed = 20) :
		gird_map(_node_x($d), $d, $m, $p),	// Make initial map
		warpping_counter($d, $m),
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

private:
	_Graph_x gird_map;

	// Basic data
	int d, m;
	double p;

	// Count data
	_datas data;	// Sow-Reap
	counter warpping_counter;

	// random
	std::mt19937 random_MTE;

	// inside functions
	int reMakeMap(GRAPH *, unsigned int, unsigned int, double);
	int inital_set_bond(GRAPH *, unsigned int, unsigned int, double);
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
		// TODO 此处可用泊松分布 + 位运算优化之
		for (int j = 0; j < $d; ++j) {
			if (random_MTE() < g->p_int) {
				(g->$map)[i].links[j] = true;
				// 添加一个反向边
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

#endif