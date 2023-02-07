//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef GA_SCHEDULER_POSITION_HPP
#define GA_SCHEDULER_POSITION_HPP

#include <cmath>
#include <iostream>

/**
 * 封装位置信息，重载了运算符 "-"，用于计算两个position的欧式距离
 * @tparam T1
 * @tparam T2
 */
template <typename T1, typename T2>
class Position {
private:
    T1 X;
    T2 Y;

public:
    Position() {}

    Position(T1 x, T2 y) : X(x), Y(y) {}

//    ~Position() {
//    }

    T1 getX() const {
        return X;
    }

    void setX(T1 x) {
        X = x;
    }

    T2 getY() const {
        return Y;
    }

    void setY(T2 y) {
        Y = y;
    }

    friend std::ostream &operator<<(std::ostream &os, Position const &position){
        os<<"("<<position.getX()<<", "<<position.getY()<<")";
        return os;
    }

    /**
     * 运算符重载，用于计算两个位置的欧式距离
     * @param other
     * @return
     */
    double operator- (Position const &other){
        double sq = pow(X-other.getX(),2)+pow(Y-other.getY(),2);
        return sqrt(sq)/1000;
    }

};


#endif //GA_SCHEDULER_POSITION_HPP
