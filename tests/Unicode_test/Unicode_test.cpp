#include "Unicode.h"
#include <iostream>
#include <cassert>

using namespace std;

void test_case_1() {
    string s = "你好世界";
    RuneStrArray runes;
    assert(DecodeRunesInString(s, runes) == true);
    string actual;
    string expected = "[\"{\"rune\": \"20320\", \"offset\": 0, \"len\": 3}\", \"{\"rune\": \"22909\", \"offset\": 3, \"len\": 3}\", \"{\"rune\": \"19990\", \"offset\": 6, \"len\": 3}\", \"{\"rune\": \"30028\", \"offset\": 9, \"len\": 3}\"]";
    actual << runes;
    assert(expected == actual);
}

void test_case_2() {
   string s = "123\x80";
    RuneStrArray runes;
    assert(DecodeRunesInString(s, runes) == false);
    string actual;
    string expected = "[]";
    actual << runes;
    assert(expected == actual);
}

void test_case_3() {
   const size_t ITERATION = 1024;
    const size_t MAX_LEN = 256;
    string s;
    srand(time(NULL));

    for (size_t i = 0; i < ITERATION; i++) {
        size_t len = rand() % MAX_LEN;
        s.resize(len);
        for (size_t j = 0; j < len; j++) {
        s[rand() % len] = rand();
        }
        RuneStrArray runes;
        DecodeRunesInString(s, runes);
    }
}

int main() {
    test_case_3();
    return 0;
}