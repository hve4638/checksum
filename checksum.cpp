#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "checksum.h"

using namespace std;

static void splithex(unsigned char c, char& h, char& l);


struct word {
    char c[2];
};



CheckSum::CheckSum(HashType hashtype) {
    m_hashtype = hashtype;
}

CheckSum::~CheckSum() {
    
}

string CheckSum::make(ifstream &file) {
    switch(m_hashtype) {
    case HASH_SHA256:
        return makeSHA256(file);
    case HASH_MD5:
        return makeMD5(file);
    case HASH_SUM:
        return makeSum(file);
    case PLAIN:
    default:
        return makePlain(file);
    }
}

string CheckSum::makePlain(ifstream &file) {
    string plaintext = "";

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer)-1);
        buffer[file.gcount()] = '\0';

        plaintext += buffer;
    }

    return plaintext;
}

string CheckSum::bytestostr(unsigned char* buffer, size_t length) {
    char h, l;
    string result = "";

    for (size_t i = 0; i < length; i++) {
        splithex(buffer[i], h, l);

        result += h;
        result += l;
    }

    return result;
}

void splithex(unsigned char c, char& h, char& l) {
    int _h = (c & (0xF0)) >> 4;
    int _l = (c & (0x0F));

    h = (_h < 10) ? ('0' + _h) : ('A' + _h - 10);
    l = (_l < 10) ? ('0' + _l) : ('A' + _l - 10);
}

string CheckSum::digittostr(int num) {
    string result;
    bool isneg = false;
    
    if (num == 0) {
        return "0";
    }
    else if (num < 0) {
        isneg = true;
        num = -num;
    }
    
    while (num > 0) {
        char digit = (num % 10) + '0';
        result = digit + result;
        num /= 10;
    }
    
    if (isneg) result = '-' + result;

    return result;
}