//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef BS_ALLOCATOR_WORKER_H
#define BS_ALLOCATOR_WORKER_H

#include "Position.hpp"

/**
 * 操作员类，用于封装操作员信息
 */
class StationNode; //头文件循环依赖

class Worker {
private:
    int id; //操作员ID
    double score; //操作员分数
    Position<int, int> position;
    bool isInitial = true;
    StationNode *head;

public:
    Worker();

    Worker(int id, double score, Position<int, int> position, bool isInitial);

    virtual ~Worker();

    int getId() const;

    void setId(int id);

    double getScore() const;

    void setScore(double score);

    Position<int, int> getPosition() const;

    void setPosition(Position<int, int> position);

    bool getIsInitial() const;

    void setIsInitial(bool isInitial);

    StationNode *getHead() const;

    void setHead(StationNode *head);

    friend std::ostream &operator<<(std::ostream &os, Worker const &worker);

};

#endif //BS_ALLOCATOR_WORKER_H
