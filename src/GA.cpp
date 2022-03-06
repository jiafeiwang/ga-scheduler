//
// Created by 加菲汪 on 2022/2/24.
//

#include "../include/GA.h"

GA::GA() {}

GA::~GA() {

}

GA::GA(std::vector<StationNode*> *nodeVec, std::vector<Worker*> *workerVec) : nodeVec(nodeVec), workerVec(workerVec) {}

std::vector<StationNode*> *GA::getNodeVec() const {
    return nodeVec;
}

void GA::setNodeVec(std::vector<StationNode*> *nodeVec) {
    GA::nodeVec = nodeVec;
}


std::vector<Worker *> *GA::getWorkerVec() const {
    return workerVec;
}

void GA::setWorkerVec(std::vector<Worker *> *workerVec) {
    GA::workerVec = workerVec;
}

/**
 * 用于计算worthy的公式
 * @param score
 * @param len
 * @param total_loss
 * @param distance
 * @return
 */
inline double worthyFormula(double score, int len, double total_loss, double distance){
    return score*Param::ALPHA*len + total_loss + distance/1000/Param::SPEED*Param::BETA + Param::GAMMA*Param::I;
}

/**
 * worker->head的总价值
 * @param worker
 * @return
 */
double getWorthy(Worker *worker){
    if(!worker){
        return 0;
    }
    if(!worker->getHead()){
        return 0;
    }
    double distance = worker->getPosition() - worker->getHead()->getVal()->getPosition();
    int len = 0;
    double total_loss = 0;
    StationNode::iterNode(worker->getHead(),len,total_loss);
    double worthy = worthyFormula(worker->getScore(),len,total_loss,distance);
    return worthy;
}


/**
 * 如果将head设置为worker的head，head对应链表的Worthy能达到多少
 * @param worker
 * @param head
 * @return
 */
double getWorthy(Worker *worker, StationNode *head){
    double worthy = 0;
    if(!head){
        return worthy;
    }
    if(!worker){
        return worthy;
    }
    double distance = worker->getPosition() - head->getVal()->getPosition();
    if(!head->getNext()){
        worthy = worker->getScore()*Param::ALPHA  +  distance/1000/Param::SPEED*Param::BETA + Param::GAMMA*Param::I;
    }else{
        int len=0;
        double total_loss = 0;
        StationNode::iterNode(head,len,total_loss);
        worthy = worthyFormula(worker->getScore(),len,total_loss,distance);
    }
    return worthy;
}

/**
 * 计算node及下游节点的worthy之和
 * @param node
 * @return
 */
double getWorthy(StationNode *node){
    if(!node){
        return 0;
    }
    if(!node->getVal()->getWorker()){
        return 0;
    }
    if(!node->getPre()){
        return getWorthy(node->getVal()->getWorker());
    }else{
        double score = node->getPre()->getVal()->getWorker()->getScore();

        double total_loss = StationNode::getLoss(node->getPre(),node);
        int len = 0;
        StationNode::iterNode(node,len,total_loss);

        double worthy = score*Param::ALPHA*len+total_loss;
        return worthy;
    }

}

/**
 * 计算把node2拼接到node1上，node2及下游节点的worthy之和
 * @param node1
 * @param node2
 * @return
 */
double getWorthy(StationNode *node1, StationNode *node2){
    if(!node1){
        return 0;
    }
    if(!node2){
        return 0;
    }
    if(!node1->getVal()->getWorker()){
        return 0;
    }
    double score = node1->getVal()->getWorker()->getScore(); //获取node1worker分数

    double total_loss = StationNode::getLoss(node1,node2); //total_loss初始化为node1与node2间loss
    int len = 0;
    StationNode::iterNode(node2,len,total_loss);

    double worthy = score*Param::ALPHA*len+total_loss;
    return worthy;
}

/**
 * 生成初代种群，worker数量少，为每个worker匹配一个相对较好的node
 * @param workerVec
 */
void GA::getOriginalGroup() {

    for(Worker *worker:*workerVec){
//        double worthy = -INFINITY;
        double worthy = 0;
        if(worker->getHead()){
            continue; //跳过已经分配的worker
        }
        for(StationNode *node:*nodeVec){
            if(node->getVal()->getWorker()){
                continue; //跳过已经分配的node
            }

//            从还没分配的node中，找一个worthy最高的分配给worker
            double tempWorthy = getWorthy(worker,node);
            if(tempWorthy>worthy){
                worker->setHead(node);
                worthy = tempWorthy;
            }
        }
        if(worker->getHead()){
            worker->getHead()->getVal()->setWorker(worker);
        }
    }

}

/**
 * 交换两个worker
 * @param worker1
 * @param worker2
 */
void swapWorker(Worker *worker1, Worker *worker2){
//    交换worker1 worker2 head
    StationNode *temp = worker1->getHead();
    worker1->setHead(worker2->getHead());
    worker2->setHead(temp);
//    刷新head的worker
    StationNode::iterNode(worker1->getHead(),worker1);
    StationNode::iterNode(worker2->getHead(),worker2);
}

/**
 * worker间的操作
 */
void GA::workerOpt() {
    for(Worker *worker1:*workerVec){
        for(Worker *worker2:*workerVec){
            if(worker2==worker1){
                continue; // 跳过worker2 == worker1
            }
            if((!worker1->getHead())&&(!worker2->getHead())){
                continue; //worker1 和 worker2都没分配head，跳过
            }
            double currentWorthy = getWorthy(worker1) + getWorthy(worker2);
            double swapWorthy = getWorthy(worker1, worker2->getHead()) + getWorthy(worker2, worker1->getHead());
            if(swapWorthy>currentWorthy){
                swapWorker(worker1,worker2);
            }
        }
    }
}

/**
 * stationNode和worker间操作，将stationNode与worker进行拼接
 */
void GA::nodeWorkerOpt(){
    for(StationNode *node:*nodeVec){
        for(Worker *worker:*workerVec){
            double currWorthy = getWorthy(node) + getWorthy(worker);
            double cutWorthy = getWorthy(worker,node);
            if(cutWorthy>currWorthy){
                if(node->getPre()){
                    //切断StationNode链
                    node->getPre()->setNext(nullptr);
                    node->setPre(nullptr);
                }else{
                    if(node->getVal()->getWorker()){
                        node->getVal()->getWorker()->setHead(nullptr);
                    }
                }
                StationNode::iterNode(node,worker);
                if(worker->getHead()){
                    StationNode::iterNode(worker->getHead(), nullptr);
                }
                worker->setHead(node);
            }
        }
    }
}

/**
 * 判断node1，node2是否已经在同一条链上，且node2在node1之后
 * @param node1
 * @param node2
 * @return
 */
bool isInChain(StationNode *node1, StationNode *node2){
    StationNode *temp = node1;
    while(temp){
        if(temp==node2){
            return true;
        }
        temp = temp->getNext();
    }
    return false;
}

bool canCatch(StationNode *node1, StationNode *node2){
    if(node1&&node2) {
        return node1->getVal()->canCatch(*node2->getVal());
    }
    return true;
}

/**
 * 链表的拼接算子，前提是node1无next节点且node2无pre节点
 * @param node1
 * @param node2
 */
void spliceOperator(StationNode *node1, StationNode *node2, LackWorkerType lackWorkerType) {
    //拼接
    if(node1->getNext()){
        node1->getNext()->setPre(nullptr);
        StationNode::iterNode(node1->getNext(), nullptr);
    }
    if(node2->getPre()){
        node2->getPre()->setNext(nullptr);
    }
    node1->setNext(node2);
    node2->setPre(node1);

    //重置worker
    switch (lackWorkerType){
        case NotLack:{
            if(!node2->getPre()){
                node2->getVal()->getWorker()->setHead(nullptr);
            }
            StationNode::iterNode(node2,node1->getVal()->getWorker());
            break;
        }
        case PreLack:{
            StationNode *temp = node1;
            while (temp->getPre()){
                temp = node1->getPre();
            }
            temp->getVal()->setWorker(node2->getVal()->getWorker());
            StationNode::iterNode(temp,node2->getVal()->getWorker());
            node2->getVal()->getWorker()->setHead(temp);
            break;
        }
        case BackLack:{
            StationNode::iterNode(node2,node1->getVal()->getWorker());
            break;
        }
        default:{
            break;
        }
    }
}

/**
 * 根据node1，node2 worker缺失的类型，判断将node2拼接在node1之后是否整体worthy会升高
 * @param node1
 * @param node2
 * @param lackWorkerType
 * @return
 */
bool compareSpliceLoss(StationNode *node1, StationNode *node2, LackWorkerType lackWorkerType){
    double currWorthy, spliceWorthy;

    switch(lackWorkerType){
        case NotLack: {
            currWorthy = getWorthy(node1->getNext()) + getWorthy(node2);
            spliceWorthy = getWorthy(node1,node2);
            break;
        }

        case PreLack:{
            currWorthy = getWorthy(node2->getVal()->getWorker());

            StationNode *node1Next = node1->getNext();

            node1->setNext(node2);

            StationNode *temp = node1;
            while (temp->getPre()){
                temp = node1->getPre();
            }
            //找到node1的头节点

            spliceWorthy = getWorthy(node2->getVal()->getWorker(),temp);
            node1->setNext(node1Next); //还原
            break;

        }
        case BackLack: {
            currWorthy = getWorthy(node1->getVal()->getWorker());

            StationNode *temp = node1->getNext();
            node1->setNext(node2);

            spliceWorthy = getWorthy(node1->getVal()->getWorker(), node1);
            node1->setNext(temp); //还原
            break;
        }
        default:
            break;
    }
    return spliceWorthy > currWorthy ? true : false;
}

/**
 * stationNode操作：两个stationNode拼接
 */
void GA::stationNodeOpt() {
    for(StationNode* node1:*nodeVec){
        for(StationNode* node2:*nodeVec){
            if(node1==node2){
                continue; // 跳过相同节点
            }
            if(!(canCatch(node1,node2))){
                continue; // 跳过赶不上的节点
            }
            if(isInChain(node1,node2)){
                continue; //跳过已经和node1在同一条链上的节点
            }
            if((!node1->getVal()->getWorker())&&(!node2->getVal()->getWorker())){
                continue; //跳过node1, node2都没worker的情况，这种情况下没法计算worthy
            }
            /**
             * 拼接操作，分三种情形：
             * 1. 都有worker
             * 2. node1 没有worker
             * 3. node2 没有worker
             */

            if ((node1->getVal()->getWorker()) && (node2->getVal()->getWorker())) {//1. 都有worker
                if (compareSpliceLoss(node1, node2, NotLack)) {
                    spliceOperator(node1, node2, NotLack);
                }
            } else if (!node1->getVal()->getWorker()) {//2. node1没有worker
                if (compareSpliceLoss(node1, node2, PreLack)) {
                    spliceOperator(node1, node2, PreLack);
                }
            } else if (!node2->getVal()->getWorker()) {
                if (compareSpliceLoss(node1, node2, BackLack)) {
                    spliceOperator(node1, node2, BackLack);
                }
            }
        }
    }
}

/**
 * 种群演化
 */
void GA::evolution() {
    GA::workerOpt();
    GA::stationNodeOpt();
    GA::nodeWorkerOpt();
}

/**
 * 多次演化迭代进行优化
 * @param n_iter
 */
void GA::optimize(int n_iter) {
    for(int i=0; i<n_iter; ++i){
        GA::evolution();
    }
}

/**
 * 获取stationNode头节点
 * @return
 */
std::vector<StationNode *> *GA::getHeadNodeVec() {
    std::vector<StationNode *> *v = new std::vector<StationNode *>();
    for(StationNode* node:*nodeVec){
        if(!node->getPre()){
            v->push_back(node);
        }
    }
    return v;
}

/**
 * 计算平均价值
 * @param headVec
 * @return
 */
int GA::getAvgWorthy(std::vector<StationNode *> *headVec) {
    int node_num = 0;
    int worker_num = 0;
    double total_loss = 0;
    double worthy = 0.0;
    for(StationNode* head:*headVec){
        StationNode::iterNode(head,node_num,total_loss);
        if(head->getVal()->getWorker()){
            worthy += getWorthy(head->getVal()->getWorker(),head);
            worker_num += 1;
        }
    }
    double avg_loss = total_loss/node_num;
    double avg_worthy = worthy/node_num;
    std::cout<<"node num: "<<node_num<<std::endl;
    std::cout<<"used worker num: "<<worker_num<<std::endl;
    std::cout<<"avg loss of nodes: "<<avg_loss<<std::endl;
    std::cout<<"avg worthy of nodes: "<<avg_worthy<<std::endl;
    return 0;
}