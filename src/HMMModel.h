#ifndef HMMMODEL_H
#define HMMMODEL_H

#include "StringUtil.h"
#include "Trie.h"

typedef unordered_map<Rune, double> EmitProbMap;

struct HMMModel
{
    /*
     * STATUS:
     * 0: HMMModel::B, 1: HMMModel::E, 2: HMMModel::M, 3:HMMModel::S
     * */
    enum {
        B = 0, E = 1, M = 2, S = 3, STATUS_SUM = 4
    };

    char statMap[STATUS_SUM];
    double startProb[STATUS_SUM];
    double transProb[STATUS_SUM][STATUS_SUM];

    EmitProbMap emitProbB;
    EmitProbMap emitProbE;
    EmitProbMap emitProbM;
    EmitProbMap emitProbS;

    vector<EmitProbMap*> emitProbVec;

    HMMModel(const string& modelPath)
    {
        memset(startProb, 0, sizeof(startProb));
        memset(transProb, 0, sizeof(transProb));

        statMap[0] = 'B';
        statMap[1] = 'E';
        statMap[2] = 'M';
        statMap[3] = 'S';

        emitProbVec.push_back(&emitProbB);
        emitProbVec.push_back(&emitProbE);
        emitProbVec.push_back(&emitProbM);
        emitProbVec.push_back(&emitProbS);

        LoadModel(modelPath);
    }

    ~HMMModel()
    {

    }

    void LoadModel(const string& filePath)
    {
        ifstream ifile(filePath.c_str());
        string line;
        vector<string> tmp;
        vector<string> tmp2;

        GetLine(ifile, line);
        split(line, tmp, " ");

        for (size_t j = 0; j < tmp.size(); j++) {
            startProb[j] = atof(tmp[j].c_str());
        }

        // 加载 transProb
        for (size_t i = 0; i < STATUS_SUM; i++) {
            GetLine(ifile, line);
            split(line, tmp, " ");
            for (size_t j = 0; j < STATUS_SUM; j++) {
                transProb[i][j] = atof(tmp[j].c_str());
            }
        }

        // 加载 emitProbB
        GetLine(ifile, line);
        LoadEmitProb(line, emitProbB);

        // 加载 emitProbE
        GetLine(ifile, line);
        LoadEmitProb(line, emitProbE);

        // 加载 emitProbM
        GetLine(ifile, line);
        LoadEmitProb(line, emitProbM);

        // 加载 emitProbS
        GetLine(ifile, line);
        LoadEmitProb(line, emitProbS);
    }

    double GetEmitProb(const EmitProbMap *ptMap, Rune key, double defVal) const
    {
        EmitProbMap::const_iterator cit = ptMap->find(key);
        if (cit == ptMap->end()) {
            return defVal;
        }

        return cit->second;
    }

    bool GetLine(ifstream& ifile, string& line)
    {
        while (std::getline(ifile, line)) {
            Trim(line);
            if (line.empty()) {
                continue;
            }

            if (StartsWith(line, "#")) {
                continue;
            }

            return true;
        }

        return false;
    }

    bool LoadEmitProb(const string& line, EmitProbMap& mp)
    {
        if (line.empty()) {
            return false;
        }

        vector<string> tmp, tmp2;
        Unicode unicode;
        split(line, tmp, ",");

        for (size_t i = 0; i < tmp.size(); i++) {
            split(tmp[i], tmp2, ":");
            if (2 != tmp2.size()) {
                return false;
            }

            if (!DecodeRunesInString(tmp2[0], unicode) || unicode.size() != 1) {
                return false;
            }

            mp[unicode[0]] = atof(tmp2[1].c_str());
        }

        return true;
    }
};

#endif