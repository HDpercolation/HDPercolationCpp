#pragma once
#ifndef TYPES
#define TYPES

#include <vector>
#include "basic/randomGirdGraph-inl.h"
#include "basic/randomGirdGraph.h"

#define POWER			long_pow2

typedef int32_t g_size_t;
typedef int32_t my_long_t;
typedef int16_t my_int_t;
typedef std::vector<g_size_t> _move;
typedef bool _link;

typedef std::vector<_link> _links;
typedef std::vector<int> _position;

struct _node_x {
	// �ڵ�
	_node_x() = default;
	_node_x(int $d) :
		links(2 * $d, false) {};

	long color = 0;
	// Ⱦɫ��0ΪδȾɫ
	bool visited = false;
	// �Ƿ񱻷���
	std::vector<bool> links;
	bool have_link_Q = false;
	// ����
	_move move = {};
	// �ӳ�ʼ�㵽�˵�λ�ƣ�ע�⼰ʱ����
};

struct _Graph_x
{
	_Graph_x() = default;
	_Graph_x(_node_x $node, int $d, int $m, double p) :
		//$map($node, POWER($m, $d)),
		d($d), m($m), size(POWER($m, $d)),
		p_int(static_cast<uint_fast32_t>(p * UINT_FAST32_MAX))
	{
		$map = new _node_x[POWER($m, $d)];
		for (g_size_t i = 0; i < size; ++i)
		{
			$map[i] = $node;
		}
	};
	
	// ���ó�ʼ����1. $map�ĳ��ȣ����������ʼֵ�� 2. ����p_int

	_node_x *$map;
	int d, m;
	uint_fast32_t p_int;
	g_size_t size;
};

#endif