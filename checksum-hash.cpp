#include "checksum.h"

using namespace std;

#ifdef __LINUX
#include <openssl/sha.h>
#include <openssl/md5.h>

string CheckSum::makeMD5(ifstream &file) {
    unsigned char hashstr[MD5_DIGEST_LENGTH] = { 0, };
    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        MD5_Update(&ctx, buffer, file.gcount());
    }    
    MD5_Final(hashstr, &ctx);

    return bytestostr(hashstr, MD5_DIGEST_LENGTH);
}

string CheckSum::makeSHA256(ifstream &file) {
    unsigned char hashstr[SHA256_DIGEST_LENGTH] = { 0, };
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        SHA256_Update(&ctx, buffer, file.gcount());
    }
    SHA256_Final(hashstr, &ctx);

    return bytestostr(hashstr, SHA256_DIGEST_LENGTH);
}
#endif
#ifdef __WINDOWS
#include <iostream>
#include <windows.h>

bool md5open(HCRYPTPROV& prov, HCRYPTHASH& hash);
bool md5close(HCRYPTPROV& prov, HCRYPTHASH& hash);

string CheckSum::makeMD5(ifstream &file) {
    string result = "";
    HCRYPTPROV prov = NULL;
    HCRYPTHASH hash = NULL;

    if (!md5open(prov, hash)) {
        return "";
    }

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        CryptHashData(hash, (BYTE*)buffer, file.gcount(), 0);
    }

    DWORD hashsize;
    DWORD bufsize = 0;
    CryptGetHashParam(hash, HP_HASHSIZE, (BYTE*)&hashsize, &bufsize, 0);

    unsigned char* md5Bytes = new unsigned char[hashsize];
    CryptGetHashParam(hash, HP_HASHVAL, (BYTE*)md5Bytes, &hashsize, 0);
    
    result = bytestostr(md5Bytes, hashsize);

    delete[] md5Bytes;

    md5close(prov, hash);
    return result;
}

bool md5open(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        md5close(prov, hash);
        return FALSE;
    }
    else if (!CryptCreateHash(prov, CALG_MD5, 0, 0, &hash)) {
        md5close(prov, hash);
        return FALSE;
    }
    else {
        return TRUE;
    }
}

bool md5close(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (prov != NULL) CryptReleaseContext(prov, 0);
    if (hash != NULL) CryptDestroyHash(hash);

    prov = NULL;
    hash = NULL;
    return TRUE;
}

bool sha256open(HCRYPTPROV& prov, HCRYPTHASH& hash);
bool sha256close(HCRYPTPROV& prov, HCRYPTHASH& hash);

string CheckSum::makeSHA256(ifstream &file) {
    string result = "";
    HCRYPTPROV prov = NULL;
    HCRYPTHASH hash = NULL;
    
    if (!sha256open(prov, hash)) {
        return "";
    }

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        CryptHashData(hash, (BYTE*)buffer, file.gcount(), 0);
    }

    DWORD hashsize;
    DWORD bufsize = 0;
    CryptGetHashParam(hash, HP_HASHSIZE, (BYTE*)&hashsize, &bufsize, 0);

    unsigned char* md5Bytes = new unsigned char[hashsize];
    CryptGetHashParam(hash, HP_HASHVAL, (BYTE*)md5Bytes, &hashsize, 0);
    
    result = bytestostr(md5Bytes, hashsize);

    delete[] md5Bytes;

    sha256close(prov, hash);
    return result;
}

bool sha256open(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        sha256close(prov, hash);
        return FALSE;
    }
    else if (!CryptCreateHash(prov, CALG_SHA_256, 0, 0, &hash)) {
        sha256close(prov, hash);
        return FALSE;
    }
    else {
        return TRUE;
    }
}

bool sha256close(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (prov != NULL) CryptReleaseContext(prov, 0);
    if (hash != NULL) CryptDestroyHash(hash);

    prov = NULL;
    hash = NULL;
    return TRUE;
}

#endif

string CheckSum::makeSum(ifstream &file) {
    int checksum = 0;

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        int len = file.gcount();

        for (int i=0; i < len; i++) {
            int ch = buffer[i];
            checksum = (checksum >> 1) + ((checksum & 1) << 15);
            checksum += ch;
            checksum &= 0xffff;
        }
    }

    return digittostr(checksum);
}
