//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef GA_SCHEDULER_TASK_H
#define GA_SCHEDULER_TASK_H

#include "Position.hpp"
#include "params.h"

/**
 * 任务类，用于封装任务信息
 */
class Task {
private:
    int id; //task ID
    Position<int, int> position; //task position
    int startSlot; // 开始时间
    int required; // 所需时长 单位0.5h

public:
    Task();

    Task(int id, Position<int, int> position, int startSlot, int required);

    virtual ~Task();

    int getId() const;

    void setId(int id);

    Position<int, int> getPosition() const;

    void setPosition(Position<int, int> position);

    int getStartSlot() const;

    void setStartSlot(int startSlot);

    int getRequired() const;

    void setRequired(int required);

    friend std::ostream &operator<<(std::ostream &os, Task const &task);

};


#endif //GA_SCHEDULER_TASK_H
