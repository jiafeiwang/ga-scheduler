//
// Created by 加菲汪 on 2023/2/3.
//

#include "../include/gene.h"

// 定义几个位运算操作函数
inline u32 getTaskId(const u32 &gene) {
    u32 taskID = gene<<TASK_REQ_BITS;
    taskID >>= 32-TASK_ID_BITS;
    return taskID;
}

inline u32 getTaskRequired(const u32 &gene) {
    u32 taskRequired = gene>>32-TASK_REQ_BITS;
    return taskRequired;
}

inline u32 getWorkId(const u32 &gene) {
    u32 workId = gene<<TASK_BITS;
    workId>>=32-WORKER_BITS;
    return workId;
}

inline u32 getSlotId(const u32 &gene) {
    u32 SlotId = gene<< 32 - SLOT_BITS;
    SlotId>>= 32 - SLOT_BITS;
    return SlotId;
}

u32 Gene::taskID() {
    return getTaskId(val);
}

u32 Gene::taskRequired() {
    return getTaskRequired(val);
}

u32 Gene::workId() {
    return getWorkId(val);
}

u32 Gene::slotId() {
    return getSlotId(val);
}

Gene::Gene(u32 val) : val(val) {}

Gene::Gene() {}

GeneExpr Gene::geneDecode() {
    u32 slotID = getSlotId(val), workID = getWorkId(val), taskId = getTaskId(val), taskRequired = getTaskRequired(val);
    return {slotID, workID, taskId, taskRequired};
}

bool Gene::isWhite() {
    return val>>IDX_BITS==0;
}

void operator>(Gene &gene, Gene &whiteGene){
    if(!whiteGene.isWhite()){
        std::cout<<"gene "<<whiteGene.val<<" is not white"<<std::endl;
        throw 100;
    }

//    获取task信息
    u32 taskInfo = gene.val>>IDX_BITS<<IDX_BITS;

//    将task信息移入到whiteGene上
    whiteGene.val|=taskInfo;
    gene.val<<=TASK_BITS;
    gene.val>>=TASK_BITS;
}

void operator*(Gene &gene1, Gene &gene2){
    u32 taskInfo_1 = gene1.val>>IDX_BITS<<IDX_BITS;
    u32 taskInfo_2 = gene2.val>>IDX_BITS<<IDX_BITS;

//    交换task信息
    gene1.val<<= TASK_BITS;
    gene1.val>>= TASK_BITS;

    gene2.val<<= TASK_BITS;
    gene2.val>>= TASK_BITS;

    gene1.val|=taskInfo_2;
    gene2.val|=taskInfo_1;
}

Gene Gene::geneEncode(u32 slotID, u32 workerID, u32 taskID, u32 taskRequired) {
    workerID = workerID<<SLOT_BITS;
    taskID = taskID<<SLOT_BITS+WORKER_BITS;
    taskRequired = taskRequired<< SLOT_BITS + WORKER_BITS + TASK_ID_BITS;
    return slotID|workerID|taskID|taskRequired;
}

Gene GeneExpr::geneEncode() {
    return Gene::geneEncode(slotID,workerID,taskID,taskRequired);
}
