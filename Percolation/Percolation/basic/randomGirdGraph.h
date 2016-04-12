/*
* Copyright 2016 Zhang Yanbo, USTC
* Created by Zhang Yanbo on 2016/3/13.
* Version 1.0.0 2016/03/19
*
* 说明：===============================================================
*      此程序用以对N维、边长为m的网格进行键逾渗计算。
*      提供一个GRAPH类型，作为运算主体，其他所有算法都是对此结构的处理
*      提供算法：
*              * makeMap   用以对一个GRAPH类型进行初始化
*              * BFS       对一个已初始化的GRAPH进行广度优先搜索，进行
*                          染色。同时，遇到成环的情形，使用Sow()抛出此
*                          点。
* =====================================================================
* TODO 待优化：
*      速度：
*          1. 随机取点使用内置泊松分布
*      算法：
*/

#ifndef PERCOLATION_RANDOMGIRDGRAPH_H
#define PERCOLATION_RANDOMGIRDGRAPH_H

#ifndef PERCOLATION_RANDOMGIRDGRAPH_H

#include <vector>
#include <array>
#include <cmath>
#include "randomAutomata.h"
#include "randomGirdGraph-inl.h"
#include "SowReap.h"
#include "wrappingCount.h"
#include "Types.h"
#include <random>
#include "ColorCount.h"
#include <deque>

//原定义的默认写法=====================
//#define GRAPH			_Graph
#define Template		 

//用以优化的新函数=====================
#define GRAPH			_Graph_x
#define POWER			long_pow2
#define GET_INDEX		get_index2x
#define LINKED_INDEX	linked_index2
#define MAP				_map
#define NODE			_node_2
//#define Template		template <long $d, long $m>


using namespace std;

namespace BondPercolation {

	//全局变量=============================
	///int G_d, G_m;


	//====================================

	typedef bool _link;
	// 用这个来存储连边状态，是用以减少空间，在10亿个点的情形下，用long存储需要花费1T的内存

	schrageRandInt randomer = schrageRandInt(10);
	std::mt19937 random_MTE(10);

	typedef vector<_link> _links;
	typedef vector<int> _position;

	struct _node {
		// 节点
		long color = 0;
		// 染色，0为未染色
		bool visited = false;
		// 是否被访问
		_links links;
		// 连边
		vector<long> move;
		// 从初始点到此的位移，注意及时清理！
	};

	template <int d>
	struct _node_2 {
		// 节点
		long color = 0;
		// 染色，0为未染色
		bool visited = false;
		// 是否被访问
		///_links links;
		///bool links[2 * d];
		array<bool, 2 * d> links;
		bool have_link_Q = false;
		// 连边
		vector<long> move = {};
		// 从初始点到此的位移，注意及时清理！
	};

	typedef vector<_node> _map;
	struct _Graph {
		MAP $map;
		// 储存一个图
		int d, m, dx;
		// d，维度；m，边长
		vector<long> shift;
		// 预处理的位移
		long p_int;
		// p_int = (long)(2147483647 * p) 用以优化
	};

	// 连边转换函数 ===============================================================
	/* 主函数：     linked_index
	* 参量：      图指针，点编号， 边编号
	* 返回值：     连边点编号
	* 测试：      已测试
	*/
	Template long linked_index2(GRAPH *, long, int);

	Template long get_index(GRAPH *, _position *);// 获得点编号
	Template long get_index2x(GRAPH *, _position *);// 获得点编号
	Template inline long get_linked_indexxx(GRAPH *, _position *, int); // 获得连边的编号

	// 构造函数 ===================================================================
	/* 主函数：     makeMap
	* 参量：      图指针，维度，边长，概率
	* 返回值：     int, 构建的连边数
	* 说明：      此函数会清空原图
	* 测试：      测试完成
	* 效率：      目前每秒大约能添加800万个点
	*/
	Template int makeMap(GRAPH *, unsigned int, unsigned int, double);
	int reMakeMap(GRAPH *, unsigned int, unsigned int, double);
	Template int inital_set_bond(GRAPH *, unsigned int, unsigned int, double);
	// 随机设置连边
	void get_position_qx(long, long, long, _position *);
	// 获得坐标
	Template inline int putLink(GRAPH *, long, int, double);// 放置无向边
	inline int putLink_p(GRAPH *, std::vector<_node_x>::iterator, std::size_t, int, double);
	// BFS ========================================================================
	/* 主函数：     BFS
	* 参量：       图指针
	* 返回值：     聚类数
	* 说明：      会对GRAPH中，map项中每个节点的color项进行染色，从1开始
	*              同时计算所绕圈数，存在wrapping之中
	* 测试：      已测试
	*/
	Template long BFS(GRAPH *);
	Template long BFS_point(GRAPH *, long, long);
	void visit_points3(GRAPH *, deque<g_size_t> *, long, bool *);
	Template inline bool is_warpping(GRAPH *, _move *, _move *, _move *);


	// 具体代码 ===================================================================
	// ============================================================================

	Template
	int makeMap(GRAPH *g, unsigned int $d, unsigned int $m, double p) {
		long count;

		count = inital_set_bond(g, $d, $m, p);

		return 1;
	}

	int reMakeMap(GRAPH *g, unsigned int $d, unsigned int $m, double p)
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

	Template
	int inital_set_bond(GRAPH *g, unsigned int $d, unsigned int $m, double p) {
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
					get_position_q(i, g->d, g->m, &p_now);

					long linked_index = get_linked_index(g, &p_now, j);
					(g->$map)[linked_index].links[j + g->d] = true;
					count_link++;
				}
			}
		}

		return count_link;
	}

	void get_position_qx(long index, long d, long m, _position *p) {
		auto $index = index;
		int k = d - 1;

		// (*p)[0]是最高位
		while (k >= 0) {
			(*p)[k] = $index % m;
			$index = $index / m;

			k--;
		}
	}

	Template
	inline int putLink(GRAPH *g, long index, int link, double p) {
		/* ===================================================
		* 功能：以概率p在index的第link个方向上放一无向条边
		* 参量：图指针，点编号，方向，概率
		* 返回：成功放点则返回1， 否则为0
		*/


		if (randomer.getNum() < g->p_int) {
			(g->$map)[index].links[link] = true;
			// 添加一个反向边
			_position p_now(g->d, 0);
			get_position_q(index, g->d, g->m, &p_now);

			long linked_index = get_linked_index(g, &p_now, link);
			(g->$map)[linked_index].links[link + g->d] = true;

			return 1;
		}
		else
			return 0;
	}

	inline int putLink_p(GRAPH *g, std::vector<_node_x>::iterator point,std::size_t index, int link, double p)
	{
		if (randomer.getNum() < g->p_int) {
			(*point).links[link] = true;
			// 添加一个反向边
			_position p_now(g->d, 0);
			get_position_q(index, g->d, g->m, &p_now);

			long linked_index = get_linked_index(g, &p_now, link);
			(g->$map)[linked_index].links[link + g->d] = true;

			return 1;
		}
		else
			return 0;
	}

	Template
	inline long get_linked_indexxx(GRAPH *g, _position *p, int link) {
		(*p)[link] = ((*p)[link] + 1) % (g->m);
		return GET_INDEX(g, p);
	}

	Template
	long BFS(GRAPH *g) {
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

	Template
	long BFS_point(GRAPH *g, long index, long $color) {
		/* ================================
		* 功能：从一个点开始，进行BFS，计算绕圈数，存入warpping之中
		* 参数：图指针，点编号，颜色
		* 返回：簇规模
		* ================================
		*/
		deque<g_size_t> go_lists;

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
		while (! go_lists.empty()) {
			// 标记、染色：

			g->$map[*go_lists.begin()].color = $color;

			visit_points3(g, &go_lists, $color, &reported_a_wrappingQ);
		}
		
		return size;
	}

	void visit_points3(GRAPH *g, deque<g_size_t> *go_visit, long $color, bool *reported_a_wrappingQ)
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

	Template
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

	Template
	inline long linked_index2(GRAPH *g, long index, int bond_id) {
		long exp_1, exp_2, low_part, change_part, high_part;
		int bond_id_2;

		
		if (bond_id < g->d){
		bond_id_2 = g->d - bond_id - 1;
		} else{
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

	Template
	long get_index(GRAPH *g, _position *p) {
		long result = 0;
		for (int i = 0; i < (*p).size(); ++i) {
			result += (*p)[i] * POWER(g->m, g->d - i - 1);
		}

		return result;
	}

	Template
	long get_index2x(GRAPH *g, _position *p) {
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
}

#endif

#endif //PERCOLATION_RANDOMGIRDGRAPH_H
