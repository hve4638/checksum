#include <iostream>
#include <filesystem>
#include "checksum.h"

using namespace std;
using namespace filesystem;

void sd(const path& directory) {
    
}

void test() {
    ifstream input("./__test");

    cout << input.is_open() << endl;
}