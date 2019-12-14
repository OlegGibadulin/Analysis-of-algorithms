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

void getPrefixFunction(vector <int> &pf, const string &pattern) {
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
    vector <int> mathesIndexes;
    vector <int> pf(pattern.size());
    
    getPrefixFunction(pf, pattern);
    
    for (int i = 0, j = 0; i < s.size(); ++i) {
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

void getSlide(map <char, int> &slide, const string &pattern) {
    for (int i = 0; i < pattern.size(); ++i) {
        slide[pattern[i]] = pattern.size() - i;
    }
}

vector <int> BM(const string &s, const string &pattern) {
    vector <int> mathesIndexes;
    map <char, int> slide;
    
    getSlide(slide, pattern);
    
    int sLen = s.size();
    int patternLen = pattern.size();
    int sInd = 0;
    
    while (sInd <= sLen - patternLen) {
        int patternInd = patternLen - 1;
        
        while (patternInd >= 0 && pattern[patternInd] == s[sInd + patternInd]) {
            --patternInd;
        }
        
        if (patternInd < 0) {
            mathesIndexes.push_back(sInd);
            sInd += patternLen;
        }
        else {
            if (slide.count(pattern[patternInd]) == 0) {
                sInd += patternLen;
            }
            else {
                sInd += slide.find(pattern[patternInd])->second;
            }
        }
    }
    
    return mathesIndexes;
}

int main(int argc, const char * argv[]) {
    string s1 = "asfasfashjk";
    string s2 = "asf";
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
    
    return 0;
}
