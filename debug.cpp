#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <filesystem>
#include <locale.h>
#include <tuple>
#include "unittest/printbuffer.h"
#include "makehash.h"
#include "checksum.h"

using namespace std;
namespace fs = std::filesystem;

int main() {
    CheckSum checksum;
    checksum.sethashtype(HASH_MD5);

    MakeHashOption option;
    option.lower = true;
    option.showFilename = true;

    MakeHash makehash(checksum);
    makehash.setOutput(cout);
    makehash.setOption(option);

    makehash.hashDir("D:\\Memo");


}


int amain() {
    string dirname = "D:\\Memo";
    fs::path dir = dirname;
    fs::recursive_directory_iterator diriter(dir);
    fs::recursive_directory_iterator fsend;
    

    return 0;
    while (diriter != fsend) {
        wifstream file;
        fs::path entry = *diriter;
        wstring str;
        str = entry.wstring();
        
        file.open(str.c_str());
        cout << file.is_open() << endl;
        file.close();
        wcout << str << endl;

        ++diriter;
    }

    cout <<"End" << endl;
    return 0;
}