//
// Created by 加菲汪 on 2022/2/28.
//
#include <vector>
#include <iostream>
#include <string>
#include "Station.h"


#ifndef BS_ALLOCATOR_OS_UTILS_H
#define BS_ALLOCATOR_OS_UTILS_H

/**
 * 定义了从data目录下的两个csv文件中读取数据的方法
 */

//站点数据字段枚举
enum StationCol{
    SID=1, //Station ID
    SBeginTime, //任务开始时间
    SEndTime, //任务结束时间，实际数据表下是任务的持续时间
    SX, //X坐标
    SY, //Y坐标
};

//worker数据字段枚举
enum WorkerCol{
    WID=1, //ID
    Score, //分数
    WX, //X坐标
    WY, //Y坐标
};

/**
 * 从每一行的string中读取station信息
 * @param line
 * @return
 */
Station* getStationFromLine(std::string line);

/**
 * 从文件取数
 * @param stationFileName
 * @return
 */
std::vector<Station*> *getStationsFromFile(std::string stationFileName);

/**
 * 从每一行的string中读取worker信息
 * @param line
 * @return
 */
Worker* getWorkerFromLine(std::string line);

/**
 * 从文件取数
 * @param stationFileName
 * @return
 */
std::vector<Worker*> *getWorkersFromFile(std::string stationFileName);


#endif //BS_ALLOCATOR_OS_UTILS_H
