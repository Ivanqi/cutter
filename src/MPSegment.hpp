#include MPSEGMENT_H
#define MPSEGMENT_H

#include <algorithm>
#include <set>
#include <cassert>
#include "DictTrie.h"
#include "SegmentTagged.h"

class MPSegment
{
    private:
        const DictTrie* dictTrie_;
        bool isNeedDestroy_;
        
};

#endif