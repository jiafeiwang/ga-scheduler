//
// Created by 加菲汪 on 2023/2/3.
//

#ifndef GA_SCHEDULER_CHROMOSOME_H
#define GA_SCHEDULER_CHROMOSOME_H

#include "gene.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/**
 * 染色体
 */
struct Chromosome {
    int slotNum; // 时间槽个数
    int workerNum; // worker数
    int length; // 染色体长度
    std::vector<Gene> genes; // 基因

    bool isInitial= false; // 初始化tag
    int taskNum; // 染色体分配task数
    std::unordered_map<u32, u32> taskMp; // taskID~workerID map
    double fitnessVal; //适应度值

    Chromosome();

    Chromosome(int slotNum, int workerNum);

    /**
     * 对染色体进行初始化
     * @param tasks task数据集
     * @param workers worker数据集
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     */
    void InitialChromosome(const std::vector<Task> &tasks, const std::vector<Worker> &workers, int maxCkTimes= 1e6);

    virtual ~Chromosome();

    /**
     * 染色体解码
     * @return
     */
    std::vector<GeneExpr> decode();

    /**
     * 染色体编码
     * @param geneExprs 基因表现数据集
     * @param slotNum
     * @param workerNum
     * @return
     */
    static Chromosome encode(const std::vector<GeneExpr> & geneExprs, int &slotNum, int &workerNum);

//    isFree、isProper、canCatch用于约束条件检验

    /**
     * 判断染色体对应worker 从slotStart起，required长度的时间槽是否空闲（未被分配task）
     * @param worker
     * @param slotStart
     * @param required
     * @return
     */
    bool isFree(int worker, int slotStart, int required);

    /**
     * 判断染色体对应worker 从slotStart起，required长度的时间槽是否可以作为一个整体被移植
     * @param worker
     * @param slotStart
     * @param required
     * @return
     */
    bool isProper(int worker, int slotStart, int required);

    /**
     * 判断task被分配到worker之后，前后task是否会迟到
     * @param worker
     * @param task
     * @param tasks
     * @return
     */
    bool canCatch(int worker, Task task, const std::vector<Task> &tasks);

    /**
     * targetTasks 中的task是否都能赶上
     * @param worker
     * @param targetTasks
     * @param tasks
     * @return
     */
    bool canCatchAll(int worker, std::unordered_set<u32> targetTasks, const std::vector<Task> &tasks);

    /**
     * 找出worker片段中slotStart~slotStart+required时间槽上task
     * @param worker
     * @param slotStart
     * @param required
     * @return
     */
    std::unordered_set<u32> getTargetTasks(int worker, u32 slotStart, u32 required);

    /**
     * 随机选取slotStart~slotStart+required时间槽上空闲、且满足约束条件的worker
     * @param task
     * @param slotStart
     * @param required
     * @param tasks
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     * @return
     */
    u32 randChooseFreeWid(Task task, u32 slotStart, u32 required, const std::vector<Task> &tasks, int maxCkTimes= 1e6);

    /**
     * 随机选取slotStart~slotStart+required时间槽上可以作为整体、交换前后均满足约束条件的worker
     * @param task
     * @param slotStart
     * @param required
     * @param tasks
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     * @return
     */
    u32 randChoosePropWid(Task task, u32 slotStart, u32 required, const std::vector<Task> &tasks, int maxCkTimes= 1e6);

    /**
     * 遗传操作
     * @param chromosome
     * @param tasks
     * @param workers
     * @param mutateTimes
     * @param maxCkTimes 基因操作在满足约束条件下随机尝试的最高次数，默认1e6
     * @return
     */
    static Chromosome mutate(Chromosome chromosome, const std::vector<Task> &tasks,
                             const std::vector<Worker> &workers, int mutateTimes, int propFreq, int maxCkTimes=1e6);

    /**
     * 单次遗传操作
     * @param chromosome
     * @param taskId
     * @param targetWid
     * @param startSlot
     * @param endSlot
     */
    static void mutateSingle(Chromosome &chromosome, u32 taskId, u32 targetWid, int startSlot, int endSlot);

//    static std::pair<Chromosome,Chromosome> cross(Chromosome chromosome1, Chromosome chromosome2,
//                                                  const std::vector<Task> &tasks, const std::vector<Worker> &workers,
//                                                  int crossTimes=100);

    /**
     * 计算单个worker片段fitness
     * @param worker
     * @param tasks
     * @return
     */
    double workerFitness(Worker worker, const std::vector<Task> &tasks);

    /**
     * 计算整条染色体的fitness
     * @param tasks
     * @param workers
     */
    void fitness(const std::vector<Task> &tasks, const std::vector<Worker> &workers);

    /**
     * 打印染色体信息
     * @param tasks
     * @param workers
     */
    void display(const std::vector<Task> &tasks, const std::vector<Worker> &workers);

    /**
     * 检查染色体是否满足约束条件
     * @param tasks
     * @param workers
     */
    void checkAlive(const std::vector<Task> &tasks, const std::vector<Worker> &workers);
};

inline u32 randomChoice(int num){
    return rand()%num+1;
}

#endif //GA_SCHEDULER_CHROMOSOME_H
