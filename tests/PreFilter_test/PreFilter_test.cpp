#include "PreFilter.h"
#include "StringUtil.h"
#include <cassert>
#include <iostream>
using namespace std;

void test_case_1() {
    
    unordered_set<Rune> symbol;
    symbol.insert(65292u); // "，"
    symbol.insert(12290u); // "。"
    string expected;
    string res;

    {
        string s = "你好，美丽的，世界";
        PreFilter filter(symbol, s);
        assert(filter.HasNext() == true);

        vector<string> words;
        while (filter.HasNext()) {
            PreFilter::Range range;
            range = filter.Next();
            string tmp = GetStringFromRunes(s, range.begin, range.end - 1);
            words.push_back(tmp);
        }

        expected = "你好/，/美丽的/，/世界";
        res = Join(words.begin(), words.end(), "/");
        assert(res == expected);
    }

    {
        string s = "我来自北京邮电大学。。。学号123456，用AK47";
        PreFilter filter(symbol, s);
        assert(filter.HasNext() == true);

        vector<string> words;
        while (filter.HasNext()) {
            PreFilter::Range range;
            range = filter.Next();
            string tmp = GetStringFromRunes(s, range.begin, range.end - 1);
            words.push_back(tmp);
        }

        expected = "我来自北京邮电大学/。/。/。/学号123456/，/用AK47";
        res = Join(words.begin(), words.end(), "/");
        assert(res == expected);
    }
}

int main() {

    test_case_1();
    return 0;
}