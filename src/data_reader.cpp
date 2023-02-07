//
// Created by 加菲汪 on 2023/2/2.
//

#include "../include/Task.h"
#include "../include/Worker.h"
#include "../include/data_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

Task getTaskFromLine(std::string line) {
    int id, beginTime = 0, required=0, x=0, y=0;
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
            case TID:
                id = str2n<int>(str);
                break;
            case BeginTime:
                beginTime = str2n<int>(str);
                break;
            case Required:
                required = str2n<int>(str);
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
    Task task = Task(id,Position<int,int>(x,y),beginTime,required);
    return task;
}


std::vector<Task> getTasksFromFile(std::string taskFileName) {
    std::ifstream fin(taskFileName,std::ios::in);
    if(!fin){
        std::cout<<"open file failed!"<<std::endl;
        return {};
    }
    std::vector<Task> v = std::vector<Task>();
    std::string str;
    while (fin>>str){
        v.push_back(getTaskFromLine(str));
    }
    return v;
}

Worker getWorkerFromLine(std::string line) {
    int id=0, x=0, y=0;
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
    Worker worker = Worker(id,score,Position<int,int>(x,y));
    return worker;
}


std::vector<Worker> getWorkersFromFile(std::string workerFileName) {
    std::ifstream fin(workerFileName,std::ios::in);
    if(!fin){
        std::cout<<"open file failed!"<<std::endl;
        return {};
    }
    std::vector<Worker> v = std::vector<Worker>();
    std::string str;
    while (fin>>str){
        v.push_back(getWorkerFromLine(str));
    }
    return v;
}