#include "checksum.h"

extern HashType hashtype;
extern bool opt[256];
extern const char* output_filename;

void help();
void version();
void opt_help(int, const char**);
void opt_version(int, const char**);
void opt_plain(int, const char**);
void opt_md5(int, const char**);
void opt_sha256(int, const char**);
void opt_sum(int, const char**);
void opt_lower(int, const char**);
void opt_recursive(int, const char**);
void opt_output(int, const char**);
void opt_test(int, const char**);
void handle_args(int, const char**);