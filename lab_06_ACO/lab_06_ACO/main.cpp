//
//  main.cpp
//  lab_06_ACO
//
//  Created by Mac-HOME on 04.11.2019.
//  Copyright © 2019 Mac-HOME. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

void getRandomGraph(vector< vector <int>>& graph, const size_t graphSize) {
    ;
}


void showGraph(const vector< vector <int>>& graph, const size_t graphSize) {
    ;
}


size_t bruteForce(const vector< vector <int>>& graph) {
    return 0;
}


size_t ACO(const vector< vector <int>>& graph) {
    return 0;
}


int main(int argc, const char * argv[]) {
    size_t repeatCount = 10;
    
    while (true)
    {
        int key;
        cout << "Keys:\n1 - Algorithm work\n2 - Time\n0 - Exit\n\n" << "Input key: " << endl;
        cin >> key;
        
        if (key == 1)
        {
            size_t start;
            size_t end;
            size_t pathLength;
            
            vector< vector <int>> graph;
            int graphSize;
            
            cout << "Input graph size: ";
            cin >> graphSize;
            
            if (graphSize < 0)
            {
                cout << "Error size";
                return 1;
            }
            
            /*for (int i = 0; i < graphSize; ++i) {
                int num;
                cin >> num;
                graph.push_back(num);
            }*/
            
            getRandomGraph(graph, graphSize);
            showGraph(graph, graphSize);
            
            start = __rdtsc();
            pathLength = bruteForce(graph);
            end = __rdtsc();
            cout << "Brute force:" << pathLength << ", time: " << (end - start) << endl;
            
            start = __rdtsc();
            pathLength = ACO(graph);
            end = __rdtsc();
            cout << "ACO" << pathLength << ", time: " << (end - start) << endl;
            
            cout << endl;
        }
        else if (key == 2) {
            for (int j = 1; j < 10; j += 1) {
                vector< vector <int>> graph;
                size_t graphSize = j;
                
                getRandomGraph(graph, graphSize);
                
                cout << j << endl;
                
                size_t start;
                size_t end;
                
                start = __rdtsc();
                for (int i = 0; i < repeatCount; ++i) {
                    ;
                }
                end = __rdtsc();
                
                cout << " time: " << (end - start) / repeatCount  << endl;
                
                cout << endl;
            }
        }
        else if (key == 0) {
            break;
        }
        
        cout << endl;
    }
    
    return 0;
}