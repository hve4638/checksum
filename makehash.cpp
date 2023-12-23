#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <tuple>
#include "makehash.h"
#include "checksum.h"
#include "unittest/printbuffer.h"
#define TRACEPRINT(X) cout << "[" << __FILE__ << ":" << __LINE__ << "] " << X << endl;
#define VERBOSE_OUT(X) if (m_option.verbose) \
                        cerr << X << endl

using namespace std;
namespace fs = std::filesystem;

void err_open(string filename) {
    cerr << "could not open : " << filename << endl;
}
inline void err_open(string filename, string reason) {
    cerr << "could not open : " << filename << " (" << reason << ")" << endl;
}

MakeHash::MakeHash(CheckSum &checksum) : m_checksum(checksum) {
    m_output = &cout;
}
MakeHash::~MakeHash() {

}

MakeHash& MakeHash::setOption(MakeHashOption option) {
    m_option = option;
    VERBOSE_OUT("[MakeHashOption]");
    VERBOSE_OUT("  verbose: " << m_option.verbose);
    VERBOSE_OUT("  lowercase: " << m_option.lower);
    VERBOSE_OUT("  showFilename: " << m_option.showFilename);
    VERBOSE_OUT("  useThreading: " << m_option.useThreading);
    VERBOSE_OUT("  maxinumThread: " << m_option.maxinumThread);
    return *this;
}
MakeHash& MakeHash::setOutput(std::ostream &out) {
    m_output = &out;
    return *this;
}

bool MakeHash::verbose() {
    return m_option.verbose;
}

void MakeHash::hashStdin() {
    make(cin, "");
}
void MakeHash::hashFile(std::filesystem::path path) {
    ifstream input(path, std::ios::binary);

    if (!fs::is_regular_file(path)) {
        err_open(path.string(), "skip");
    }
    else if (!input.is_open()) {
        err_open(path.string(), "skip");
    }
    else {
        make(input, path.string());
    }
}
void MakeHash::hashDir(std::filesystem::path path) {
    if (fs::is_regular_file(path)) {
        hashFile(path);
    }
    else if (fs::is_directory(path)) {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
                hashFile(entry.path());
            }
        }
    }
}

void MakeHash::make(istream& input, string name) {
    string result = m_checksum.make(input);
    if (m_option.lower) {
        for (char &c : result) {
            c = tolower(c);
        }
    }

    if (m_option.showFilename && name.size() != 0) {
        *m_output << result << " " << name << endl;
    }
    else {
        *m_output << result << endl;
    }
}