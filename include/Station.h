//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef BS_ALLOCATOR_STATION_H
#define BS_ALLOCATOR_STATION_H

#include "Position.hpp"
#include "Worker.h"
#include "constant_param.h"

/**
 * 站点类，用于封装站点信息
 */
class Station {
private:
    int id; //站点ID
    Position<int, int> position; //站点位置
    Worker *worker; //操作员
    int beginTime; //开始时间
    int endTime; //结束时间

public:
    Station();

    Station(int id, Position<int, int> position, int beginTime, int endTime);

    Station(int id, Position<int, int> position, Worker *worker, int beginTime, int endTime);

    virtual ~Station();

    int getId() const;

    void setId(int id);

    Position<int, int> getPosition() const;

    void setPosition(Position<int, int> position);

    Worker *getWorker() const;

    void setWorker(Worker *worker);

    int getBeginTime() const;

    void setBeginTime(int beginTime);

    int getEndTime() const;

    void setEndTime(int endTime);

    friend std::ostream &operator<<(std::ostream &os, Station const &station);

    /**
     * 是否能赶上另外一个other节点
     * @param other
     * @return
     */
    bool canCatch(Station const &other);

    /**
     * 计算两个节点间的损失
     * @param other
     * @return
     */
    double calculateLoss(Station const &other);

    double calculateLoss(Worker const &other);

};


#endif //BS_ALLOCATOR_STATION_H
