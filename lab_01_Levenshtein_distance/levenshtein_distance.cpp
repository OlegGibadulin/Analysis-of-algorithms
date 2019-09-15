size_t findMin(size_t a, size_t b, size_t c) {
    return std::min(std::min(a, b), c);
}

size_t match(char c1, char c2) {
    return (c1 == c2) ? 0 : 1;
}

size_t levenshteinDistance(const std::string& str1, const std::string& str2) {
    const size_t len1 = str1.size();
    const size_t len2 = str2.size();
    
    if (len1 == 0) {
        return len2;
    }
    if (len2 == 0) {
        return len1;
    }
    
    std::vector<size_t> prevRow;
    for (int i = 0; i < len2 + 1; ++i) {
        prevRow.push_back(i);
    }
    std::vector<size_t> curRow;
    
    for (int i = 1; i < len1 + 1; ++i) {
        curRow.push_back(i);
        for (int j = 1; j < len2 + 1; ++j) {
            const size_t insertionCost = curRow[j - 1] + 1;
            const size_t deletionCost = prevRow[j] + 1;
            const size_t substitutionCost = prevRow[j - 1] + match(str1[i - 1], str2[j - 1]);
            const size_t minCost = findMin(insertionCost, deletionCost, substitutionCost);
            curRow.push_back(minCost);
        }
        
        prevRow = curRow;
        curRow.clear();
    }
    
    return curRow[len2];
}
