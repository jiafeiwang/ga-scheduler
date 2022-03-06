#include <iostream>
#include <list>
#include <vector>
#include <time.h>

#include "include/Worker.h"
#include "include/StationNode.h"
#include "include/io_utils.h"
#include "include/GA.h"

using namespace std;


int main(){
    cout<<"begin..."<<endl;

    vector<Station*> *vs = getStationsFromFile("./data/stationInfo.csv");
    vector<Worker*> *vw = getWorkersFromFile("./data/workerInfo.csv");

    cout<<"vs size: "<<vs->size()<<endl;
    cout<<"vw size: "<<vw->size()<<endl;

    vector<StationNode*> *nodeVec = new vector<StationNode*>();
    for(Station* s: *vs){
        nodeVec->push_back(new StationNode(s));
    }


    GA *ga = new GA(nodeVec,vw);

    clock_t start, end;
    start = clock();

    ga->getOriginalGroup();

    ga->optimize(2);

    end = clock();
    double duration = (double)(end -start);
    cout<<"after optimize, using: "<<duration<<" ms"<<endl;

    cout<<"--------------------"<<endl;
    cout<<"nodeVec size: "<<nodeVec->size()<<endl;
    vector<StationNode*> *headVec = ga->getHeadNodeVec();

//    for(StationNode* node: *headVec){
//        StationNode::show(node);
//    }

    cout<<"headVec size: "<<headVec->size()<<endl;
    ga->getAvgWorthy(headVec);

    cout<<"nodeVec size: "<<nodeVec->size()<<endl;


    //destruct
    for(StationNode* stationNode: *nodeVec){
        delete stationNode;
    }

    delete nodeVec;

    for(Station* station: *vs){
        delete station;
    }

    delete vs;

    for(Worker* worker: *vw){
        delete worker;
    }

    delete vw;

    cout<<"...finish"<<endl;
    return 0;
}


