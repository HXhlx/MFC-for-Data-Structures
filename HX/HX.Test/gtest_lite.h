#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>

namespace testing {

class Test {
public:
    virtual ~Test() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
    virtual void Run() = 0;
    virtual std::string GetName() const = 0;
};

class TestRegistry {
public:
    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

    void Register(Test* test) {
        tests_.push_back(test);
    }

    int RunAll() {
        int passed = 0;
        int failed = 0;
        int total = 0;

        for (auto* test : tests_) {
            total++;
            std::cout << "[RUN      ] " << test->GetName() << std::endl;
            try {
                test->SetUp();
                test->Run();
                test->TearDown();
                std::cout << "[       OK ] " << test->GetName() << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[  FAILED  ] " << test->GetName() << std::endl;
                std::cout << "           Error: " << e.what() << std::endl;
                failed++;
            }
        }

        std::cout << std::endl;
        std::cout << "[==========] " << total << " tests ran." << std::endl;
        std::cout << "[  PASSED  ] " << passed << " tests." << std::endl;
        if (failed > 0) {
            std::cout << "[  FAILED  ] " << failed << " tests." << std::endl;
        }

        return failed;
    }

private:
    std::vector<Test*> tests_;
};

} // namespace testing

#define TEST_F(TestFixture, TestName) \
    class TestFixture##_##TestName : public TestFixture { \
    public: \
        std::string GetName() const override { \
            return #TestFixture "." #TestName; \
        } \
        void Run() override; \
    }; \
    static bool registered_##TestFixture##_##TestName = []() { \
        static TestFixture##_##TestName instance; \
        testing::TestRegistry::Instance().Register(&instance); \
        return true; \
    }(); \
    void TestFixture##_##TestName::Run()

#define TEST(TestSuite, TestName) \
    class TestSuite##_##TestName : public testing::Test { \
    public: \
        std::string GetName() const override { \
            return #TestSuite "." #TestName; \
        } \
        void Run() override; \
    }; \
    static bool registered_##TestSuite##_##TestName = []() { \
        static TestSuite##_##TestName instance; \
        testing::TestRegistry::Instance().Register(&instance); \
        return true; \
    }(); \
    void TestSuite##_##TestName::Run()

#include <ostream>

// Helper to output wstring to ostream
inline std::ostream& operator<<(std::ostream& os, const std::wstring& s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return os << converter.to_bytes(s);
}

#define EXPECT_EQ(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a != _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " == " << #b; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_NE(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a == _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " != " << #b \
                << " but both are " << _a; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_TRUE(expr) \
    do { \
        if (!(expr)) { \
            std::ostringstream oss; \
            oss << "Expected " << #expr << " to be true"; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_FALSE(expr) \
    do { \
        if ((expr)) { \
            std::ostringstream oss; \
            oss << "Expected " << #expr << " to be false"; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_GT(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a <= _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " > " << #b \
                << " but got " << _a << " vs " << _b; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_GE(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a < _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " >= " << #b \
                << " but got " << _a << " vs " << _b; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_LT(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a >= _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " < " << #b \
                << " but got " << _a << " vs " << _b; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define EXPECT_LE(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (_a > _b) { \
            std::ostringstream oss; \
            oss << "Expected " << #a << " <= " << #b \
                << " but got " << _a << " vs " << _b; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define ASSERT_EQ EXPECT_EQ
#define ASSERT_NE EXPECT_NE
#define ASSERT_TRUE EXPECT_TRUE
#define ASSERT_FALSE EXPECT_FALSE
#define ASSERT_GT EXPECT_GT
#define ASSERT_GE EXPECT_GE
#define ASSERT_LT EXPECT_LT
#define ASSERT_LE EXPECT_LE
