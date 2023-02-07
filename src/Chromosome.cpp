//
// Created by 加菲汪 on 2023/2/3.
//

#include <iostream>
#include <algorithm>
#include <cfloat>
#include <ctime>
#include "../include/Chromosome.h"

Chromosome::Chromosome() {}

Chromosome::Chromosome(int slotNum, int workerNum) : slotNum(slotNum), workerNum(workerNum) {
    if(slotNum>1<<SLOT_BITS){
        std::cout<<"slotNum out of range 0~"<<1<<SLOT_BITS<<std::endl;
        throw 100;
    }
    if(workerNum>1<<WORKER_BITS){
        std::cout<<"workerNum out of range 0~"<<1<<WORKER_BITS<<std::endl;
        throw 100;
    }

    length = slotNum*workerNum;
    genes.resize(length);

    for(int worker=0; worker<workerNum; worker++){
        for(int slot=0; slot<slotNum; slot++){
            genes[worker*slotNum+slot] = Gene::geneEncode(slot,worker,0,0);
        }
    }
}

Chromosome::~Chromosome() {
}

std::vector<GeneExpr> Chromosome::decode() {
    std::vector<GeneExpr> GeneExprs;
    for(int i=0; i<length; i++){
        GeneExprs.push_back(genes[i].geneDecode());
    }
    return GeneExprs;
}

Chromosome Chromosome::encode(const std::vector<GeneExpr> & geneExprs, int &slotNum, int &workerNum) {
    if(geneExprs.size()!=slotNum*workerNum){
        std::cout<< "size of geneExprs is not equals to slowNum*workerNum"<<std::endl;
        throw 100;
    }

    int i=0;
    Chromosome chromosome = Chromosome(slotNum, workerNum);
    for(GeneExpr geneExpr:geneExprs){
        chromosome.genes[i] = geneExpr.geneEncode();
        i++;
    }
    return chromosome;
}

bool Chromosome::isFree(int worker, int slotStart, int required){
    int prod = worker*slotNum;
    int slotEnd = slotStart+required-1;
    for(int i=slotStart; i<=slotEnd; i++){
        if(genes[prod+i].taskID()!=0){
            return false;
        }
    }
    return true;
}

bool Chromosome::isProper(int worker, int slotStart, int required) {
    bool res=true;

    int prod = worker*slotNum;
    int slotEnd = slotStart+required-1;

    // 获取当前worker片段目标时间槽前后taskID，没有则置0
    u32 preTask = slotStart > 0 ? genes[prod + slotStart - 1].taskID() : 0;
    u32 nextTask = slotEnd < slotNum ? genes[prod + slotEnd + 1].taskID() : 0;

    // 目标时间槽与前后位置对应taskID连续，返回false
    if(preTask!=0&&preTask==genes[prod + slotStart].taskID()){
        return false;
    }
    if(nextTask!=0&&nextTask==genes[prod+slotEnd].taskID()){
        return false;
    }

    // 不连续说明可以作为整体，返回true
    return true;
}

bool Chromosome::canCatch(int worker, Task task, const std::vector<Task> &tasks) {
    int prod = worker*slotNum;
    int slotStart = task.getStartSlot();
    int required = task.getRequired();

//    task加入worker对应时间槽后，是否会迟到
    bool preRes;
    // 找到前一个task结束位置
    int i = slotStart-1;
    while(i>0&&genes[prod+i].taskID()==0){
        i--;
    }
    if(i<=0){ // 没有前一个返回true
        preRes=true;
    }else{  // 有前一个检查是否能赶上
        Task preTask = tasks[genes[prod+i].taskID()-1];
        double preDistance = task.getPosition() - preTask.getPosition();
        preRes = slotStart-i-1 >= preDistance/Param::SPEED*2;
    }

//    task加入worker对应时间槽后，后一个task是否会迟到
    bool nextRes;
    i = slotStart+required;
    while(i<slotNum&&genes[prod+i].taskID()==0){
        i++;
    }
    if(i==slotNum){
        nextRes = true;
    }else{
        Task nextTask = tasks[genes[prod+i].taskID()-1];
        double nextDistance = task.getPosition() - nextTask.getPosition();
        nextRes = i - slotStart-required >= nextDistance/Param::SPEED*2;
    }

    return preRes&&nextRes;
}

bool Chromosome::canCatchAll(int worker, std::unordered_set<u32> targetTasks, const std::vector<Task> &tasks){
    bool res = true;
    for(u32 task:targetTasks){
        res = res && canCatch(worker, tasks[task-1], tasks);
    }
    return res;
}

u32 Chromosome::randChooseFreeWid(Task task, u32 slotStart, u32 required, const std::vector<Task> &tasks, int maxCkTimes) {
    u32 workerId = randomChoice(workerNum); // 先随机选取一个workerId
    int i=0;

//    定义一个set，用于储存已经被检查过的不满足约束条件的workerId
    std::unordered_set<u32> checkedWorker;

//    约束条件检测
    while(checkedWorker.find(workerId)==checkedWorker.end() || !isFree(workerId-1,
            slotStart, required) || !canCatch(workerId - 1,task, tasks)){
        checkedWorker.insert(workerId);
        workerId = randomChoice(workerNum);
        i++;

//        达到最高次数，如果进行了大量的随机选取都没有合适的，说明很大概率workers中不存在合适的
        if(i == maxCkTimes){
            std::cout<<"reach max random iterations!"<<std::endl;
            if(checkedWorker.size()==workerNum){
                std::cout<<"all workers have been checked and no suitable been found!"<<std::endl;
            }
            throw 200;
        }
    }

    return workerId;
}

std::unordered_set<u32> Chromosome::getTargetTasks(int worker, u32 slotStart, u32 required){
    int prod = worker*slotNum;
    int endSlot = slotStart+required;
    std::unordered_set<u32> targetTasks;
    for(int i=slotStart; i<endSlot; i++){
        u32 taskId = genes[prod+i].taskID();
        if(taskId!=0){
            targetTasks.insert(taskId);
        }
    }
    return targetTasks;
}

u32 Chromosome::randChoosePropWid(Task task, u32 slotStart, u32 required, const std::vector<Task> &tasks, int maxCkTimes) {
    u32 rawWorkerId = taskMp[task.getId()];

    u32 targetWid = randomChoice(workerNum);
//    targetWid的taskId到rawWid中是否会迟到判断
    std::unordered_set<u32> targetTasks = getTargetTasks(targetWid-1,slotStart,required);
    bool catchAll = canCatchAll(rawWorkerId-1,targetTasks,tasks);

    int i=0;
    std::unordered_set<u32> checkedWorker;

    while(checkedWorker.find(targetWid) == checkedWorker.end() ||
    !isProper(targetWid - 1,slotStart, required) ||
    !canCatch(targetWid - 1, task, tasks)||
    !catchAll) {
        checkedWorker.insert(targetWid);

        targetWid = randomChoice(workerNum);
        targetTasks = getTargetTasks(targetWid-1,slotStart,required);
        catchAll = canCatchAll(rawWorkerId-1,targetTasks,tasks);

        i++;
        if(i == maxCkTimes){
            std::cout<<"reach max random iterations!"<<std::endl;
            if(checkedWorker.size()==workerNum){
                std::cout<<"all workers have been checked and no suitable been found!"<<std::endl;
            }
            throw 200;
        }
    }
    return targetWid;
}


void Chromosome::InitialChromosome(const std::vector<Task> &tasks, const std::vector<Worker> &workers, int maxCkTimes) {
    taskNum = tasks.size();
//    tasks 的idx随机打乱，用来随机分配
    std::vector<int> idx;
    for(int i=0; i<taskNum; i++){
        idx.push_back(i);
    }

    std::random_shuffle(idx.begin(),idx.end());

    for(int i=0; i<taskNum; i++){
        Task task = tasks[idx[i]];
        u32 taskId = task.getId();
        u32 startSlot = task.getStartSlot();
        u32 required = task.getRequired();

//        随机选取对应时间槽空闲的worker
        u32 workerId = randChooseFreeWid(task, startSlot, required, tasks, maxCkTimes);

        int endSlot = startSlot+required;
        int prod = (workerId-1)*slotNum;

//        task worker信息编码进对应gene
        for(int slot=startSlot; slot<endSlot; slot++){
            genes[prod+slot] = Gene::geneEncode(slot, workerId, taskId, required);
        }
        taskMp[taskId] = workerId;
    }

//    重置tag和fitnessVal
    isInitial=true;
    fitness(tasks, workers);
}

void Chromosome::mutateSingle(Chromosome &chromosome, u32 taskId, u32 targetWid, int startSlot, int endSlot) {
    u32 rawWid = chromosome.taskMp[taskId];
//    更新task的workerId
    chromosome.taskMp[taskId] = targetWid;
    int slotNum = chromosome.slotNum;

//    交换task操作
    for(int j=startSlot; j<endSlot; j++){
        u32 targetTid = chromosome.genes[(targetWid-1)*slotNum+j].taskID();
        chromosome.genes[(rawWid-1)*slotNum+j]*chromosome.genes[(targetWid-1)*slotNum+j];

//        更新target task的workerID
        if(targetTid!=0){
            chromosome.taskMp[targetTid] = rawWid;
        }
    }

}


Chromosome Chromosome::mutate(Chromosome chromosome, const std::vector<Task> &tasks, const std::vector<Worker> &workers,
        int mutateTimes, int propFreq, int maxCkTimes) {
    if(!chromosome.isInitial){
        std::cout<<"chromosome is not initial!"<<std::endl;
        throw 300;
    }

    int taskNum = chromosome.taskNum;

    for(int i=0; i < mutateTimes; i++){
//        随机选取task
        u32 taskId = randomChoice(taskNum);

        Task task = tasks[taskId-1];
        u32 startSlot = task.getStartSlot();
        u32 required = task.getRequired();
        u32 endSlot = startSlot+required;
        u32 rawWid = chromosome.taskMp[taskId];

//        随机选取目标task
        u32 targetWid;
        // 每propFreq次进行一次randChoosePropWid， Prop开销比Free大
        if(propFreq>0&&i%propFreq==propFreq-1){
            targetWid = chromosome.randChoosePropWid(task, startSlot, required, tasks, maxCkTimes);
        }else{
            targetWid = chromosome.randChooseFreeWid(task, startSlot, required, tasks, maxCkTimes);
        }
//        记录两个worker下操作前的fitness之和
        double fitnessPro = chromosome.workerFitness(workers[rawWid-1],tasks) +
                chromosome.workerFitness(workers[targetWid-1],tasks);

        mutateSingle(chromosome, taskId, targetWid, startSlot, endSlot);

//        记录操作后的fitness之和
        double fitnessAfter = chromosome.workerFitness(workers[rawWid-1],tasks) +
                            chromosome.workerFitness(workers[targetWid-1],tasks);

//        如果反倒变差了就交换回去，等同于此轮不做操作
        if(fitnessAfter<fitnessPro){
            mutateSingle(chromosome, taskId, rawWid, startSlot, endSlot);
        }
    }
//    重置fitness
    chromosome.fitness(tasks,workers);
    return chromosome;
}

//std::pair<Chromosome,Chromosome> Chromosome::cross(Chromosome chromosome1, Chromosome chromosome2,
//        const std::vector<Task> &tasks, const std::vector<Worker> &workers,
//        int crossTimes) {
//    if(!chromosome1.isInitial){
//        std::cout<<"chromosome1 is not initial!"<<std::endl;
//        throw 300;
//    }
//
//    if(!chromosome2.isInitial){
//        std::cout<<"chromosome2 is not initial!"<<std::endl;
//        throw 300;
//    }
//
//    if(chromosome1.taskNum!=chromosome2.taskNum){
//        std::cout<<"task num of chromosome1 and chromosome2 not equal!"<<std::endl;
//        throw 100;
//    }
//
//    if(chromosome1.slotNum!=chromosome2.slotNum){
//        std::cout<<"slot num of chromosome1 and chromosome2 not equal!"<<std::endl;
//        throw 100;
//    }
//
//    int taskNum = chromosome1.taskNum;
//    int slotNum = chromosome1.slotNum;
//
//    for(int i=0; i < crossTimes; i++){
//        u32 taskId = randomChoice(taskNum);
//
//        Task task = tasks[taskId-1];
//        u32 startSlot = task.getStartSlot();
//        u32 required = task.getRequired();
//        u32 endSlot = startSlot+required;
//
//        u32 rawWid_1 = chromosome1.taskMp[taskId];
//        u32 rawWid_2 = chromosome2.taskMp[taskId];
//
//        for(int j=startSlot; j<endSlot; j++){
//            chromosome1.genes[(rawWid_1-1)*slotNum+j]*chromosome2.genes[(rawWid_2-1)*slotNum+j];
//            chromosome1.taskMp[taskId] = rawWid_2;
//            chromosome2.taskMp[taskId] = rawWid_1;
//        }
//
//    }
//    chromosome1.fitness(tasks,workers);
//    chromosome2.fitness(tasks,workers);
//    return{chromosome1,chromosome2};
//}

/**
 * score计算公式
 * @param score
 * @param distance
 * @param interval
 * @return
 */
inline double formula(double score, double distance, double interval){
    return Param::ALPHA*score + Param::BETA*distance/Param::SPEED + Param::GAMMA*interval;
}

double Chromosome::workerFitness(Worker worker, const std::vector<Task> &tasks){
    u32 prod = (worker.getId()-1)*slotNum;
    double score = worker.getScore();

    Position<int,int> workerPosition = worker.getPosition();

    int i=0;
    int lastEndSlot = 0;

//    worker片段上是否为第一个task
    bool isHeadTask = true;
    double res = 0;

    while(i<slotNum){
        u32 taskId = genes[prod+i].taskID();
        if(taskId==0){
            i++;
            continue;
        }

        Task task = tasks[taskId-1];
        double distance = workerPosition - task.getPosition();
        int startSlot = task.getStartSlot();

//        遇到了第一个Task，用默认槽间隔I
        if(isHeadTask){
            res += formula(score, distance, Param::I);
            isHeadTask = false;
        }else{
//            第一个之后的Task，计算槽间隔Interval
            double interval = (startSlot - lastEndSlot)/2;
            res += formula(score, distance, interval);
        }

//        更新workerPosition、lastEndSlot、i
        workerPosition = task.getPosition();
        lastEndSlot = startSlot + task.getRequired();
        i = i + task.getRequired();
    }
    return res;

}

void Chromosome::fitness(const std::vector<Task> &tasks, const std::vector<Worker> &workers) {
    if(!isInitial){
        return;
    }
    double res = 0;
    for(int i=0; i<workerNum; i++){
        Worker worker = workers[i];
        res += workerFitness(worker, tasks);
    }
    fitnessVal =  res/taskNum;
}

void Chromosome::display(const std::vector<Task> &tasks, const std::vector<Worker> &workers) {
    for(int i=0; i<workerNum; i++){
        std::cout<<workers[i]<<"->";
        int j=0;
        while(j<slotNum){
            u32 taskID = genes[i*slotNum+j].taskID();
            if(taskID==0){
                j++;
                continue;
            }
            u32 required = tasks[taskID-1].getRequired();
            std::cout<<"("<<j<<")"<<tasks[taskID-1]<<"->";
            j+=required;
        }
        std::cout<<std::endl;
        std::cout<<"-----------------------------------------------------------"<<std::endl;
    }
}

void Chromosome::checkAlive(const std::vector<Task> &tasks, const std::vector<Worker> &workers) {

    std::unordered_set<u32> task_st;
    for(int i=0; i<workerNum; i++){
        int j=0;
        int endSlot = 0;
        bool isHeadTask=true;
        Task preTask;
        while(j<slotNum){
            u32 taskID = genes[i*slotNum+j].taskID();
            if(taskID==0){
                j++;
                continue;
            }

            Task task = tasks[taskID-1];
            u32 slotStart = task.getStartSlot();

//            检查是否有task被分配多次
            if(task_st.find(taskID)!=task_st.end()){
                std::cout<<"("<<j<<")"<<"("<<taskID<<")"<<task<<" assigned multiple times"<<std::endl;
            }

//            检查相邻task时间槽位置是否有重叠
            if(task.getStartSlot()<endSlot&&endSlot!=0){
                std::cout<<task<<"<="<<endSlot<<std::endl;
            }

            double distance;
            if(isHeadTask){
                distance = 0;
                isHeadTask = false;

            }else{
                distance = task.getPosition() - preTask.getPosition();
            }

//            检查是否能在预约时间前赶上
            if(slotStart-endSlot < distance/Param::SPEED*2){
                std::cout<<task<<" can't catch distance "<<distance<<std::endl;
            }

            u32 required = task.getRequired();
            task_st.insert(taskID);
            endSlot = task.getStartSlot()+required;
            preTask = task;
            j+=required;
        }
    }

//    检查是否有task没被分配
    if(task_st.size()!=tasks.size()){
        std::cout<<"assigned task num: "<<task_st.size()<<std::endl;
        std::cout<<"task num: "<<tasks.size();
    }
}
