void getMatrixRes(vector<vector<int>>& matrixRes, const size_t nA, const size_t mB) {
    vector<vector<int>> tmp(nA);
    for (size_t i = 0; i < nA; ++i) {
        tmp[i].assign(mB, 0);
    }
    matrixRes = tmp;
}

void calculateRowFactor(vector<int>& rowFactor, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, const size_t nA, const size_t d) {
    vector<int> tmp(nA);
    for (size_t i = 0; i < nA; ++i) {
        for (size_t j = 0; j < d; ++j) {
            tmp[i] += (matrixA[i][(j * 2)] * matrixA[i][(j * 2) + 1]);
        }
    }
    
    rowFactor = tmp;
}

void calculateColumnFactor(vector<int>& columnFactor, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, const size_t mB, const size_t d) {
    vector<int> tmp(mB);
    for (size_t i = 0; i < mB; ++i) {
        for (size_t j = 0; j < d; ++j) {
            tmp[i] += (matrixB[(j * 2)][i] * matrixB[(j * 2) + 1][i]);
        }
    }
    columnFactor = tmp;
}

void calculateGeneralProductionRows(vector<vector<int>>& matrixRes, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, const vector<int>& rowFactor, const vector<int>& columnFactor, const size_t lowRow, const size_t highRow, const size_t mB, const size_t d) {
    for (size_t i = lowRow; i < highRow; ++i) {
        for (size_t j = 0; j < mB; ++j) {
            matrixRes[i][j] = -rowFactor[i] - columnFactor[j];
            for (size_t k = 0; k < d; ++k) {
                matrixRes[i][j] = matrixRes[i][j] + ((matrixA[i][2 * k] + matrixB[2 * k + 1][j]) * (matrixA[i][2 * k + 1] + matrixB[2 * k][j]));
            }
        }
    }
}

void calculateAdditionalProduction(vector<vector<int>>& matrixRes, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, const size_t nA, const size_t mB, const size_t nB, const size_t mA) {
    for (size_t i = 0; i < nA; ++i) {
        for (size_t j = 0; j < mB; ++j) {
            matrixRes[i][j] = matrixRes[i][j] + (matrixA[i][mA - 1] * matrixB[nB - 1][j]);
        }
    }
}

vector<vector<int>> parallelMultiplyWinograd(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, const size_t threadsCount = 4)
{
    const size_t nA = matrixA.size();
    const size_t nB = matrixB.size();
    if (nA == 0 || nB == 0) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    const size_t mA = matrixA[0].size();
    const size_t mB = matrixB[0].size();
    if (mA != nB || mA == 0 || mB == 0) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
        
    }
    
    vector<vector<int>> matrixRes;
    vector<int> rowFactor;
    vector<int> columnFactor;
    const size_t d = nB / 2;
    
    thread matrixResThread(getMatrixRes, ref(matrixRes), nA, mB);
    thread rowFactorThread(calculateRowFactor, ref(rowFactor), ref(matrixA), ref(matrixB), nA, d);
    thread columnFactorThread(calculateColumnFactor, ref(columnFactor), ref(matrixA), ref(matrixB), mB, d);
    
    if (matrixResThread.joinable() && rowFactorThread.joinable() && columnFactorThread.joinable()) {
        matrixResThread.join();
        rowFactorThread.join();
        columnFactorThread.join();
    }
    
    double threadRowCount = (double) nA / threadsCount;
    if (threadRowCount < 1) {
        threadRowCount = 1;
    }
    vector<thread> threads;
    
    for (size_t i = 0, lowRow = 0, highRow = 0 && highRow < nA; i < threadsCount; ++i) {
        highRow += threadRowCount;
        if (i == threadsCount) {
            highRow = nA;
        }
        threads.push_back(thread(calculateGeneralProductionRows, ref(matrixRes), ref(matrixA), ref(matrixB), ref(rowFactor), ref(columnFactor), lowRow, highRow, mB, d));
        lowRow = highRow;
    }
    
    if (mA % 2 != 0) {
        thread additionalProductionTread(calculateAdditionalProduction, ref(matrixRes), ref(matrixA), ref(matrixB), nA, mB, nB, mA);
        if (additionalProductionTread.joinable()) {
            additionalProductionTread.join();
        }
    }
    
    for (int i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
    threads.clear();
    
    return matrixRes;
}
