#include <stdlib.h>
#include "opt.h"

HashType hashtype = HASH_SUM;
bool opt[256] = { 0, };
const char* output_filename = 0;
const char* optarg_filename = 0;
const char* optarg_output = 0;
int optarg_jobs = 0;

void opt_elapsed(int argc, const char** argv) {
    opt['e'] = true;
}
void opt_jobs(int argc, const char** argv) {
    opt['j'] = true;
    optarg_jobs = atoi(argv[0]);
}
void opt_verbose(int argc, const char**argv) {
    opt['V'] = true;
}
void opt_version(int argc, const char**argv) {
    opt['v'] = true;
}
void opt_plain(int argc, const char **argv) {
    hashtype = PLAIN;
}
void opt_md5(int argc, const char **argv) {
    hashtype = HASH_MD5;
}
void opt_sha256(int argc, const char **argv) {
    hashtype = HASH_SHA256;
}
void opt_sum(int argc, const char **argv) {
    hashtype = HASH_SUM;
}
void opt_lower(int argc, const char **argv) {
    opt['l'] = true;
}
void opt_upper(int argc, const char **argv) {
    opt['u'] = true;
}
void opt_help(int argc, const char **argv) {
    opt['h'] = true;
}
void opt_output(int argc, const char** argv) {
    opt['o'] = true;
    output_filename = argv[0];
    optarg_output = argv[0];
}
void opt_recursive(int argc, const char** argv) {
    opt['r'] = true;
}
void opt_test(int argc, const char** argv) {
    opt['t'] = true;
}