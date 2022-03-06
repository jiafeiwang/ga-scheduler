//
// Created by 加菲汪 on 2022/3/1.
//

#ifndef BS_ALLOCATOR_CONSTANT_PARAM_H
#define BS_ALLOCATOR_CONSTANT_PARAM_H

/**
 * 定义了一些系数常量
 */
class Param {
public:
    static const int SPEED = 15; //worker速度
    constexpr static const double ALPHA = 1.0; //score系数
    constexpr static const double BETA = -0.5; //路程系数
    constexpr static double GAMMA = -0.25; //时间间隔系数
    constexpr static double I = 0.5; //worker初始时间间隔值
};

#endif //BS_ALLOCATOR_CONSTANT_PARAM_H
