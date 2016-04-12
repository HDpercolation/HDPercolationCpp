//
// Created by Zhang Yanbo on 2015/9/8.
//

/*
 * ��ͷ�ļ�������һ��schrageRandInt����������������������д�����
 * ����seed������a, b, m����������q, r�����ṩ��getNum������ȡ�����
 * ͬʱҲ�ṩ���޸Ĳ����ĸ��ַ�����
 */

// TODO 使用整数优化！

#ifndef RANDOMWORK_RANDOMAUTOMATA_H
#define RANDOMWORK_RANDOMAUTOMATA_H

/*使用梅森旋转，开启下面的定义*/

class schrageRandInt{                       //�Զ�����α�������Ĭ��Ϊ16807�����������Ե���
private:
    long seed;
    long a, b, m;
    long q, r;
public:
    schrageRandInt(long seedx = 0, long mx = 2147483647, long ax = 16807, long bx = 0){ // ��ʼ��
        seed = seedx;
        a = ax;
        b = bx;
        m = mx;

        q = m / a;
        r = m % a;
    }
    inline int setSeed(long);                      //�����������
    inline int setABM(long, long, long);          //����a, b, m
    inline long getNum(void);                      //��ȡ��һ�������
    inline double rand(void);                      //Get a double number
    inline void setA(long);                        //����a
};

inline double schrageRandInt::rand(void) {
    return double(double(getNum()) / double(m));
}

inline long schrageRandInt::getNum(void) {
    long u;

    u = a * (seed % q) - r * (seed / q);
    if (u < 0)
        u += m;

    if (u < m - b)
        u += b;
    else
        u += b - m;//��b��ͬʱ��ֹ����ʱ���

    seed = u;
    return u;
}


inline int schrageRandInt::setSeed(long sx) {
    this->seed = sx;
    return 1;
}

inline void schrageRandInt::setA(long ax) {
    a = ax;

    q = m / a;
    r = m % a;
}

inline int schrageRandInt::setABM(long ax, long bx, long mx) {
    a = ax;
    b = bx;
    m = mx;

    q = m / a;
    r = m % a;
}

#endif //RANDOMWORK_RANDOMAUTOMATA_H
