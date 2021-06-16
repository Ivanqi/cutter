#include "SegmentBase.h"
#include "HMMSegment.h"
#include <cassert>
#include <iostream>
using namespace std;

void test_case_1() {
    HMMSegment hmmsegment("../../dict/hmm_model.utf8");
    string sentence;
    vector<string> words;
    string actual;
    string expected;

    {
        sentence = "我来自北京邮电大学。。。学号123456，用AK47";
        expected = "我/来自/北京邮电大学/。/。/。/学号/123456/，/用/AK47";

        hmmsegment.Cut(sentence, words);
        actual = Join(words.begin(), words.end(), "/");
        cout << "actual: " << actual << endl;
        assert(actual == expected);
    }
}

int main() {

    test_case_1();
    return 0;
}