#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "checksum.h"

using namespace std;

CheckSum::CheckSum(HashType hashtype) {
    m_hashtype = hashtype;
}

CheckSum::~CheckSum() {
    
}

void CheckSum::sethashtype(HashType hashtype) {
    m_hashtype = hashtype;
}


string CheckSum::make(istream &file) {
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

string CheckSum::makePlain(istream &file) {
    string plaintext = "";

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer)-1);
        buffer[file.gcount()] = '\0';

        plaintext += buffer;
    }

    return plaintext;
}

string CheckSum::bytestostr(unsigned char* buf, size_t length) {
    string result;
    static char digits[] = "0123456789abcdef";
    
    for(size_t i = 0; i < length; i++) {
        result += digits[buf[i] >> 4];
        result += digits[buf[i] & 0xf];
    }
    return result;
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