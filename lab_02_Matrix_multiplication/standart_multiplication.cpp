vector<vector<int>> standartMultiply(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    const int nA = matrixA.size();
    const int nB = matrixB.size();
    
    if (nA == 0 || nB == 0) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    const int mA = matrixA[0].size();
    const int mB = matrixB[0].size();
    
    if (mA != nB || mA == 1 || mB == 1) {
        cout << "Wrong dimension of the matrix" << endl;
        return vector<vector<int>>();
    }
    
    vector<vector<int>> matrixRes(nA);
    for (int i = 0; i < nA; ++i) {
        matrixRes[i].assign(mB, 0);
    }
    
    for (size_t row = 0; row < nA; ++row) {
        for (size_t column = 0; column < mB; ++column) {
            for (size_t k = 0; k < mA; ++k) {
                matrixRes[row][column] += matrixA[row][k] * matrixB[k][column];
            }
        }
    }
    
    return matrixRes;
}
