#include "checksum.h"

extern HashType hashtype;
extern bool opt[256];
extern const char* output_filename;
extern const char* optarg_output;
extern int optarg_jobs;

void help();
void version();

void opt_help(int, const char**);
void opt_version(int, const char**);
void opt_verbose(int, const char**);
void opt_plain(int, const char**);
void opt_md5(int, const char**);
void opt_sha256(int, const char**);
void opt_sum(int, const char**);
void opt_jobs(int, const char**);
void opt_lower(int, const char**);
void opt_upper(int, const char **);
void opt_recursive(int, const char**);
void opt_output(int, const char**);
void opt_test(int, const char**);
void opt_elapsed(int argc, const char** argv);

void handle_args(int, const char**);
