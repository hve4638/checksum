#include <iostream>
#include <memory>
#include <filesystem>
#include "checksum.h"
#include "cargs.h"
#include "opt.h"
#define VERSION "1.1"
#define ENABLE_DEBUG (1)

using namespace std;
using namespace filesystem;

const char* output_file = NULL;
CheckSum checksum;

extern void help();
extern void version();

inline void show_openerr(const char* filename, const char* signiture) {
    cerr << "could not open : " << filename << " (" << signiture << ")" << endl;
}

void makehash_single(const char* target, ostream& output, bool showname=false);
// void makehash_multi(int, const char**, ostream&);
void makehash(istream& input, ostream& output, const char* name = NULL);
void makehash_recursive(const char* dirname, ostream& output);

int main(int argc, const char **argv) {
    struct cargs* cargs = cargs_init();
    cargs_option(cargs, "md5", 0, opt_md5, 1);
    cargs_option(cargs, "sha256", 0, opt_sha256, 1);
    cargs_option(cargs, "sum", 0, opt_sum, 1);
    // cargs_option(cargs, "plain", 0, opt_plain, 1);
    cargs_option(cargs, "version", 0, opt_version, 1);
    cargs_option(cargs, "h:help", 0, opt_help, 1);
    cargs_option(cargs, "lower", 0, opt_lower, 1);
    cargs_option(cargs, "r:recursive", 0, opt_recursive, 1);
    cargs_option(cargs, "o:output", 1, opt_output, 1);
    // cargs_option(cargs, "t:test", 0, opt_test, 1);
    cargs_args(cargs, handle_args);
    cargs_run(cargs, argc, argv);
    cargs_close(cargs);

    return 0;
}

void handle_args(int argc, const char **argv) {
    string result;
    checksum.sethashtype(hashtype);

    if (opt['v']) {
        version();
    }
    else if (opt['h']) {
        help();
    }
    else {
        bool need_to_free_output = false;
        ostream *output = NULL;

        if (opt['o']) {
            ofstream* file = new ofstream(output_filename);
            if (file->is_open()) {
                output = file;
                need_to_free_output = true;
            }
            else {
                show_openerr(output_filename, "exit");
                delete output;
                return;
            }
        }
        else {
            output = &cout;
            need_to_free_output = false;
        }
        
        if (opt['r']) { // recursive mode
            if (argc == 0) {
                help();
            }
            else {
                for (int i = 0; i < argc; i++) {
                    makehash_recursive(argv[i], *output);
                }
            }
        }
        else {
            if (argc == 0) {
                makehash(cin, *output);
            }
            else if (argc == 1) {
                makehash_single(argv[0], *output);
            }
            else {
                for (int i = 0; i < argc; i++) {
                    makehash_single(argv[i], *output, true);
                }
            }
        }

        if (need_to_free_output) {
            delete output;
        }
    }
}

void makehash_recursive(const char* dirname, ostream& output) {
    path dir = dirname;

    if (is_regular_file(dir)) {
        string str = dir.string();
        const char *ch = str.c_str();
        makehash_single(ch, output, true);
    }
    else if (is_directory(dir)){
        for (const auto& entry : recursive_directory_iterator(dir)) {
            if (is_regular_file(entry)) {
                string str = entry.path().string();
                const char *ch = str.c_str();

                makehash_single(ch, output, true);
            }
        }
    }
}

void makehash_single(const char* target, ostream& output, bool showname) {
    path file = target;
    ifstream input(target);
    
    if (is_directory(file)) {
        show_openerr(target, "skip");
    }
    else {
        if (input.is_open()) {
            const char* name = showname ? target : 0;
            makehash(input, output, name);
        }
        else {
            show_openerr(target, "skip");
        }
    }
}

void makehash(istream& input, ostream& output, const char* name) {
    string result = checksum.make(input);

    if (opt['l']) {
        for (char &c : result) {
            c = tolower(c);
        }
    }

    if (name == 0) {
        output << result << endl;
    }
    else {
        output << result << " " << name << endl;
    }

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
    cout << "  -r, --recursive \tcheck directory recursively" << endl;
    cout << "  -o, --output \tcheck directory recursively" << endl;
    cout << "  --help   \tdisplay this help" << endl;
    cout << "  --verbose\toutput version infomation" << endl;
    cout << "  --version\toutput version infomation" << endl;
}
