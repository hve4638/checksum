#include "checksum.h"

#ifdef __LINUX
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;

string CheckSum::makeMD5(istream &file) {
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

#endif
#ifdef __WINDOWS
#define DWORD_MD5_LENGTH 16
#include <iostream>
#include <windows.h>

using namespace std;

static bool md5open(HCRYPTPROV& prov, HCRYPTHASH& hash);
static bool md5close(HCRYPTPROV& prov, HCRYPTHASH& hash);
static void md5work(istream &file, unsigned char* result);

string CheckSum::makeMD5(istream &file) {
    unsigned char hash[DWORD_MD5_LENGTH];

    md5work(file, hash);
    return bytestostr(hash, DWORD_MD5_LENGTH);
}

bool md5open(HCRYPTPROV& hProv, HCRYPTHASH& hHash) {
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        md5close(hProv, hHash);
        return false;
    }
    else if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        md5close(hProv, hHash);
        return false;
    }
    else {
        return true;
    }
}

bool md5close(HCRYPTPROV& hProv, HCRYPTHASH& hHash) {
    if (hHash != 0) CryptDestroyHash(hHash);
    if (hProv != 0) CryptReleaseContext(hProv, 0);

    hProv = 0;
    hHash = 0;
    return true;
}

void md5work(istream &file, unsigned char* result) {
    HCRYPTPROV prov = 0;
    HCRYPTHASH hash = 0;
    
    if (!md5open(prov, hash)) {
        result[0] = 0;
    }
    else {
        char buffer[1024] = {0,};
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            CryptHashData(hash, (BYTE*)buffer, file.gcount(), 0);
        }

        DWORD hashsize = DWORD_MD5_LENGTH;
        CryptGetHashParam(hash, HP_HASHVAL, (BYTE*)result, &hashsize, 0);
        
        md5close(prov, hash);
    }
}
#endif
