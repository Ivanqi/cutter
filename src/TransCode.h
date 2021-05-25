#ifndef TRANSCODE_H
#define TRANSCODE_H

#include "LocalVector.h"
#include "StringUtil.h"

typedef uint16_t UnicodeValueType;
typedef LocalVector<UnicodeValueType> Unicodes;

namespace TransCode
{
    inline Unicodes decode(const std::string& str)
    {
        Unicodes ret;
        utf8ToUnicode(str, ret);
        return ret;
    }

    inline void encode(Unicodes::const_iterator begin, Unicodes::const_iterator end, std::string& res) 
    {
        unicodeToUtf8(begin, end, res);
    }
};

#endif