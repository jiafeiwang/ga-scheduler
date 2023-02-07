//
// Created by 加菲汪 on 2023/2/4.
//

#include <algorithm>
#include <iomanip>
#include "../include/Ga.h"

Ga::Ga(int heldChromosomeNum, int slotNum, int workerNum,
       const std::vector<Task> &tasks, const std::vector<Worker> &workers) : heldChromosomeNum(heldChromosomeNum),
                                                                             slotNum(slotNum), workerNum(workerNum),
                                                                             tasks(tasks),
                                                                             workers(workers) {}

Ga::~Ga() {

}

int Ga::getHeldChromosomeNum() const {
    return heldChromosomeNum;
}

void Ga::setHeldChromosomeNum(int heldChromosomeNum) {
    Ga::heldChromosomeNum = heldChromosomeNum;
}

int Ga::getSlotNum() const {
    return slotNum;
}

void Ga::setSlotNum(int slotNum) {
    Ga::slotNum = slotNum;
}

int Ga::getWorkerNum() const {
    return workerNum;
}

void Ga::setWorkerNum(int workerNum) {
    Ga::workerNum = workerNum;
}

bool Ga::getIsInitial() const {
    return isInitial;
}

void Ga::setIsInitial(bool isInitial) {
    Ga::isInitial = isInitial;
}

const std::vector<Chromosome> &Ga::getChromosomes() const {
    return chromosomes;
}

void Ga::setChromosomes(const std::vector<Chromosome> &chromosomes) {
    Ga::chromosomes = chromosomes;
}

const std::vector<Task> &Ga::getTasks() const {
    return tasks;
}

void Ga::setTasks(const std::vector<Task> &tasks) {
    Ga::tasks = tasks;
}

const std::vector<Worker> &Ga::getWorkers() const {
    return workers;
}

void Ga::setWorkers(const std::vector<Worker> &workers) {
    Ga::workers = workers;
}

void Ga::initialChromosome(int maxCkTimes) {
    for(int i=0; i<heldChromosomeNum; i++){
//        初始化
        Chromosome chromosome = Chromosome(slotNum, workerNum);
        chromosome.InitialChromosome(tasks, workers, maxCkTimes);
        chromosomes.push_back(chromosome);
    }
//    更新tag
    isInitial = true;
}

void Ga::mutate(int mutateTimes, int taskMtTimes, int propFreq, int maxCkTimes) {
    for(int i=0; i<mutateTimes; i++){
        int size = chromosomes.size();
        int idx = randomChoice(size)-1;
        chromosomes.push_back(Chromosome::mutate(chromosomes[idx], tasks, workers,taskMtTimes, maxCkTimes, propFreq));
    }
}

//void Ga::cross(int crossTimes, int taskCsTimes) {
//    for(int i=0; i<crossTimes; i++){
//        int size = chromosomes.size();
//        int idx1 = randomChoice(size)-1;
//        int idx2 = (idx1 + randomChoice(size-1))%size;
//
//        std::pair<Chromosome,Chromosome> crossRes = Chromosome::cross(chromosomes[idx1],
//                                                                      chromosomes[idx2],
//                                                                      tasks, workers, taskCsTimes);
//        chromosomes.push_back(crossRes.first);
//        chromosomes.push_back(crossRes.second);
//    }
//}

void Ga::select() {
//    按fitness排序
    std::sort(chromosomes.begin(), chromosomes.end(),[](Chromosome chromosome1, Chromosome chromosome2){
        return chromosome1.fitnessVal>chromosome2.fitnessVal;
    });
//    选取最优
    chromosomes.resize(heldChromosomeNum);
}

void Ga::gaIter(int iterNum, int mutateTimes, int taskMtTimes,
//                int crossTimes, int taskCsTimes,
                int propFreq, int maxCkTimes) {
    // 检查是否完成初始化
    if(!isInitial){
        std::cout<<"chromosomes is not initial!"<<std::endl;
        throw 300;
    }
    std::cout<<std::setprecision(3);
    select();
    std::cout<<"initial fitness: "<<chromosomes[0].fitnessVal<<std::endl;
    for(int i=0; i<iterNum; i++){
//        遗传操作
        mutate(mutateTimes,taskMtTimes,propFreq,maxCkTimes);
//        cross(crossTimes,taskCsTimes);
//        选取
        select();
        std::cout<<"epoch: "<<i+1<<", best fitness: "<<chromosomes[0].fitnessVal<<std::endl;
    }
}

void Ga::check(const std::vector<Task> &tasks, const std::vector<Worker> &workers) {
    for(int i=0; i<heldChromosomeNum; i++){
        std::cout<<"-----------------------------"<<std::endl;
        std::cout<<"held chromosome idx: "<<i<<",total taskNum: "<<chromosomes[i].taskNum<<std::endl;
        chromosomes[i].checkAlive(tasks,workers);
        std::cout<<"-----------------------------"<<std::endl;
    }
}
