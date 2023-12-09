#include <fstream>
#include <string>

enum HashType {
    PLAIN,
    HASH_MD5,
    HASH_SHA256,
    HASH_SUM,
};

class MakeHash {
    HashType m_hashtype;

    std::string makeMD5(std::ifstream &file);
    std::string makePlain(std::ifstream &file);
    std::string makeSHA256(std::ifstream &file);
    std::string makeSum(std::ifstream &file);
    std::string bintostr(unsigned char*, size_t);
public:
    MakeHash(HashType hashtype = HASH_SHA256);
    ~MakeHash();

    std::string make(std::ifstream &file);
};