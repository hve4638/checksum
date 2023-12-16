#include "checksum.h"

#ifdef __LINUX
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;

string CheckSum::makeSHA256(istream &file) {
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
#define DWORD_SHA256_LENGTH 32
#include <iostream>
#include <windows.h>

using namespace std;

static bool sha256open(HCRYPTPROV& prov, HCRYPTHASH& hash);
static bool sha256close(HCRYPTPROV& prov, HCRYPTHASH& hash);
static void sha256work(istream &file, unsigned char* result);

string CheckSum::makeSHA256(istream &file) {
    unsigned char hash[DWORD_SHA256_LENGTH];
    
    sha256work(file, hash);
    return bytestostr(hash, DWORD_SHA256_LENGTH);
}

bool sha256open(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (!CryptAcquireContext(&prov, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        sha256close(prov, hash);
        return false;
    }
    else if (!CryptCreateHash(prov, CALG_SHA_256, 0, 0, &hash)) {
        sha256close(prov, hash);
        return false;
    }
    else {
        return true;
    }
}

bool sha256close(HCRYPTPROV& prov, HCRYPTHASH& hash) {
    if (prov != 0) CryptReleaseContext(prov, 0);
    if (hash != 0) CryptDestroyHash(hash);

    prov = 0;
    hash = 0;
    return true;
}

void sha256work(istream &file, unsigned char* result) {
    HCRYPTPROV prov = 0;
    HCRYPTHASH hash = 0;
    
    if (!sha256open(prov, hash)) {
        result[0] = 0;
    }
    else {
        char buffer[1024];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            CryptHashData(hash, (BYTE*)buffer, file.gcount(), 0);
        }

        DWORD hashsize = DWORD_SHA256_LENGTH;
        CryptGetHashParam(hash, HP_HASHVAL, (BYTE*)result, &hashsize, 0);
    }
}

#endif