//
// Created by 加菲汪 on 2023/2/4.
//

#ifndef GA_SCHEDULER_GA_H
#define GA_SCHEDULER_GA_H


#include <vector>
#include "Task.h"
#include "Worker.h"
#include "Chromosome.h"

/**
 * initialChromosome->gaIter
 */

class Ga {
private:
    int heldChromosomeNum; //每轮保留的染色体数量
    int slotNum; //时间槽数
    int workerNum; //worker数

    bool isInitial=false; //初始化tag

    std::vector<Chromosome> chromosomes; //种群
    std::vector<Task> tasks; //task数据集
    std::vector<Worker> workers; //worker数据集

public:

    Ga(int heldChromosomeNum, int slotNum, int workerNum,
            const std::vector<Task> &tasks, const std::vector<Worker> &workers);

    virtual ~Ga();

    int getHeldChromosomeNum() const;

    void setHeldChromosomeNum(int heldChromosomeNum);

    int getSlotNum() const;

    void setSlotNum(int slotNum);

    int getWorkerNum() const;

    void setWorkerNum(int workerNum);

    bool getIsInitial() const;

    void setIsInitial(bool isInitial);

    const std::vector<Chromosome> &getChromosomes() const;

    void setChromosomes(const std::vector<Chromosome> &chromosomes);

    const std::vector<Task> &getTasks() const;

    void setTasks(const std::vector<Task> &tasks);

    const std::vector<Worker> &getWorkers() const;

    void setWorkers(const std::vector<Worker> &workers);

    /**
     * 初始化种群
     * @param maxCkTimes
     */
    void initialChromosome(int maxCkTimes= 1e6);

    /**
     * 在种群中随机选取个体chromosome进行遗传操作，生成新的后代
     * @param mutateTimes 随机抽取个体的次数
     * @param taskMtTimes 被选中个体进行基因操作的次数
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     */
    void mutate(int mutateTimes, int taskMtTimes, int propFreq, int maxCkTimes= 1e6);

//    void cross(int crossTimes=100, int taskCsTimes=100);
    /**
     * 按个体fitness由高到低排序，每轮只保留heldChromosomeNum数量的个体
     */
    void select();

    /**
     * 按 mutate->select顺序进行迭代
     * @param iterNum 迭代次数
     * @param mutateTimes 每轮迭代从保留的个体中随机抽取个体的次数
     * @param taskMtTimes 被选中个体进行基因操作的次数
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     */
    void gaIter(int iterNum, int mutateTimes, int taskMtTimes,
//            int crossTimes=100, int taskCsTimes=100,
                int propFreq, int maxCkTimes= 1e6);
    /**
     * 检查保留的染色体是否满足边界条件
     * @param tasks
     * @param workers
     */

    void check(const std::vector<Task> &tasks, const std::vector<Worker> &workers);
};


#endif //GA_SCHEDULER_GA_H
