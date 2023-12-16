#include <iostream>
#include <filesystem>
#include "checksum.h"
#include "unity/unity.h"

using namespace std;
using namespace filesystem;

void test_sha256(void) {
    std::stringstream ss;
    ss << "hello world";

    string expected = "B94D27B9934D3E08A52E52D7DA7DABFAC484EFE37A5380EE9088F7ACE2EFCDE9";
    string actual;
    
    CheckSum checksum(HASH_SHA256);
    actual = checksum.make(ss);
    
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), actual.c_str());
}
void test_md5(void) {
    std::stringstream ss;
    ss << "hello world";
    string expected = "5EB63BBBE01EEED093CB22BB8F5ACDC3";
    string actual;
    
    CheckSum checksum(HASH_MD5);
    actual = checksum.make(ss);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), actual.c_str());
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_md5);
    RUN_TEST(test_sha256);
    
    return UNITY_END();
}

/*
    test api : https://github.com/ThrowTheSwitch/Unity/blob/master/README.md

TEST_ASSERT_EQUAL_STRING(expected, actual)
TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len)
TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message)
TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(expected, actual, len, message)
*/