#ifndef MAXSEGMENT_H
#define MAXSEGMENT_H

#include <cassert>
#include "SegmentTagged.h"
#include "MPSegment.h"
#include "HMMSegment.h"
#include "PosTagger.h"

class MixSegment: public SegmentTagged
{
    private:
        MPSegment mpSeg_;
        HMMSegment hmmSeg_;
        PosTagger tagger_;

    public:
        MixSegment(const string& mpSegDict, const string& hmmSegDict, const string& userDict = "")
            : mpSeg_(mpSegDict, userDict), hmmSeg_(hmmSegDict)
        {

        }

        MixSegment(const DictTrie *dictTrie, const HMMModel *model)
            : mpSeg_(dictTrie), hmmSeg_(model)
        {

        }

        ~MixSegment()
        {

        }

        void Cut(const string& sentence, vector<string>& words) const
        {
            Cut(sentence, words, true);
        }

        void Cut(const string& sentence, vector<string>& words, bool hmm) const
        {
            vector<Word> tmp;
            Cut(sentence, tmp, hmm);
            GetStringsFromWords(tmp, words);
        }

        void Cut(const string& sentence, vector<Word>& words, bool hmm = true) const
        {
            PreFilter pre_filter(symbols_, sentence);
            PreFilter::Range range;
            vector<WordRange> wrs;

            wrs.reserve(sentence.size() / 2);

            while (pre_filter.HasNext()) {
                range = pre_filter.Next();
                Cut(range.begin, range.end, wrs, hmm);
            }

            words.clear();
            words.reserve(wrs.size());
            GetWordsFromWordRanges(sentence, wrs, words);
        }

        void Cut(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<WordRange>& res, bool hmm) const
        {
            if (!hmm) {
                mpSeg_.Cut(begin, end, res);
                return;
            }

            vector<WordRange> words;
            assert(end >= begin);
            words.reserve(end - begin);
            mpSeg_.Cut(begin, end, words);

            vector<WordRange> hmmRes;
            hmmRes.reserve(end - begin);

            for (size_t i = 0; i < words.size(); i++) {
                // 如果 mp 得到一个词，把它放到结果里
                if (words[i].left != words[i].right || (words[i].left == words[i].right && mpSeg_.IsUserDictSingleChineseWord(words[i].left->rune))) {
                    res.push_back(words[i]);
                    continue;
                }

                // 如果 mp 得到一个单独的，并且它不在userdict中，则按顺序收集它
                size_t j = i;
                while (j < words.size() && words[j].left == words[j].right && !mpSeg_.IsUserDictSingleChineseWord(words[j].left->rune)) {
                    j++;
                }

                // 用hmm剪切序列
                assert(j - 1 >= i);
                hmmSeg_.Cut(words[i].left, words[j - 1].left + 1, hmmRes);
                // 把 hmm 的结果放入结果集
                for (size_t k = 0; k < hmmRes.size(); k++) {
                    res.push_back(hmmRes[k]);
                }

                // 清除临时变量
                hmmRes.clear();

                // 跳过,已经处理的地方
                i = j - 1;
            }
        }

        const DictTrie* GetDictTrie() const
        {
            return mpSeg_.GetDictTrie();
        }

        bool Tag(const string& src, vector<pair<string, string>>& res) const
        {
            return tagger_.Tag(src, res, *this);
        }

        string LookupTag(const string& str) const
        {
            return tagger_.LookupTag(str, *this);
        }
};

#endif