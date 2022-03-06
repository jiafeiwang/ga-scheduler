//
// Created by 加菲汪 on 2022/2/23.
//

#ifndef BS_ALLOCATOR_STATIONNODE_H
#define BS_ALLOCATOR_STATIONNODE_H

#include "./Station.h"
/**
 * 站点链表节点
 */
class StationNode {
private:
    Station *val;
    StationNode *pre;
    StationNode *next;

public:
    StationNode();

    StationNode(Station *val);

    StationNode(Station *val, StationNode *pre, StationNode *next);

    virtual ~StationNode();

    Station *getVal() const;

    void setVal(Station *val);

    StationNode *getPre() const;

    void setPre(StationNode *pre);

    StationNode *getNext() const;

    void setNext(StationNode *next);

    friend std::ostream &operator<<(std::ostream &os, StationNode const &stationNode);

    static void iterNode(StationNode *node, int &len);

    /**
     * 计算两个节点间的loss
     * @param node1
     * @param node2
     * @return
     */
    static double getLoss(StationNode *node1, StationNode *node2);

    /**
     * 遍历node，计算总loss
     * @param node
     * @param total_loss
     */
    static void iterNode(StationNode* node, double &total_loss);

    /**
     * 遍历node，计算下游节点总数和总loss
     * @param node
     * @param len
     * @param total_loss
     */
    static void iterNode(StationNode* node, int &len, double &total_loss);

    /**
     * 遍历node，将node的worker置为worker
     * @param node
     * @param worker
     */
    static void iterNode(StationNode* node, Worker *worker);

    static void show(StationNode *node);
};


#endif //BS_ALLOCATOR_STATIONNODE_H
