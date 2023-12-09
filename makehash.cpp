#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "makehash.h"

using namespace std;

struct word {
    char c[2];
};

static string int_to_str(int num) {
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

static struct word i_to_str(unsigned char c) {
    struct word word;
    int h = (c & (0xF0)) >> 4;
    int l = (c & (0x0F));
    
    word.c[0] = (h < 10) ? ('0' + h) : ('A' + h - 10);
    word.c[1] = (l < 10) ? ('0' + l) : ('A' + l - 10);

    return word;
}

MakeHash::MakeHash(HashType hashtype) {
    m_hashtype = hashtype;
}

MakeHash::~MakeHash() {
    
}

string MakeHash::make(ifstream &file) {
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

string MakeHash::makeMD5(ifstream &file) {
    unsigned char hashstr[MD5_DIGEST_LENGTH] = { 0, };
    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        MD5_Update(&ctx, buffer, file.gcount());
    }    
    MD5_Final(hashstr, &ctx);

    return bintostr(hashstr, MD5_DIGEST_LENGTH);
}

string MakeHash::makeSum(ifstream &file) {
    int checksum = 0;

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        size_t len = file.gcount();

        for (int i=0; i < len; i++) {
            int ch = buffer[i];
            checksum = (checksum >> 1) + ((checksum & 1) << 15);
            checksum += ch;
            checksum &= 0xffff;
        }
    }

    return int_to_str(checksum);
}

string MakeHash::makeSHA256(ifstream &file) {
    unsigned char hashstr[SHA256_DIGEST_LENGTH] = { 0, };
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        SHA256_Update(&ctx, buffer, file.gcount());
    }    
    SHA256_Final(hashstr, &ctx);

    return bintostr(hashstr, SHA256_DIGEST_LENGTH);
}

string MakeHash::makePlain(ifstream &file) {
    string plaintext = "";

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer)-1);
        buffer[file.gcount()] = '\0';

        plaintext += buffer;
    }

    return plaintext;
}

string MakeHash::bintostr(unsigned char* buffer, size_t length) {
    string result;

    for(size_t i=0; i < length; i++) {
        struct word word = i_to_str(buffer[i]);
        
        result += word.c[0];
        result += word.c[1];
    }
    return result;
}