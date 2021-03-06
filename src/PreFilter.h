#ifndef PREFILTER_H
#define PREFILTER_H

#include "Trie.h"

class PreFilter
{
    private:
        RuneStrArray::const_iterator cursor_;
        RuneStrArray sentence_;
        const unordered_set<Rune>& symbols_;

    public:
        struct Range
        {
            RuneStrArray::const_iterator begin;
            RuneStrArray::const_iterator end;
        };

        PreFilter(const unordered_set<Rune>& symbols, const string& sentence)
            : symbols_(symbols)
        {
            if (!DecodeRunesInString(sentence, sentence_)) {

            }

            cursor_ = sentence_.begin();
        }

        ~PreFilter()
        {

        }

        bool HasNext() const
        {
            return cursor_ != sentence_.end();
        }

        /**
         * 返回符合symbols_标志的首尾地址
         */
        Range Next()
        {
            Range range;
            range.begin = cursor_;

            while (cursor_ != sentence_.end()) {
                // 查询与symbols_ 相同的unicode 编码
                if (symbols_.find(cursor_->rune) != symbols_.end()) {
                    if (range.begin == cursor_) {
                        cursor_++;
                    }
                    range.end = cursor_;
                    return range;
                }
                cursor_++;
            }

            range.end = sentence_.end();
            return range;
        }

};

#endif