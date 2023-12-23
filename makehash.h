#ifndef __MAKEHASH_H
#define __MAKEHASH_H
#include <iostream>
#include <fstream>
#include "checksum.h"

struct MakeHashOption {
    bool lower;
    bool showFilename;
    bool verbose;
    bool useThreading;
    int maxinumThread;
};

class MakeHash {
    CheckSum &m_checksum;
    std::ostream *m_output;
    MakeHashOption m_option;
public:
    MakeHash(CheckSum &checksum);
    ~MakeHash();

    MakeHash& setOption(MakeHashOption option);
    MakeHash& setOutput(std::ostream &out);
    void hashStdin();
    void hashFile(std::filesystem::path filename);
    void hashDir(std::filesystem::path dirname);
private:
    void make(std::istream& input, std::string name);
    bool verbose();
};
#endif