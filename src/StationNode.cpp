//
// Created by 加菲汪 on 2022/2/23.
//

#include "../include/StationNode.h"
StationNode::StationNode() {}

StationNode::StationNode(Station *val) : val(val), pre(nullptr), next(nullptr) {}

StationNode::StationNode(Station *val, StationNode *pre, StationNode *next) {}

StationNode::~StationNode() {
}

Station *StationNode::getVal() const {
    return val;
}

void StationNode::setVal(Station *val) {
    StationNode::val = val;
}

StationNode *StationNode::getPre() const {
    return pre;
}

void StationNode::setPre(StationNode *pre) {
    StationNode::pre = pre;
}

StationNode *StationNode::getNext() const {
    return next;
}

void StationNode::setNext(StationNode *next) {
    StationNode::next = next;
}

std::ostream &operator<<(std::ostream &os, StationNode const &stationNode) {
    os << "node(";
    if (!stationNode.val) {
        os << "station=null";
    } else {
        os << "station=" << *stationNode.val;
    }
    os << ")";
    return os;
}

void StationNode::iterNode(StationNode *node, int &len) {
    if(!node){
        return;
    }
    len += 1;
    iterNode(node->getNext(),len);
}

double StationNode::getLoss(StationNode *node1, StationNode *node2){
    if(node1&&node2) {
        return node1->getVal()->calculateLoss(*node2->getVal());
    }
    return 0;
}

/**
 * 遍历node，计算总loss
 * @param node
 * @param total_loss
 */
void StationNode::iterNode(StationNode* node, double &total_loss){
    if(!node){
        return;
    }
    total_loss += StationNode::getLoss(node,node->getNext());
    iterNode(node->getNext(), total_loss);
}

/**
 * 遍历node，计算下游节点总数和总loss
 * @param node
 * @param len
 * @param total_loss
 */
void StationNode::iterNode(StationNode *node, int &len, double &total_loss) {
    if(!node) {
        return;
    }
    len += 1;
    total_loss += StationNode::getLoss(node,node->getNext());
    iterNode(node->getNext(),len,total_loss);
}

/**
 * 遍历node，将node的worker置为worker
 * @param node
 * @param worker
 */
void StationNode::iterNode(StationNode *node, Worker *worker) {
    if(!node) {
        return;
    }
    node->getVal()->setWorker(worker);
    iterNode(node->getNext(),worker);
}

void StationNode::show(StationNode *node) {
    StationNode *temp = node;
    while (temp) {
        std::cout<<*temp;
        if(temp->getNext()){
            std::cout<<" --> ";
        }else{
            std::cout<<"\n";
        }
        temp = temp->getNext();
    };
}



