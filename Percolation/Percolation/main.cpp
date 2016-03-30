/* ========================================================
 * :::::::::::::::::::: Percolation :::::::::::::::::::::::
 * ========================================================
 * 命令行格式：==============================================
 * 用以探测缠绕多圈的情况
 *      类型标识：
 *          * AP    在一个P概率附近，测试绕多圈的概率（平均+加权）
 *      格式：
 *          * >> Percolation.exe AP d m p n (seed)
 *          其中的(seed)，表示随机种子，若提供，则使用seed，
 *          否则使用内部种子（时间）。
 *      例子：
 *          >> Percolation.exe AP 2 10 0.5 10000 23342
 * ========================================================
 */

#include <iostream>
#include "basic/randomGirdGraph.h"
#include "basic/SowReap.h"


using namespace std;
using namespace BondPercolation;


void get_colors(const int d, const int m, double p){
    //GRAPH g;
	WrappingCount::assignCounter(d, m);

	_node_x		O_node(d);
	_Graph_x	g(O_node, d, m, p);

    makeMap(&g, d, m, p);

    BFS(&g);

    long k = 0;
    for (int i = 0; i < g.size; ++i) {
        k++;
        if (k % 100 == 0 || k == g.size){
            cout << g.$map[i].color << endl;
        } else{
			cout << g.$map[i].color << ",";
        }
    }

    return;
}

void warpping_data(int d, int m, double p){
	_node_x		O_node(d);
	_Graph_x	g(O_node, d, m, p);

	WrappingCount::assignCounter(d, m);
	ColorCount::assignColors();

    makeMap(&g, d, m, p);

    BFS(&g);

	/*参见SowReap.h*/
    SOW_GET_01;
    SOW_GET_02;

	SOW_PRINT_01{
		SOW_PRINT_02
		SOW_PRINT_03 
	}

	WrappingCount::counters data;
	ColorCount::color_list_t colorData;

	data = WrappingCount::getCounter();
	colorData = ColorCount::getColor();

	for (int i = 0; i < data.size() - 1; ++i)
	{
		cout << data[i] << ",";
	}
	cout << data[data.size() - 1] << endl;

	for (int j = 0; j < colorData.size() - 1; ++j)
	{
		cout << colorData[j] << ",";
	}
	cout << colorData[colorData.size() - 1] << endl;

    return;
}

void get_AP(int d, int m, double p, long n){
	_node_x		O_node(d);
	_Graph_x	g(O_node, d, m, p);

	WrappingCount::assignCounter(d, m);
	ColorCount::assignColors();

	WrappingCount::counters r_n;
	//ColorCount::color_list_t colorData;

	makeMap(&g, d, m, p);
	BFS(&g);

	r_n = WrappingCount::getCounter();


	for (long i = 0; i < n - 1; ++i) {
		reMakeMap(&g, d, m, p);
		BFS(&g);
	}
	

	//r_n = WrappingCount::getCounter();
	cout << "Have wrapping:," << WrappingCount::one << endl;
	cout << "Have higher wrapping:," << WrappingCount::more_than_one << endl;
	cout << "Have higher wrapping(weighted):," << WrappingCount::more_than_one_total << endl;

    return;
}

int main(int argc, char *argv[]) {

    int			d;
    int			m;
    long		n, seed;
    double		p;
    string		types;
	auto		seed_bool = false;

	//用以性能测试的部分：
	//get_AP(2, 20, 0.5, 5000);
	//cin >> d;
	//return 0;

    if (argc < 6){
        cout << "Format error." << endl;
        //return -1;
		return 0;
    } else{
        types = argv[1];
            // 读取控制符

        if (types == "AP"){
            if (argc < 6){
                cout << "Format error." << endl;
                return 0;
            }
            d = atoi(argv[2]);
            m = atoi(argv[3]);
            p = atof(argv[4]);
            n = atoi(argv[5]);
            if (argc == 7){
                seed_bool = true;
                seed = atol(argv[6]);
				random_MTE.seed(seed);
				randomer.setSeed(seed);
            }
			get_AP(d, m, p, n);
        }
    }

    return 0;
}