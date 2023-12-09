#include <iostream>
#include "makehash.h"
#include "cargs.h"
#define VERSION "1.0"

using namespace std;

bool noopt = true;
bool optver = false;
bool opthelp = false;
bool optlower = false;
HashType hashtype = HASH_SUM;

static void help();
static void version();
static void opt_help(int, const char**);
static void opt_version(int, const char**);
static void opt_plain(int, const char**);
static void opt_md5(int, const char**);
static void opt_sha256(int, const char**);
static void opt_sum(int, const char**);
static void opt_lower(int, const char**);
static void handle_args(int, const char**);


int main(int argc, const char **argv) {
    struct cargs* cargs = cargs_init();
    cargs_option(cargs, "md5", 0, opt_md5, 1);
    cargs_option(cargs, "sha256", 0, opt_sha256, 1);
    cargs_option(cargs, "sum", 0, opt_sum, 1);
    cargs_option(cargs, "plain", 0, opt_plain, 1);
    cargs_option(cargs, "version", 0, opt_version, 1);
    cargs_option(cargs, "lower", 0, opt_lower, 1);
    cargs_args(cargs, handle_args);
    cargs_run(cargs, argc, argv);
    cargs_close(cargs);

    return 0;
}

void version() {
    cout << VERSION << endl;
}

void help() {
    cout << "Usage:" << endl;
    cout << "  checksum <target> <options...>" << endl;
    cout << "" << endl;
    cout << "Arguments:" << endl;
    cout << "  --sum    \tuse sum (default)" << endl;
    cout << "  --md5    \tuse MD5" << endl;
    cout << "  --sha256 \tuse SHA256" << endl;
    cout << "  --plain  \tuse no algorithm" << endl;
    cout << "  --lower  \tuse no algorithm" << endl;
    cout << "  -h       \tdisplay this help" << endl;
    cout << "  --version\toutput version infomation" << endl;
}

void opt_version(int argc, const char**argv) {
    optver = true;
    noopt = false;
}
void opt_plain(int argc, const char **argv) {
    hashtype = PLAIN;
    noopt = false;
}
void opt_md5(int argc, const char **argv) {
    hashtype = HASH_MD5;
    noopt = false;
}
void opt_sha256(int argc, const char **argv) {
    hashtype = HASH_SHA256;
    noopt = false;
}
void opt_sum(int argc, const char **argv) {
    hashtype = HASH_SUM;
    noopt = false;
}
void opt_lower(int argc, const char **argv) {
    optlower = true;
}
void opt_help(int argc, const char **argv) {
    opthelp = true;
    noopt = false;
}
void handle_args(int argc, const char **argv) {
    if (optver) {
        version();
        return;
    }
    else if (opthelp || argc != 1) {
        help();
        return;
    }

    const char* target = argv[0];
    MakeHash hash = MakeHash(hashtype);
    
    ifstream file(target, ifstream::binary);
    if (!file.is_open()) {
        cerr << "could not open : " << target << endl;
        return;
    }
    
    string result = hash.make(file);
    if (optlower) {
        for (char &c : result) {
            c = tolower(c);
        }
    }
    cout << result << endl;
}