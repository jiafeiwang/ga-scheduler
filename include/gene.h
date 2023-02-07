//
// Created by 加菲汪 on 2023/2/3.
//

#ifndef GA_SCHEDULER_GENE_H
#define GA_SCHEDULER_GENE_H

#include "Task.h"
#include "Worker.h"

typedef unsigned int u32;

struct Gene;
/**
 * 基因表现
 */
struct GeneExpr{
    u32 slotID;
    u32 workerID;
    u32 taskID;
    u32 taskRequired;

    Gene geneEncode();
};

static constexpr int SLOT_BITS = 5; //用于时间槽编码的位数，共5位0~32最大值31
static constexpr int WORKER_BITS = 12; //用于workerId编码的位数
static constexpr int TASK_ID_BITS = 12; //用于taskId编码的位数
static constexpr int TASK_REQ_BITS = 3; //用于task required编码的位数

static constexpr int IDX_BITS = SLOT_BITS+WORKER_BITS;
static constexpr int TASK_BITS = TASK_ID_BITS+TASK_REQ_BITS;

struct Gene{
    u32 val; //值

    Gene();

    Gene(u32 val);

    /**
     * 解码
     * @return
     */
    GeneExpr geneDecode();

    /**
     * 编码
     * @param slotID
     * @param workerID
     * @param taskID
     * @param taskRequired
     * @return
     */
    static Gene geneEncode(u32 slotID, u32 workerID, u32 taskID, u32 taskRequired);

    /**
     * 是否空白基因，即没有匹配Task
     * @return
     */
    bool isWhite();

    u32 taskID();

    u32 taskRequired();

    u32 workId();

    u32 slotId();

    /**
     * 将gene上信息移到空白基因whiteGene上
     * @param gene
     * @param whiteGene
     */
    friend void operator>(Gene &gene, Gene &whiteGene);

    /**
     * 交换gene1和gene2上信息
     * @param gene1
     * @param gene2
     */
    friend void operator*(Gene &gene1, Gene &gene2);
};

#endif //GA_SCHEDULER_GENE_H
