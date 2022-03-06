//
// Created by 加菲汪 on 2022/2/24.
//

#ifndef BS_ALLOCATOR_GA_H
#define BS_ALLOCATOR_GA_H


#include <vector>
#include <list>
#include "StationNode.h"
#include "constant_param.h"

/**
 * 定义了遗传算法的基本步骤
 */
class GA {

private:
    std::vector<StationNode *> *nodeVec; //StationNode集合
    std::vector<Worker *> *workerVec; //Worker集合

public:
    GA();

    GA(std::vector<StationNode *> *nodeVec, std::vector<Worker*> *workerVec);

    virtual ~GA();

    std::vector<StationNode *> *getNodeVec() const;

    void setNodeVec(std::vector<StationNode *> *nodeVec);

    std::vector<Worker *> *getWorkerVec() const;

    void setWorkerVec(std::vector<Worker *> *workerVec);


    /**
     * 贪心算法生成初代种群
     * @param workerVec
     */
    void getOriginalGroup();

    /**
     * 种群演化
     */
    void evolution();

    /**
     * worker间的操作：交换head
     */
    void workerOpt();

    /**
     * stationNode操作：两个stationNode拼接
     */
    void stationNodeOpt();

    /**
     * stationNode和worker间操作，将stationNode与worker进行拼接
     */
    void nodeWorkerOpt();

    /**
     * 多次演化迭代进行优化
     * @param n_iter
     */
    void optimize(int n_iter);

    /**
     * 获取stationNode头节点
     * @return
     */
    std::vector<StationNode*> *getHeadNodeVec();

    /**
     * 计算平均价值
     * @param headVec
     * @return
     */
    int getAvgWorthy(std::vector<StationNode*> *headVec);

};

/**
 * 定义两个stationNode的worker缺失类型
 */
enum LackWorkerType{
    NotLack, //两个节点都不缺失worker
    PreLack, //前面的节点缺失worker
    BackLack, //后面的节点缺失worker
    BothLack, //两个都缺失worker
};

/**
 * worker->head的总价值
 * @param worker
 * @return
 */
double getWorthy(Worker *worker);

/**
 * 如果将head设置为worker的head，head对应链表的Worthy能达到多少
 * @param worker
 * @param head
 * @return
 */
double getWorthy(Worker *worker, StationNode *head);

/**
 * 计算node及下游节点的worthy之和
 * @param node
 * @return
 */
double getWorthy(StationNode *node);

/**
 * 计算把node2拼接到node1上，node2及下游节点的worthy之和
 * @param node1
 * @param node2
 * @return
 */
double getWorthy(StationNode *node1, StationNode *node2);

/**
 * 交换两个worker
 * @param worker1
 * @param worker2
 */
void swapWorker(Worker *worker1, Worker *worker);

/**
 * 判断node2是否已经连接在node1的下游
 * @param node1
 * @param node2
 * @return
 */
bool isInChain(StationNode *node1, StationNode *node2);

/**
 * 判断做完node1是否还来得及做node2
 * @param node1
 * @param node2
 * @return
 */
bool canCatch(StationNode *node1, StationNode *node2);

/**
 * 根据node1，node2 worker缺失的类型，判断将node2拼接在node1之后是否整体worthy会升高
 * @param node1
 * @param node2
 * @param lackWorkerType
 * @return
 */
bool compareSpliceLoss(StationNode *node1, StationNode *node2, LackWorkerType lackWorkerType);
/**
 * 链表的拼接算子，前提是node1无next节点且node2无pre节点
 * @param node1
 * @param node2
 */
void spliceOperator(StationNode *node1, StationNode *node2, LackWorkerType lackWorkerType);




#endif //BS_ALLOCATOR_GA_H
