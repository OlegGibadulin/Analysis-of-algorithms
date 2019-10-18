void ShellSort (vector<int>& arr, const int arrSize) {
    for (int step = arrSize / 2; step > 0; step /= 2) {
        for (int i = step; i < arrSize; i++) {
            for (int j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step) {
                swap(arr[j], arr[j + step]);
            }
        }
    }
}

void gnomeSort(vector<int>& arr, const int arrSize) {
    for (size_t i = 0; i + 1 < arrSize; ++i) // 2
        if (arr[i] > arr[i + 1]) { // 3
            swap(arr[i], arr[i + 1]); // 10
            if (i > 0) { // 1
                i -= 2; // 1
            }
        }
}

void combSort(vector<int>& arr, const int arrSize) {
    const double fakt = 1.2473309;
    double step = arrSize - 1;
    
    while (step >= 1) {
        for (size_t i = 0; i + step < arrSize; ++i) {
            if (arr[i] > arr[i + step]) {
                swap(arr[i], arr[i + step]);
            }
        }
        step /= fakt;
    }
    
    for (int i = 0; i < arrSize - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < arrSize - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        if (!swapped)
            break;
    }
}
