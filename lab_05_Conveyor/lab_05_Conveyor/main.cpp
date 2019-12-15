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
#include <fstream>

using namespace std;


struct Object {
    Object(size_t number) : number(number) {}
    
    size_t number;
};

// vector <vector <int> > objectTimeStayingAtQueue(3);


class Conveyor {
private:
    size_t elementsCount;
    size_t queuesCount;
    size_t averegeTime;
    ofstream resTimeFile;
    const size_t delayTime = 1;
    mutex fileMutex;
    
    size_t getCurTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
    
    size_t getRandDeltaTime() {
        return rand() % averegeTime;
    }
    
    void doObjectLinearWork(Object& curObject, size_t queueNum) {
        size_t start = getCurTime();
        resTimeFile << "Object #" << curObject.number <<  " from Queue #" << queueNum << ": START - " << start << endl;
        
        size_t deltaTime = getRandDeltaTime();
        this_thread::sleep_for(chrono::milliseconds(averegeTime + deltaTime));
        
        size_t end = getCurTime();
        resTimeFile << "Object #" << curObject.number <<  " from Queue #" << queueNum << ": STOP -  " << end << endl;
    }
    
public:
    Conveyor(size_t elementsCount, size_t queuesCount, size_t milliseconds) : elementsCount(elementsCount), queuesCount(queuesCount), averegeTime(milliseconds) {}
    
    void executeLinear() {
        resTimeFile.open("LinearConveyor.txt");
        
        queue <Object> objectsGenerator;
        
        for (size_t i = 0; i < elementsCount; ++i) {
            objectsGenerator.push(Object(i + 1));
        }
        
        vector <Object> objectsPool;
        
        while (objectsPool.size() != elementsCount) {
            Object curObject = objectsGenerator.front();
            objectsGenerator.pop();
            
            for (size_t i = 0; i < queuesCount; ++i) {
                doObjectLinearWork(curObject, i);
            }
            
            objectsPool.push_back(curObject);
        }
        
        resTimeFile.close();
    }
    
private:
    void doObjectParallelWork(Object curObject, queue <Object>& queue, size_t queueNum, mutex& mutex) {
        size_t deltaTime = getRandDeltaTime();
        this_thread::sleep_for(chrono::milliseconds(averegeTime + deltaTime));
        
        mutex.lock();
        queue.push(curObject);
        mutex.unlock();
        
        size_t end = getCurTime();
        fileMutex.lock();
        resTimeFile << "Object #" << curObject.number <<  " from Queue #" << queueNum << ": STOP -  " << end << endl;
        fileMutex.unlock();
        
        // objectTimeStayingAtQueue[queueNum][objectTimeStayingAtQueue.size() - 1] -= end;
    }
    
public:
    void executeParallel() {
        resTimeFile.open("ParallelConveyor.txt");
        
        queue <Object> objectsGenerator;
        
        for (size_t i = 0; i < elementsCount; ++i) {
            objectsGenerator.push(Object(i + 1));
        }
        
        vector <thread> threads(3);
        vector <queue <Object> > queues(3);
        queue <Object> objectsPool;
        vector <mutex> mutexes(4);
        size_t prevTime = getCurTime() - delayTime;
        
        while (objectsPool.size() != elementsCount) {
            size_t curTime = getCurTime();
            
            if (!objectsGenerator.empty() && prevTime + delayTime < curTime) {
                Object curObject = objectsGenerator.front();
                objectsGenerator.pop();
                queues[0].push(curObject);
                
                prevTime = getCurTime();
            }
            
            for (int i = 0; i < queuesCount; ++i) {
                if (threads[i].joinable()) {
                    threads[i].join();
                }
                if (!queues[i].empty() && !threads[i].joinable()) {
                    mutexes[i].lock();
                    Object curObject = queues[i].front();
                    queues[i].pop();
                    mutexes[i].unlock();
                    
                    size_t start = getCurTime();
                    fileMutex.lock();
                    resTimeFile << "Object #" << curObject.number <<  " from Queue #" << i << ": START - " << start << endl;
                    fileMutex.unlock();
                    
                    // objectTimeStayingAtQueue[i].push_back(start);
                    
                    if (i == queuesCount - 1) {
                        threads[i] = thread(&Conveyor::doObjectParallelWork, this, curObject, ref(objectsPool), i, ref(mutexes[i + 1]));
                    }
                    else {
                        threads[i] = thread(&Conveyor::doObjectParallelWork, this, curObject, ref(queues[i + 1]), i, ref(mutexes[i + 1]));
                    }
                }
            }
        }
        
        for (int i = 0; i < queuesCount; ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        
        resTimeFile.close();
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
            
            Conveyor conveyor(elementsCount, 3, 10);
            
            auto begin = std::chrono::steady_clock::now();
            conveyor.executeLinear();
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
            
            cout << "Linear conveyor" << ", time: " << duration.count() << " milliseconds" <<  endl;
            
            begin = std::chrono::steady_clock::now();
            conveyor.executeParallel();
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
            
            cout << "Parallel conveyor" << ", time: " << duration.count() << " milliseconds" <<  endl;
            
            /*for (int i = 0; i < 3; ++i) {
                int sum = 0;
                for (int j = 0; j < elementsCount; ++j) {
                    sum += objectTimeStayingAtQueue[i][j];
                }
                
                cout << sum / elementsCount << endl;
            }*/
            
            cout << endl;
        }
        else if (key == 2) {
            ofstream tests;
            tests.open("tests.txt");
            for (int elementsCount = 100; elementsCount < 1100; elementsCount += 100) {
                Conveyor conveyor(elementsCount, 3, 5);
                
                auto begin = std::chrono::steady_clock::now();
                conveyor.executeLinear();
                auto end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
                
                tests << elementsCount << "&" << duration.count() << "&";
                
                begin = std::chrono::steady_clock::now();
                conveyor.executeParallel();
                end = std::chrono::steady_clock::now();
                duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
                
                tests << duration.count() << "\\\\" << endl;
                
                cout << endl;
            }
            
            tests.close();
        }
        else if (key == 0) {
            break;
        }
        
        cout << endl;
    }
    
    return 0;
}

