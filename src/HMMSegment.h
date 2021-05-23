#ifndef HMMSEGMENT_H
#define HMMSEGMENT_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <cassert>
#include "HMMModel.h"
#include "SegmentBase.h"

class HMMSegment: public SegmentBase
{
    private:
        const HMMModel *model_;
        bool isNeedDestroy_;
};

#endif