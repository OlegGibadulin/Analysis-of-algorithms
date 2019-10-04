vector<vector<int>> multiplyVinograd(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    const int nA = matrixA.size();
    const int nB = matrixB.size();
    if (nA < 1 || nB < 1) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    const int mA = matrixA[0].size();
    const int mB = matrixB[0].size();
    if (mA != nB || mA < 1 || mB < 1) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    vector<vector<int>> matrixRes(nA);
    for (int i = 0; i < nA; ++i) {
        matrixRes[i].assign(mB, 0);
    }
    
    vector<int> rowFactor(nA);
    for (int i = 0; i < nA; ++i) {
        for (int j = 0; j < nB / 2; ++j) {
            rowFactor[i] = rowFactor[i] + (matrixA[i][2 * j] * matrixA[i][2 * j + 1]);
        }
    }
    
    vector<int> columnFactor(mB);
    for (int i = 0; i < mB; ++i) {
        for (int j = 0; j < mA / 2; ++j) {
            columnFactor[i] = columnFactor[i] + (matrixB[2 * j][i] * matrixB[2 * j + 1][i]);
        }
    }
    
    for (int i = 0; i < nA; ++i) {
        for (int j = 0; j < mB; ++j) {
            matrixRes[i][j] = -rowFactor[i] - columnFactor[j];
            for (int k = 0; k < mA / 2; ++k) {
                matrixRes[i][j] = matrixRes[i][j] + ((matrixA[i][2 * k] + matrixB[2 * k + 1][j]) * (matrixA[i][2 * k + 1] + matrixB[2 * k][j]));
            }
        }
    }
    
    if (mA % 2 != 0)
    {
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < mB; ++j) {
                matrixRes[i][j] = matrixRes[i][j] + (matrixA[i][mA - 1] * matrixB[nB - 1][j]);
            }
        }
    }
    
    return matrixRes;
}

vector<vector<int>> improvedMultiplyVinograd(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB)
{
    const int nA = matrixA.size();
    const int nB = matrixB.size();
    if (nA < 1 || nB < 1) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    const int mA = matrixA[0].size();
    const int mB = matrixB[0].size();
    if (mA != nB || mA < 1 || mB < 1) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    vector<vector<int>> matrixRes(nA);
    for (int i = 0; i < nA; ++i) {
        matrixRes[i].assign(mB, 0);
    }
    
    int d = nB / 2;
    
    vector<int> rowFactor(nA);
    for (int i = 0; i < nA; ++i) {
        for (int j = 0; j < d; ++j) {
            rowFactor[i] += (matrixA[i][(j<<1)] * matrixA[i][(j<<1)|1]);
        }
    }
    
    vector<int> columnFactor(mB);
    for (int i = 0; i < mB; ++i) {
        for (int j = 0; j < d; ++j) {
            columnFactor[i] += (matrixB[(j << 1)][i] * matrixB[(j << 1) | 1][i]);
        }
    }
    
    if (nB % 2 == 0) {
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < mB; ++j) {
                matrixRes[i][j] = -rowFactor[i] - columnFactor[j];
                for (int k = 0; k < d; ++k) {
                    matrixRes[i][j] += ((matrixA[i][(k << 1)] + matrixB[(k << 1) | 1][j])* (matrixA[i][(k << 1) | 1] + matrixB[(k << 1)][j]));
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < mB; ++j) {
                matrixRes[i][j] = -rowFactor[i] - columnFactor[j] + (matrixA[i][nB - 1] * matrixB[nB - 1][j]);
                for (int k = 0; k < d; ++k) {
                    matrixRes[i][j] += ((matrixA[i][(k << 1)] + matrixB[(k << 1) | 1][j]) * (matrixA[i][(k << 1) | 1] + matrixB[(k << 1)][j]));
                }
            }
        }
    }
    
    return matrixRes;
}
