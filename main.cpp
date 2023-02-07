#include <iostream>
#include <direct.h>
#include <time.h>
#include <algorithm>

using namespace std;

#include "include/Worker.h"
#include "include/Task.h"
#include "include/data_reader.h"
#include "include/Ga.h"

using namespace std;

int main() {
    cout<<"begin..."<<endl;
    cout<<"\n-----------------------------"<<endl;

    cout<<"runTime Dir: "<<endl;
    cout<<_pgmptr<<endl;

    cout<<"work Dir: "<<endl;
    char buffer[1000];
    char *path = getcwd(buffer, 1000);
    printf(path);
    cout<<"\n-----------------------------"<<endl;

    int seed = 111;
    srand(seed);

//    加载task和worker数据集
//    D:/ClionProjects/ga-scheduler/data/taskInfo.csv
    vector<Task> tasks = getTasksFromFile("./data/taskInfo.csv");
    vector<Worker> workers = getWorkersFromFile("./data/workerInfo.csv");

    cout<<"tasks size: "<<tasks.size()<<endl;
    cout<<"workers size: "<<workers.size()<<endl;

//    cout<<tasks[0]<<endl;
//    cout<<workers[0]<<endl;

    Ga ga = Ga(1,30,1209,tasks,workers);

//    初始化种群
    ga.initialChromosome();

//    迭代
    ga.gaIter(50,10,10000,0);

//    检查最终结果是否存货
    ga.check(tasks,workers);

    cout<<"\n-----------------------------"<<endl;

    cout<<"finish!"<<endl;

    return 0;
}
