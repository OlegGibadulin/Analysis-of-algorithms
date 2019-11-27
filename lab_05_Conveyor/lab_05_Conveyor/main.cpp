//
//  main.cpp
//  lab_05_Conveyor
//
//  Created by Mac-HOME on 25.11.2019.
//  Copyright © 2019 Mac-HOME. All rights reserved.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

using namespace std;


struct Object {
    Object(size_t queuesCount, size_t number) : number(number) {
        for (int i = 0; i < queuesCount; ++i) {
            queueTimeBegin.push_back(0);
            queueTimeEnd.push_back(0);
        }
    }
    
    size_t number;
    
    vector <size_t> queueTimeBegin;
    vector <size_t> queueTimeEnd;
};


class LinearConveyor {
private:
    size_t elementsCount;
    size_t queuesCount;
    size_t averegeTime;
    
    size_t getCurTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
    
    size_t getRandDeltaTime() {
        return rand() % averegeTime;
    }
    
    void doObjectWork(Object& curObject, size_t queueNum) {
        size_t deltaTime = getRandDeltaTime();
        
        size_t start = getCurTime();
        this_thread::sleep_for(chrono::milliseconds(averegeTime * queueNum + deltaTime));
        size_t end = getCurTime();
        
        curObject.queueTimeBegin[queueNum] = start;
        curObject.queueTimeEnd[queueNum] = end;
    }
    
public:
    LinearConveyor(size_t elementsCount, size_t queuesCount, size_t milliseconds) : elementsCount(elementsCount), queuesCount(queuesCount), averegeTime(milliseconds) {}
    
    void execute() {
        queue <Object> objectsGenerator;
        
        for (size_t i = 0; i < elementsCount; ++i) {
            objectsGenerator.push(Object(queuesCount, i + 1));
        }
        
        vector <Object> objectsPool;
        
        while (objectsPool.size() != elementsCount) {
            Object curObject = objectsGenerator.front();
            objectsGenerator.pop();
            
            for (size_t i = 0; i < queuesCount; ++i) {
                doObjectWork(curObject, i);
            }
            
            objectsPool.push_back(curObject);
        }
        
        // objectsPool into file
    }
};


class Conveyor {
private:
    size_t elementsCount;
    size_t queuesCount;
    size_t averegeTime;
    const size_t delayTime = 1;
    
    vector <mutex> queuesMutex;
    vector <queue <Object> > queues;
    
    size_t getCurTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
    
    size_t getRandDeltaTime() {
        return rand() % averegeTime;
    }
    
    void doObjectWork(Object& curObject, size_t queueNum) {
        // cout << curObject.queueTimeEnd.size() << "?" << queueNum << endl;
        queuesMutex[queueNum].lock();
        // cout << curObject.queueTimeEnd.size() << "!" << endl;
        
        size_t deltaTime = getRandDeltaTime();
        
        size_t start = getCurTime();
        this_thread::sleep_for(chrono::milliseconds(averegeTime * queueNum + deltaTime));
        size_t end = getCurTime();
        
        // curObject.queueTimeBegin[queueNum] = start;
        // curObject.queueTimeEnd[queueNum] = end;
        
        cout << queueNum << endl;
        
        queuesMutex[queueNum].unlock();
    }
    
public:
    Conveyor(size_t elementsCount, size_t queuesCount, size_t milliseconds) : elementsCount(elementsCount), queuesCount(queuesCount), averegeTime(milliseconds) {
        queuesMutex = vector <mutex> (queuesCount);
        queues = vector <queue <Object> > (queuesCount);
    }
    
    void execute() {
        queue <Object> objectsGenerator;
        
        for (size_t i = 0; i < elementsCount; ++i) {
            objectsGenerator.push(Object(queuesCount, i + 1));
        }
        
        vector <thread> threads = vector <thread> (3);
        vector <Object> objectsPool;
        size_t prevTime = getCurTime() - delayTime;
        
        while (objectsPool.size() != elementsCount) {
            size_t curTime = getCurTime();
            
            if (prevTime + delayTime < curTime) {
                Object curObject = objectsGenerator.front();
                objectsGenerator.pop();
                queues[0].push(curObject);
                
                prevTime = getCurTime();
            }
            
            for (int i = 0; i < queuesCount; ++i) {
                if (!queues[i].empty()) {
                    Object curObject = queues[i].front();
                    queues[i].pop();
                    threads[i] = thread(&Conveyor::doObjectWork, this, ref(curObject), i);
                    
                    if (objectsPool.size() != elementsCount - 1) {
                        threads[i].detach();
                    }
                    else {
                        threads[i].join();
                    }
                    
                    if (i == queuesCount - 1) {
                        objectsPool.push_back(curObject);
                    }
                    else {
                        queues[i + 1].push(curObject);
                    }
                }
            }
        }
    }
};


int main(int argc, const char * argv[]) {
    while (true) {
        int key;
        cout << "Keys:\n1 - Algorithm work\n2 - Time\n0 - Exit\n\n" << "Input key: " << endl;
        cin >> key;
        
        if (key == 1) {
            int elementsCount;
            
            cout << "Input count of elements: ";
            cin >> elementsCount;
            
            LinearConveyor conveyorLR(elementsCount, 3, 10);
            
            auto begin = std::chrono::steady_clock::now();
            conveyorLR.execute();
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
            
            cout << "Linear conveyor" << ", time: " << duration.count() << " milliseconds" <<  endl;
            
            Conveyor conveyor(elementsCount, 3, 10);
            
            begin = std::chrono::steady_clock::now();
            conveyor.execute();
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
            
            cout << "Parallel conveyor" << ", time: " << duration.count() << " milliseconds" <<  endl;
            
            cout << endl;
        }
        else if (key == 2) {
            ;
        }
        else if (key == 0) {
            break;
        }
        
        cout << endl;
    }
    
    return 0;
}

