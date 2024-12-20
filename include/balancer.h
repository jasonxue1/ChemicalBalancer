// balancer.h

#ifndef BALANCER_H
#define BALANCER_H

#include <string>

// 配平器类
class Balancer {
public:
    // 配平方程式
    // 参数：
    //   equation - 输入的化学方程式
    //   balancedEquation - 输出的配平后方程式
    // 返回值：
    //   成功返回true，失败返回false
    bool balanceEquation(const std::string& equation, std::string& balancedEquation);
};

#endif // BALANCER_H
