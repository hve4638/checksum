#include <fstream>
#include <string>

enum HashType {
    PLAIN,
    HASH_MD5,
    HASH_SHA256,
    HASH_SUM,
};

class CheckSum {
    HashType m_hashtype;

    std::string makeMD5(std::ifstream &file);
    std::string makePlain(std::ifstream &file);
    std::string makeSHA256(std::ifstream &file);
    std::string makeSum(std::ifstream &file);
    
    std::string bintostr(unsigned char*, size_t);

    std::string digittostr(int);
    std::string bytestostr(unsigned char*, size_t);
public:
    CheckSum(HashType hashtype = HASH_SHA256);
    ~CheckSum();

    std::string make(std::ifstream &file);
};