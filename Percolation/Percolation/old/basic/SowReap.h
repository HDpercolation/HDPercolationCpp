/*
 * Copyright 2016 Zhang Yanbo, USTC
 * Created by Zhang Yanbo on 2016/3/16.
 * SowReap:==================================
 * 用途：
 *      用以进行全局的散布、收割工作，其原理来自Mathematica中的Sow[], Reap[]函数，
 *      在任意处使用Sow(a)之后，可以使用Reap()获取之前所散布的数据。Clear()用以清除数据。
 *
 * 示例：
 *      ...<code>...
 *      ......
 *      Sow({1, 0, 2, 3});
 *      .....
 *      Sow({2, 3, 5, 6});
 *      .....
 *      d = Reap();
 *      此时，d = {{1, 0, 2, 3}, {2, 3, 5, 6}}
 *      注意此时若再使用Reap,只能得到{}，因为数据已被清除。
 * 测试：
 *      2016/03/16 18:44 已测试
 */

#ifndef PERCOLATION_SOWREAP_H
#define PERCOLATION_SOWREAP_H

 //使用Sow()功能，请开启下面所有的#define
#define SOW_GET_01		//SowReap::_datas data_sow
#define SOW_GET_02		//data_sow = SowReap::Reap()
#define SOW_PRINT_01	//for (int i = 0; i < data_sow.size(); ++i) 
#define SOW_PRINT_02	//for (int j = 0; j < data_sow[i].size() - 1; ++j) { cout << data_sow[i][j] << ",";}
#define SOW_PRINT_03	//cout << data_sow[i][data_sow[i].size() - 1] << endl;
#define SOW_DATA		//SowReap::Sow(total_move)

#include <vector>

namespace SowReap{
    typedef std::vector<std::vector<long>> _datas;
        // 用以存储数据

    _datas data;

    inline _datas Reap(void);
        // 获取数据，并清除

    inline void Sow(std::vector<long>);
        // 存储

    inline void Clear(void);
        // 清除

    inline _datas Reap(void){
        _datas zerodata = {};
        std::swap(zerodata, SowReap::data);

        return zerodata;
    }

    inline void Sow(std::vector<long> $input){
        SowReap::data.push_back($input);

        //std::cout << "Sow::Get a data." << std::endl;
        return;
    }

    inline void Clear(void){
        SowReap::data.clear();

        return;
    }
}

#endif //PERCOLATION_SOWREAP_H
