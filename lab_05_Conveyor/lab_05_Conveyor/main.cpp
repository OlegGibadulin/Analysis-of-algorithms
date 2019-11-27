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

#define time_type std::__1::chrono::duration<long long, std::__1::ratio<1, 1000000000> >

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

class Algorithm {
private:
    size_t elementsCount;
    size_t queuesCount;
    size_t averegeTime;
    
public:
    Algorithm(size_t elementsCount, size_t queuesCount, size_t milliseconds) : elementsCount(elementsCount), queuesCount(queuesCount), averegeTime(milliseconds) {}
    
    void linearRealization() {
        ;
    }
    
    void conveyor() {
        ;
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
            
            Algorithm algorithm(elementsCount, 3, 100);
            
            auto begin = std::chrono::steady_clock::now();
            algorithm.linearRealization();
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
            
            cout << "Linear realisztion" << ", time: " << duration.count() << " milliseconds" <<  endl;
            
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

