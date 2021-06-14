#ifndef MPSEGMENT_H
#define MPSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "DictTrie.h"
#include "SegmentTagged.h"
#include "PosTagger.h"

/**
 * (Maximum Probability)最大概率法
 * 负责根据Trie树构建有向无环图进行动态规划算法，是分词算法的核心
 * 
 * 构造一个前缀词典，然后利用前缀词典对输入句子进行切分，得到所有的切分位置，根据切分位置，构造一个有向无环图
 * 通过动态规划算法，计算得到最大概率路径，也就得到了最终的切分形式
 */
class MPSegment: public SegmentTagged
{
    private:
        const DictTrie* dictTrie_;
        bool isNeedDestroy_;
        PosTagger tagger_;
    
    public:
        MPSegment(const string& dictPath, const string& userDictPath = "")
            : dictTrie_(new DictTrie(dictPath, userDictPath)), isNeedDestroy_(true)
        {
            
        }

        MPSegment(const DictTrie *dictTrie)
            : dictTrie_(dictTrie), isNeedDestroy_(false)
        {
            assert(dictTrie_);
        }

        ~MPSegment()
        {
            if (isNeedDestroy_) {
                delete dictTrie_;
            }
        }

        void Cut(const string& sentence, vector<string>& words) const
        {
            Cut(sentence, words, MAX_WORD_LENGTH);
        }

        void Cut(const string& sentence, vector<string>& words, size_t max_word_len) const
        {
            vector<Word> tmp;
            Cut(sentence, tmp, max_word_len);
            GetStringsFromWords(tmp, words);
        }

        void Cut(const string& sentence, vector<Word>& words, size_t max_word_len = MAX_WORD_LENGTH) const
        {
            PreFilter pre_filter(symbols_, sentence);
            PreFilter::Range range;
            vector<WordRange> wrs;

            wrs.reserve(sentence.size() / 2);

            while (pre_filter.HasNext()) {
                range = pre_filter.Next();
                Cut(range.begin, range.end, wrs, max_word_len);
            }

            words.clear();
            words.reserve(wrs.size());
            GetWordsFromWordRanges(sentence, wrs, words);
        }

        void Cut(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<WordRange>& words, size_t max_word_len = MAX_WORD_LENGTH) const
        {
            vector<Dag> dags;
            // 通过前缀树构建候选词
            dictTrie_->Find(begin, end, dags, max_word_len);

            CalcDP(dags);
            CutByDag(begin, end, dags, words);
        }

        const DictTrie* GetDictTrie() const
        {
            return dictTrie_;
        }

        bool Tag(const string& src, vector<pair<string, string>>& res) const
        {
            return tagger_.Tag(src, res, *this);
        }

        bool IsUserDictSingleChineseWord(const Rune& value) const
        {
            return dictTrie_->IsUserDictSingleChineseWord(value);
        }

    private:
        /**
         * 从后往前的动态规划算法
         * 例子:
         *  候选词
         *      国民党当局破坏(不为NULL的情况)
         *          国 {民, 党, 当, 局}
         *          民 {民, 党, 当, 局, 破}
         *          党 {当, 局, 破, 坏}
         *          当 {局, 破, 坏}
         *          局 {破, 坏}
         *          破 {坏}
         *          坏 {}
         */
        void CalcDP(vector<Dag>& dags) const
        {
            size_t nextPos;
            const DictUnit *p;
            double val;

            // 后趋词
            for (vector<Dag>::reverse_iterator rit = dags.rbegin(); rit != dags.rend(); rit++) {
                rit->pInfo = NULL;
                rit->weight = MIN_DOUBLE;
                assert(!rit->nexts.empty());

                for (LocalVector<pair<size_t, const DictUnit*>>::const_iterator it = rit->nexts.begin(); it != rit->nexts.end(); it++) {
                    nextPos = it->first;
                    p = it->second;
                    val = 0.0;

                    // 在dags中找后一个单词的权重，寻找它的后趋词
                    if (nextPos + 1 < dags.size()) {
                        val += dags[nextPos + 1].weight;
                    }

                    // 当前词的权重
                    if (p) {
                        val += p->weight;
                    } else {
                        val += dictTrie_->GetMinWeight();
                    }

                    // 如果后一个词的权重 + 当前词的权重大于 rit词的权重，更新rit词的权重。这样就得到最佳的权重的词
                    if (val > rit->weight) {
                        rit->pInfo = p;
                        rit->weight = val;
                    }
                }
            }
        }

        // 选择dags的候选词的权重得到相关的最大概率的词组，然后记录到words中
        void CutByDag(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, const vector<Dag>& dags, vector<WordRange>& words) const
        {
            size_t i = 0;
            while (i < dags.size()) {
                const DictUnit *p = dags[i].pInfo;
                if (p) {
                    assert(p->word.size() >= 1);
                    WordRange wr(begin + i, begin + i + p->word.size() - 1);
                    words.push_back(wr);
                    i += p->word.size();
                } else {    // single chinese word
                    WordRange wr(begin + i, begin + i);
                    words.push_back(wr);
                    i++;
                }
            }
        }
};

#endif