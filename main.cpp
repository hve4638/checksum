#ifndef PROGRAM_NAME
#define PROGRAM_NAME "Checksum"
#endif
#ifndef VERSION
#define VERSION "0.0.0"
#endif
#define TRACE() cout << "[" << __FILE__ << ":" << __LINE__ << "] " << endl

#include <iostream>
#include <memory>
#include <locale>
#include <thread>
#include <filesystem>
#include <cstring>
#include <ctime>
#include "makehash.h"
#include "checksum.h"
#include "cargs.h"
#include "opt.h"

using namespace std;
using namespace filesystem;

class Clock {
    clock_t m_begin = 0, m_end = 0;
public:
    void start() {
        m_begin = clock();
    }
    void stop() {
        m_end = clock();
    }
    clock_t duration() {
        return m_end - m_begin;
    }
};

CheckSum checksum;

void help();
void version();

ostream* setup_output(const char*);
void teardown_output(ostream*);
MakeHashOption make_option();

void printbuffer(void* buffer, size_t length, size_t interval);

inline void show_openerr(string filename, const char* signiture) {
    cerr << "could not open : " << filename << " (" << signiture << ")" << endl;
}

int main(int argc, const char **argv) {
    setlocale(LC_ALL, "");

    struct cargs* cargs = cargs_init();
    cargs_option(cargs, "md5", 0, opt_md5, 1);
    cargs_option(cargs, "sha256", 0, opt_sha256, 1);
    cargs_option(cargs, "sum", 0, opt_sum, 1);
    // cargs_option(cargs, "plain", 0, opt_plain, 1);
    cargs_option(cargs, "verbose", 0, opt_verbose, 1);
    cargs_option(cargs, "version", 0, opt_version, 1);
    cargs_option(cargs, "h:help", 0, opt_help, 1);
    cargs_option(cargs, "j:jobs", 1, opt_jobs, 1);
    cargs_option(cargs, "l:lower", 0, opt_lower, 1);
    cargs_option(cargs, "e:elapsed", 0, opt_elapsed, 1);
    cargs_option(cargs, "r:recursive", 0, opt_recursive, 1);
    cargs_option(cargs, "o:output", 1, opt_output, 1);
    // cargs_option(cargs, "t:test", 0, opt_test, 1);
    cargs_args(cargs, handle_args);
    cargs_run(cargs, argc, argv);
    cargs_close(cargs);

    return 0;
}

void handle_args(int argc, const char **argv) {
    Clock clock;
    string result;
    checksum.sethashtype(hashtype);
    
    if (opt['v']) {
        version();
        return;
    }
    else if (opt['h']) {
        help();
        return;
    }

    ostream *output = setup_output(optarg_output);
    if (output == nullptr) {
        show_openerr(optarg_output, "exit");
        return;
    }

    MakeHash makehash(checksum);
    MakeHashOption option = make_option();
    makehash.setOutput(*output);

    clock.start();
    if (opt['r']) { // recursive mode
        if (argc == 0) {
            help();
        }
        else {
            option.showFilename = true;
            makehash.setOption(option);

            for (int i = 0; i < argc; i++) {
                makehash.hashDir(argv[i]); 
            }
        }
    }
    else {
        if (argc == 0) {
            makehash.setOption(option);
            makehash.hashStdin();
        }
        else if (argc == 1) {
            makehash.setOption(option);
            makehash.hashFile(argv[0]);
        }
        else {
            option.showFilename = true;
            makehash.setOption(option);

            for (int i = 0; i < argc; i++) {
                makehash.hashFile(argv[0]);
            }
        }
    }

    teardown_output(output);
    clock.stop();

    if (opt['e']) {
        auto duration = clock.duration();
        cerr << endl;
        cerr << "Elapsed time : " << (double)duration/1000 << "s" << endl;
    }
}

void version() {
    cout << PROGRAM_NAME << " " << VERSION << endl;
}

void help() {
    cout << "Usage:" << endl;
    cout << "  checksum <target> <options...>" << endl;
    cout << "" << endl;
    cout << "Arguments:" << endl;
    
    cout << "  --sum           \tuse sum (default)" << endl;
    cout << "  --md5           \tuse MD5" << endl;
    cout << "  --sha256        \tuse SHA256" << endl;
    cout << "  -u, --upper     \toutput in uppercase" << endl;
    cout << "  -l, --lower     \toutput in lowercase" << endl;
    cout << "  -r, --recursive \tcheck directory recursively" << endl;
    cout << "  -e, --elapsed   \tdisplay execution time (stderr)" << endl;
    cout << "  -o, --output    \toutput as file" << endl;
    cout << "  --help          \tdisplay this help" << endl;
    cout << "  --verbose       \tverbose mode" << endl;
    cout << "  --version       \toutput version infomation" << endl;
}

ostream* setup_output(const char* filename) {
    if (opt['o']) {
        ofstream* file = new ofstream(filename);
        if (file->is_open()) {
            return file;
        }
        else {
            file->close();
            delete file;
            return nullptr;
        }
    }
    else {
        return &cout;
    }
}

void teardown_output(ostream* output) {
    if (output != &cout) {
        ofstream* file = reinterpret_cast<ofstream*>(file);
        file->close();
        delete output;
    }
}

MakeHashOption make_option() {
    MakeHashOption option = { 0, };
    if (opt['l']) {
        option.lower = true;
    }
    if (opt['V']) {
        option.verbose = true;
    }
    if (opt['j']) {
        if (optarg_jobs == 1) {
            option.useThreading = false;
            option.maxinumThread = 1;
        }
        else if (optarg_jobs == 0) {
            option.useThreading = true;
            option.maxinumThread = thread::hardware_concurrency();
        }
        else {
            option.useThreading = true;
            option.maxinumThread = optarg_jobs;
        }
    }
    
    return option;
}

void printbuffer(void* buffer, size_t length, size_t interval) {
	const unsigned char* buf = (const unsigned char*)buffer;

	printf("        \t");
	for (size_t i = 0; i < interval; i++) {
		printf("%2X ", (int)i);
	}
	printf("\n");

	for (size_t pos = 0; pos < length; pos += interval) {
		printf("%08X\t", (int)pos);

		for (size_t i = 0; i < interval; i++) {
			if (pos + i < length) printf("%02X ", buf[pos + i]);
			else printf("   ");
		}
		printf("\t");
		for (size_t i = 0; i < interval; i++) {
			if (pos + i >= length) break;
			size_t ch = buf[pos + i];

			if (ch >= 0x21 && ch <= 0x7E) { // printable char
				printf("%c", (int)ch);
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}
}