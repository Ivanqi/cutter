#ifndef SEGMENTBASE_H
#define SEGMENTBASE_H

#include <unordered_set>
#include "PreFilter.h"
#include "Unicode.h"
#include <string>

const char* const SPECIAL_SEPARATORS = " \t\n\xEF\xBC\x8C\xE3\x80\x82";

class SegmentBase
{
    protected:
        unordered_set<Rune> symbols_;

    public:
        SegmentBase()
        {
            ResetSeparators(SPECIAL_SEPARATORS);
        }

        virtual ~SegmentBase()
        {

        }

        virtual void Cut(const std::string& sentence, vector<std::string>& words) const = 0;

        bool ResetSeparators(const string& s)
        {
            symbols_.clear();
            RuneStrArray runes;

            if (!DecodeRunesInString(s, runes)) {
                return false;
            }

            for (size_t i = 0; i < runes.size(); i++) {
                if (!symbols_.insert(runes[i].rune).second) {
                    return false;
                }
            }

            return true;
        }
};

#endif