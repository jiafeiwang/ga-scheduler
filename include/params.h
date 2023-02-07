//
// Created by 加菲汪 on 2022/3/1.
//

#ifndef GA_SCHEDULER_PARAMS_H
#define GA_SCHEDULER_PARAMS_H

/**
 * 定义了一些系数常量
 */
class Param {
public:
    static constexpr int SPEED = 15; //worker速度
    static constexpr double ALPHA = 1.0; //score系数
    static constexpr double BETA = -0.5; //路程系数
    static constexpr double GAMMA = -0.25; //时间间隔系数
    static constexpr double I = 0.5; //worker初始时间间隔值
};

#endif //GA_SCHEDULER_PARAMS_H
