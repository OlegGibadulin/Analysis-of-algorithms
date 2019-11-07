//
//  main.cpp
//  lab_06_ACO
//
//  Created by Mac-HOME on 04.11.2019.
//  Copyright © 2019 Mac-HOME. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;


void getRandomGraph(vector <vector <int>>& graph, const size_t graphSize) {
    for (int i = 0; i < graphSize; ++i) {
        vector<int> line;
        for (int j = 0; j < graphSize; ++j) {
            if (i == j) {
                line.push_back(0);
            }
            else {
                line.push_back(rand() % 9 + 1);
            }
        }
        graph.push_back(line);
    }
}


void showGraph(const vector <vector <int>>& graph, const size_t graphSize) {
    if (graph.size() == 0)
    {
        cout << "This graph is empty" << endl;
    }
    cout << endl;
    
    for (auto &line : graph)
    {
        for (auto &elem : line)
        {
            cout << elem << " ";
        }
        cout << endl;
    }
    cout << endl;
}


class BruteForce {
private:
    const vector <vector <int>> graph;
    const size_t graphSize;
    vector <bool> visited;
    size_t initialVert = 0;
    
public:
    size_t minLength = 0;
    vector <size_t> minPath;
    
    BruteForce(const vector<vector <int>>& graph) : graph(graph), graphSize(graph.size()) {
        for (int i = 0; i < graphSize; ++i) {
            visited.push_back(false);
        }
    }
    
    void execute() {
        vector<size_t> path;
        for (initialVert = 0; initialVert < graphSize; ++initialVert) {
            execute(initialVert, 0, path);
        }
    }
    
private:
    void execute(const size_t vert, size_t curLength, vector<size_t>& curPath) {
        bool isThereNodes = false;
        visited[vert] = true;
        curPath.push_back(vert);
        
        for (size_t i = 0; i < graphSize; ++i) {
            if (visited[i] == false) {
                isThereNodes = true;
                execute(i, curLength + graph[vert][i], curPath);
            }
        }
        
        if (isThereNodes == false) {
            curLength += graph[vert][initialVert];
            if (curLength < minLength or minLength == 0) {
                minLength = curLength;
                minPath = curPath;
            }
        }
        
        curPath.pop_back();
        visited[vert] = false;
    }
};


class Ant {
public:
    size_t pathLength;
    vector <bool> visited;
    vector <size_t> path;
    
    Ant(const size_t graphSize) : pathLength(0) {
        for (size_t i = 0; i < graphSize; ++i) {
            path.push_back(0);
            visited.push_back(false);
        }
    }
    
    void visitCity(const size_t city, const size_t curPathLength, const size_t curPathDistance) {
        pathLength += curPathDistance;
        path[curPathLength] = city;
        visited[city] = true;
    }
    
    void clearVisits() {
        for (size_t i = 0; i < visited.size(); ++i) {
            visited[i] = false;
        }
    }
    
    void makeDefaultPath() {
        pathLength = 0;
        visitCity(path[path.size() - 1], 0, 0);
    }
    
    bool isVisited(const size_t city) const {
        return visited[city];
    }
};


class ACO {
private:
    const vector <vector <int>> distGraph;
    const size_t citiesCount;
    
    vector <vector <double>> pherGraph;
    vector <vector <double>> desireGraph;
    
    vector <Ant> ants;
    size_t antsCount;
    
    vector <double> pathsProbabilities;
    
    double alpha = 0.5;
    double rho = 0.5;
    size_t tMax = 100;
    double beta = 1 - alpha;
    double Q = 5;
    double antsFactor = 1;
    double initialPherVal = 1;
    
public:
    size_t minLength = 0;
    vector <size_t> minPath;
    
    ACO(const vector<vector <int>>& graph) : distGraph(graph), citiesCount(graph.size()) {
        // init pherGraph
        for (size_t i = 0; i < citiesCount; ++i) {
            vector<double> line;
            for (size_t j = 0; j < citiesCount; ++j) {
                line.push_back(initialPherVal);
            }
            pherGraph.push_back(line);
        }
        
        // init desireGraph
        for (size_t i = 0; i < citiesCount; ++i) {
            vector<double> line;
            for (size_t j = 0; j < citiesCount; ++j) {
                if (distGraph[i][j] == 0) {
                    line.push_back(0);
                }
                else {
                    line.push_back(1.0 / distGraph[i][j]);
                }
            }
            desireGraph.push_back(line);
        }
        
        // init antsCount
        antsCount = citiesCount * antsFactor;
        for (size_t i = 0; i < antsCount; ++i) {
            Ant ant(citiesCount);
            ants.push_back(ant);
        }
        
        // init pathsProbabilities
        for (size_t i = 0; i < citiesCount; ++i) {
            pathsProbabilities.push_back(0);
        }
    }
    
    void execute() {
        initPherGraph();
        initAnts();
        
        for (size_t i = 0; i < tMax; ++i) {
            paveAntsPaths();
            updateMinPath();
            updatePheromones();
            makeDefaultAnts();
        }
    }
    
    void changeParams(double alpha, double rho, size_t tMax) {
        this->alpha = alpha;
        this->beta = 1 - alpha;
        this->rho = rho;
        this->tMax = tMax;
    }
    
private:
    void initAnts() {
        for (size_t i = 0; i < antsCount; ++i) {
            ants[i].visitCity(rand() % citiesCount, 0, 0);
        }
    }
    
    void initPherGraph() {
        for (size_t i = 0; i < citiesCount; ++i) {
            for (size_t j = 0; j < citiesCount; ++j) {
                pherGraph[i][j] = initialPherVal;
            }
        }
    }
    
    void paveAntsPaths() {
        for (size_t i = 0; i < citiesCount - 1; ++i) {
            for (size_t j = 0; j < antsCount; ++j) {
                const size_t curCity = ants[j].path[i];
                const size_t nextCity = getNextCity(ants[j], curCity);
                const int distance = distGraph[curCity][nextCity];
                
                ants[j].visitCity(nextCity, i + 1, distance);
            }
        }
        
        for (size_t j = 0; j < antsCount; ++j) {
            const int distToInitialCity = distGraph[ants[j].path[ants[j].path.size() - 1]][ants[j].path[0]];
            ants[j].pathLength += distToInitialCity;
        }
    }
    
    size_t getNextCity(const Ant& ant, const size_t curCity) {
        double sumP = 0;
        
        for (int i = 0; i < citiesCount; ++i) {
            sumP += pow(pherGraph[curCity][i], alpha) * pow(desireGraph[curCity][i], beta);
        }
        
        for (int i = 0; i < citiesCount; ++i) {
            if (i == curCity || ant.isVisited(i)) {
                pathsProbabilities[i] = 0;
            }
            else {
                pathsProbabilities[i] = pow(pherGraph[curCity][i], alpha) * pow(desireGraph[curCity][i], beta) / sumP;
            }
        }
        
        size_t nextCity = selectNextCity();
        
        return nextCity;
    }
    
    void updateMinPath() {
        for (size_t i = 0; i < antsCount; ++i) {
            const size_t curLength = ants[i].pathLength;
            if (curLength < minLength || minLength == 0) {
                minLength = curLength;
                minPath = ants[i].path;
            }
        }
    }
    
    void updatePheromones() {
        for (int i = 0; i < citiesCount; ++i) {
            for (int j = 0; j < citiesCount; ++j) {
                pherGraph[i][j] *= (1 - rho);
            }
        }
        
        for (size_t i = 0; i < antsCount; ++i) {
            double deltaTau = Q / ants[i].pathLength;
            for (int j = 0; j < citiesCount - 1; ++j) {
                pherGraph[ants[i].path[j]][ants[i].path[j + 1]] += deltaTau;
            }
            pherGraph[ants[i].path[citiesCount - 1]][ants[i].path[0]] += deltaTau;
        }
    }
    
    void makeDefaultAnts() {
        for (size_t i = 0; i < antsCount; ++i) {
            ants[i].clearVisits();
            ants[i].makeDefaultPath();
        }
    }
    
    size_t selectNextCity() {
        double sumProbabilities = getSumProbabilities();
        
        double randNum = ((double) rand() / (RAND_MAX)) * sumProbabilities;
        double total = 0;
        size_t city = 0;
        
        for (size_t i = 0; i < citiesCount; ++i) {
            total += pathsProbabilities[i];
            if (total >= randNum) {
                city = i;
                break;
            }
        }
        
        return city;
    }
    
    double getSumProbabilities() {
        double sumProbabilities = 0;
        
        for (size_t i = 0; i < citiesCount; ++i) {
            sumProbabilities += pathsProbabilities[i];
        }
        
        return sumProbabilities;
    }
};


int main(int argc, const char * argv[]) {
    while (true) {
        int key;
        cout << "Keys:\n1 - Algorithm work\n2 - Time\n0 - Exit\n\n" << "Input key: " << endl;
        cin >> key;
        
        if (key == 1) {
            size_t start;
            size_t end;
            size_t pathLength;
            vector <size_t> path;
            
            vector <vector <int>> graph;
            int graphSize;
            
            cout << "Input graph size: ";
            cin >> graphSize;
            
            if (graphSize <= 0) {
                cout << "Error size";
                return 1;
            }
            
            getRandomGraph(graph, graphSize);
            showGraph(graph, graphSize);
            
            BruteForce algorithm(graph);
            start = __rdtsc();
            algorithm.execute();
            end = __rdtsc();
            
            pathLength = algorithm.minLength;
            path = algorithm.minPath;
            
            cout << "Brute force: [";
            for (int i = 0; i < graphSize; ++i) {
                cout << " " << path[i];
            }
            cout << " ], " << pathLength << ", time: " << (end - start) << endl;
            
            ACO algorithmACO(graph);
            start = __rdtsc();
            algorithmACO.execute();
            end = __rdtsc();
            pathLength = algorithmACO.minLength;
            path = algorithmACO.minPath;
            cout << "ACO: [";
            for (int i = 0; i < graphSize; ++i) {
                cout << " " << path[i];
            }
            cout << " ], " << pathLength << ", time: " << (end - start) << endl;
            
            cout << endl;
        }
        else if (key == 2) {
            vector <vector <int>> graph;
            int graphSize = 10;
            
            size_t pathLengthBF;
            vector <size_t> pathBF;
            size_t pathLengthACO;
            vector <size_t> pathACO;
            
            getRandomGraph(graph, graphSize);
            showGraph(graph, graphSize);
            
            BruteForce algorithm(graph);
            algorithm.execute();
            pathLengthBF = algorithm.minLength;
            pathBF = algorithm.minPath;
            
            ACO algorithmACO(graph);
            
            for (double alpha = 0; alpha < 1; alpha += 0.1) {
                for (double rho = 0; rho < 1; rho += 0.1) {
                    for (size_t t = 0; t < 500; t += 100) {
                        algorithmACO.changeParams(alpha, rho, t);
                        algorithmACO.execute();
                        
                        pathLengthACO = algorithmACO.minLength;
                        pathACO = algorithmACO.minPath;
                        
                        cout << pathLengthBF << " " << pathLengthACO << endl;
                    }
                }
            }
        }
        else if (key == 0) {
            break;
        }
        
        cout << endl;
    }
    
    return 0;
}
