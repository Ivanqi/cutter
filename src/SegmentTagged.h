#ifndef SEGMENTTAGGED_H
#define SEGMENTTAGGED_H

#include "DictTrie.h"
#include "SegmentBase.h"

class SegmentTagged: public SegmentBase
{
    public:
        SegmentTagged()
        {

        }

        virtual ~SegmentTagged()
        {

        }

        virtual bool Tag(const std::string& src, std::vector<std::string>& words) const = 0;

        virtual const DictTrie* GetDictTrie() const = 0;
};

#endif