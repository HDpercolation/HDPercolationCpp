/* ========================================================
 * :::::::::::::::::::: Percolation :::::::::::::::::::::::
 * ========================================================
 * 命令行格式：==============================================
 * TODO 此部分尚未完成
 *      类型标识：
 *          * RN    返回N次迭代中的环绕次数
 *          * RD    返回一次迭代中的环绕数据
 *          * C     返回染色数据
 *      格式：
 *          * >> Percolation.exe RN d m p n (seed)
 *          * >> Percolation.exe RD d m p (seed)
 *          * >> Percolation.exe C d m p (seed)
 *          其中的(seed)，表示随机种子，若提供，则使用seed，
 *          否则使用内部种子（时间）。
 *      例子：
 *          >> Percolation.exe RN 2 10 0.5 50 23342
 *          >> Percolation.exe RD 7 5 0.12
 *          >> Percolation.exe C 5 6 0.15 3455
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

void run_for_n_times(int d, int m, double p, long n){
	_node_x		O_node(d);
	_Graph_x	g(O_node, d, m, p);

	WrappingCount::assignCounter(d, m);
	ColorCount::assignColors();

	WrappingCount::counters r_n;
	ColorCount::color_list_t colorData;

	makeMap(&g, d, m, p);
	BFS(&g);

	r_n = WrappingCount::getCounter();
	colorData = ColorCount::getColor();

	//输出wrapping信息
	{
		for (int k = 0; k < r_n.size() - 1; ++k)
		{
			cout << r_n[k] << ",";
		}
		cout << r_n[r_n.size() - 1] << endl;
	}

	//输出色彩信息
	{
		for (int j = 0; j < colorData.size() - 1; ++j)
		{
			cout << colorData[j] << ",";
		}
		cout << colorData[colorData.size() - 1] << endl;
	}


	for (long i = 0; i < n - 1; ++i) {
		reMakeMap(&g, d, m, p);
		BFS(&g);

		r_n = WrappingCount::getCounter();
		colorData = ColorCount::getColor();

		for (int k = 0; k < r_n.size() - 1; ++k)
		{
			cout << r_n[k] << ",";
		}
		cout << r_n[r_n.size() - 1] << endl;

		for (int j = 0; j < colorData.size() - 1; ++j)
		{
			cout << colorData[j] << ",";
		}
		cout << colorData[colorData.size() - 1] << endl;
	}

    return;
}

void runRangeForNTimes(double rng_a, double rng_b, double dx, double $d, double $m, long n)
/*rng_a, rng_b为计算的p的范围，dx为步长，$d, $m, p为维度、宽度, n是每个点计算的次数*/
{
	/*构造网格*/

	/*循环计算每个点*/
}

int main(int argc, char *argv[]) {

    int			d;
    int			m;
    long		n, seed;
    double		p;
    string		types;
	auto		seed_bool = false;

	//用以性能测试的部分：
	//warpping_data(8, 8, 0.08);
	//return 0;

    if (argc < 5){
        cout << "Format error." << endl;
        //return -1;
		return 0;
    } else{
        types = argv[1];
            // 读取控制符

        if (types == "RN"){
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
				randomer.setSeed(seed);
            }
            run_for_n_times(d, m, p, n);
        } else
            if (types == "RD"){
                d = atoi(argv[2]);
                m = atoi(argv[3]);
                p = atof(argv[4]);
                if (argc == 6){
                    seed_bool = true;
                    seed = atol(argv[5]);
					randomer.setSeed(seed);
					random_MTE.seed(seed);
                }
                warpping_data(d, m, p);
            } else
                if (types == "C"){
                    d = atoi(argv[2]);
                    m = atoi(argv[3]);
                    p = atof(argv[4]);
                    if (argc == 6){
                        seed_bool = true;
                        seed = atol(argv[5]);
						randomer.setSeed(seed);
                    }
                    get_colors(d, m, p);
                }
    }

    return 0;
}