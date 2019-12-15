//
//  main.cpp
//  lab_07_String_searching
//
//  Created by Mac-HOME on 13.12.2019.
//  Copyright © 2019 Mac-HOME. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

//int countKMP = 0;
//int countBM = 0;

void getPrefixFunction(vector <int> &pf, const string &pattern) {
    if (pattern.empty()) {
        return;
    }
    
    pf[0] = 0;
    
    for (int i = 1, j = 0; i < pattern.size(); ++i) {
        if (j > 0 && pattern[i] != pattern[j]) {
            j = pf[j - 1];
        }
        
        if (pattern[i] == pattern[j]) {
            ++j;
        }
        
        pf[i] = j;
    }
}

vector <int> KMP(const string &s, const string &pattern) {
    if (s.empty() || pattern.empty()) {
        return vector <int>();
    }
    
    vector <int> mathesIndexes;
    vector <int> pf(pattern.size());
    
    getPrefixFunction(pf, pattern);
    
    for (int i = 0, j = 0; i < s.size(); ++i) {
//        countKMP += 1;
        if (j > 0 && pattern[j] != s[i]) {
            j = pf[j - 1];
        }
        
        if (pattern[j] == s[i]) {
            ++j;
        }
        
        if (j == pattern.size()) {
            mathesIndexes.push_back(i - pattern.size() + 1);
        }
    }
    
    return mathesIndexes;
}

void getSlide(vector <int>  &slide, const string &pattern) {
    if (pattern.empty()) {
        return;
    }
    
    const int alphabetSize = 255;
    
    for (int i = 0; i < alphabetSize; ++i) {
        slide.push_back(pattern.size());
    }
    
    for (int i = 0; i < pattern.size() - 1; ++i) {
        slide[pattern[i]] = pattern.size() - i - 1;
    }
}

vector <int> BM(const string &s, const string &pattern) {
    if (s.empty() || pattern.empty()) {
        return vector <int>();
    }
    
    vector <int> mathesIndexes;
    vector <int> slide;
    
    getSlide(slide, pattern);
    
    int sLen = s.size();
    int pLen = pattern.size();
    int sInd = 0;
    
    while (sInd < sLen - (pLen - 1)) {
        int pInd = pLen - 1;
        
//        if (pattern[pInd] != s[sInd + pInd]) {
//            countBM += 1;
//        }
        
        for (;pInd >= 0 && pattern[pInd] == s[sInd + pInd]; --pInd) {
            if (pInd == 0) {
                mathesIndexes.push_back(sInd);
            }
//            countBM += 1;
        }
        
        sInd += slide[s[sInd + pInd]];
    }
    
    return mathesIndexes;
}

int main(int argc, const char * argv[]) {
    string s1 = "abcerere";s
    string s2 = "ere";
    vector <int> mathesIndexes = KMP(s1, s2);
    
    cout << "KMP" << endl;
    if (mathesIndexes.empty()) {
        cout << s1 << " does not contatain " << s2 << endl;
    }
    else {
        cout << s1 << " contatains " << s2 << " by index:";
        for (int i = 0; i < mathesIndexes.size(); ++i) {
            cout << " " << mathesIndexes[i];
        }
        cout << endl;
    }
    
    mathesIndexes = BM(s1, s2);
    
    cout << "BM" << endl;
    if (mathesIndexes.empty()) {
        cout << s1 << " does not contatain " << s2 << endl;
    }
    else {
        cout << s1 << " contatains " << s2 << " by index:";
        for (int i = 0; i < mathesIndexes.size(); ++i) {
            cout << " " << mathesIndexes[i];
        }
        cout << endl;
    }
    
//    cout << countKMP << " " << countBM << endl;
    
    return 0;
}
