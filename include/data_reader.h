//
// Created by 加菲汪 on 2023/2/2.
//

#include "Task.h"
#include "Worker.h"
#include <vector>

#ifndef GA_SCHEDULER_DATA_READER_H
#define GA_SCHEDULER_DATA_READER_H

/**
 * 定义了从data目录下的两个csv文件中读取数据的方法
 */

//Task 源数据字段枚举
enum TaskCol{
    TID=1, //Task ID
    BeginTime, //任务开始时间
    Required, //任务结束时间，实际数据表下是任务的持续时间
    SX, //X坐标
    SY, //Y坐标
};

//Worker字段枚举
enum WorkerCol{
    WID=1, //ID
    Score, //分数
    WX, //X坐标
    WY, //Y坐标
};

/**
 * 从每一行的string中读取Task信息
 * @param line
 * @return
 */
Task getTaskFromLine(std::string line);

/**
 * 从文件取数
 * @param TaskFileName
 * @return
 */
std::vector<Task> getTasksFromFile(std::string TaskFileName);

/**
 * 从每一行的string中读取worker信息
 * @param line
 * @return
 */
Worker getWorkerFromLine(std::string line);

/**
 * 从文件取数
 * @param TaskFileName
 * @return
 */
std::vector<Worker> getWorkersFromFile(std::string TaskFileName);



#endif //GA_SCHEDULER_DATA_READER_H
