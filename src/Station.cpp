//
// Created by 加菲汪 on 2022/2/23.
//

#include "../include/Station.h"

Station::Station(){}

Station::Station(int id, Position<int, int> position, int beginTime, int endTime) : id(id),
                                                                                           position(position),
                                                                                           beginTime(beginTime),
                                                                                           endTime(endTime) {}

Station::Station(int id, Position<int, int> position, Worker *worker, int beginTime, int endTime) : id(id), position(position), worker(worker),
                                                beginTime(beginTime), endTime(endTime){}

Station::~Station() {
}

int Station::getId() const {
    return id;
}

void Station::setId(int id) {
    Station::id = id;
}

Position<int, int> Station::getPosition() const {
    return position;
}

void Station::setPosition(Position<int, int> position) {
    Station::position = position;
}

Worker *Station::getWorker() const {
    return worker;
}

void Station::setWorker(Worker *worker) {
    Station::worker = worker;
}

int Station::getBeginTime() const {
    return beginTime;
}

void Station::setBeginTime(int beginTime) {
    Station::beginTime = beginTime;
}

int Station::getEndTime() const {
    return endTime;
}

void Station::setEndTime(int endTime) {
    Station::endTime = endTime;
}

std::ostream &operator<<(std::ostream &os, Station const &station){
    os<<"station(id="<<station.id;
    os<<",position="<<station.position;
    os<<",beginTime="<<station.beginTime;
    os<<",endTime="<<station.endTime;
    os<<")";
    return os;
}

bool Station::canCatch(Station const &other) {
    double distance = Station::position - other.getPosition();
    double duration = distance/1000/Param::SPEED*3600; //路上耗费的时间
    if (Station::endTime + duration <= other.getBeginTime()){
        return true;
    }
    return false;
}

double Station::calculateLoss(Station const &other) {
    double distance = Station::position - other.getPosition();
    double interval = (other.getBeginTime() - Station::endTime)/3600.0;
    double loss  = distance/1000/15*Param::BETA + interval*Param::GAMMA;
    return loss;
};

double Station::calculateLoss(Worker const &other) {
    double distance = Station::position - other.getPosition();
    double loss = distance/1000/15*Param::BETA + Param::GAMMA*Param::I;
    return loss;
};
