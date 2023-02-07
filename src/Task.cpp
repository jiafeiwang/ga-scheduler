//
// Created by 加菲汪 on 2022/2/23.
//

#include "../include/Task.h"

Task::Task(){}

Task::Task(int id, Position<int, int> position, int startSlot, int required) : id(id),
                                                                              position(position),
                                                                              startSlot(startSlot),
                                                                              required(required) {}

Task::~Task() {
}

int Task::getId() const {
    return id;
}

void Task::setId(int id) {
    Task::id = id;
}

Position<int, int> Task::getPosition() const {
    return position;
}

void Task::setPosition(Position<int, int> position) {
    Task::position = position;
}

int Task::getStartSlot() const {
    return startSlot;
}

void Task::setStartSlot(int startSlot) {
    Task::startSlot = startSlot;
}

int Task::getRequired() const {
    return required;
}

void Task::setRequired(int required) {
    Task::required = required;
}

std::ostream &operator<<(std::ostream &os, Task const &task){
    os<<"task(id="<<task.id;
    os<<",position="<<task.position;
    os<<",startSlot="<<task.startSlot;
    os<<",required="<<task.required;
    os<<")";
    return os;
}
