#include "checksum.h"

using namespace std;

string CheckSum::makeSum(istream &file) {
    int checksum = 0;

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        int len = file.gcount();

        for (int i=0; i < len; i++) {
            int ch = buffer[i];
            checksum = (checksum >> 1) + ((checksum & 1) << 15);
            checksum += ch;
            checksum &= 0xffff;
        }
    }

    return digittostr(checksum);
}
