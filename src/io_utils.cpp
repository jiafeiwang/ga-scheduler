//
// Created by 加菲汪 on 2022/2/28.
//
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../include/io_utils.h"

/**
 * 将数值格式的字符串转成数字
 * @tparam T
 * @param s
 * @return
 */
template <typename T>
T str2n(std::string s){
    T t;
    std::stringstream ss;
    ss<<s;
    ss>>t;
    return t;
}

Station *getStationFromLine(std::string line) {
    int id, beginTime, endTime, x, y;
    int le = line.length();
    int begin = 0, end = 0, ct = 0;
    for(char c: line){
        std::string str;
        end ++;
        if(end==le){
            ct++;
            str = line.substr(begin,end-begin);
        }
        if(c==','){
            ct++;
            str = line.substr(begin,end-begin-1);
            begin = end;
        }
        switch (ct){
            case SID:
                id = str2n<int>(str);
                break;
            case SBeginTime:
                beginTime = str2n<int>(str);
                break;
            case SEndTime:
                endTime = beginTime + str2n<int>(str) * 60;
                break;
            case SX:
                x = str2n<int>(str);
                break;
            case SY:
                y = str2n<int>(str);
                break;
            default:
                break;
        }
    }
    Station *station = new Station(id,Position<int,int>(x,y), nullptr,beginTime,endTime);
    return station;
}


std::vector<Station*> *getStationsFromFile(std::string stationFileName) {
    std::ifstream fin(stationFileName,std::ios::in);
    if(!fin){
        std::cout<<"open file failed!"<<std::endl;
        return nullptr;
    }
    std::vector<Station*> *v = new std::vector<Station*>();
    std::string str;
    while (fin>>str){
        v->push_back(getStationFromLine(str));
    }
    return v;
}

Worker *getWorkerFromLine(std::string line) {
    int id, x, y;
    double score;
    int le = line.length();
    int begin = 0, end = 0, ct = 0;
    for(char c: line){
        std::string str;
        end ++;
        if(end==le){
            ct++;
            str = line.substr(begin,end-begin);
        }
        if(c==','){
            ct++;
            str = line.substr(begin,end-begin-1);
            begin = end;
        }
        switch (ct){
            case WID:
                id = str2n<int>(str);
                break;
            case Score:
                score = str2n<double>(str);
                break;
            case WX:
                x = str2n<int>(str);
                break;
            case WY:
                y = str2n<int>(str);
                break;
        }
    }
    Worker *worker = new Worker(id,score,Position<int,int>(x,y),false);
    return worker;
}


std::vector<Worker*> *getWorkersFromFile(std::string workerFileName) {
    std::ifstream fin(workerFileName,std::ios::in);
    if(!fin){
        std::cout<<"open file failed!"<<std::endl;
        return nullptr;
    }
    std::vector<Worker*> *v = new std::vector<Worker*>();
    std::string str;
    while (fin>>str){
        v->push_back(getWorkerFromLine(str));
    }
    return v;
}
