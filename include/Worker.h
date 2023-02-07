//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef GA_SCHEDULER_WORKER_H
#define GA_SCHEDULER_WORKER_H

#include "Position.hpp"

/**
 * worker类，用于封装worker信息
 */

class Worker {
private:
    int id; //workerID
    double score; //worker分数
    Position<int, int> position;

public:
    Worker();

    Worker(int id, double score, Position<int, int> position);

    virtual ~Worker();

    int getId() const;

    void setId(int id);

    double getScore() const;

    void setScore(double score);

    Position<int, int> getPosition() const;

    void setPosition(Position<int, int> position);

    friend std::ostream &operator<<(std::ostream &os, Worker const &worker);

};

#endif //GA_SCHEDULER_WORKER_H
