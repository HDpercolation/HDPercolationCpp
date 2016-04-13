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
#include "PercolationClass.h"


using namespace std;

void testNTimes(int d, int m, double p, long maxn = 1000)
{
	PercolationMap g(d, m, p);
	int c = 0;
	g.make();

	for (int i = 0; i < maxn; ++i)
	{
		g.reMake(p);
		g.BFS();
		if (g.warppingQ())
		{
			c++;
		}
	}
	cout << c / static_cast<double>(maxn) << endl;

}

int main(int argc, char *argv[]) {

    int			d;
    int			m;
    long		n, seed, ns;
    double		p;
    string		types;
	auto		seed_bool = false;

	//用以性能测试的部分：
	//warpping_data(8, 8, 0.08);
	//return 0;

	// Input
	cin >> d;
	cin >> m;
	cin >> p;
	cin >> ns;

	testNTimes(d, m, p, ns);

	cin >> d;

    return 0;
}