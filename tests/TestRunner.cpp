
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <exception>

// Simple test framework
class TestFramework {
private:
    struct Test {
        std::string name;
        std::function<void()> test_func;
    };
    
    std::vector<Test> tests_;
    int passed_ = 0;
    int failed_ = 0;
    
public:
    void add_test(const std::string& name, std::function<void()> test_func) {
        tests_.push_back({name, std::move(test_func)});
    }
    
    void run_all() {
        std::cout << "Running " << tests_.size() << " tests..." << std::endl;
        std::cout << "================================" << std::endl;
        
        for (const auto& test : tests_) {
            try {
                std::cout << "Running: " << test.name << " ... ";
                test.test_func();
                std::cout << "PASSED" << std::endl;
                ++passed_;
            } catch (const std::exception& e) {
                std::cout << "FAILED: " << e.what() << std::endl;
                ++failed_;
            } catch (...) {
                std::cout << "FAILED: Unknown exception" << std::endl;
                ++failed_;
            }
        }
        
        std::cout << "================================" << std::endl;
        std::cout << "Results: " << passed_ << " passed, " << failed_ << " failed" << std::endl;
    }
    
    int get_failed_count() const { return failed_; }
};

// Test assertion macros
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: !(" #condition ")"); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: " #expected " == " #actual); \
    }

#define ASSERT_NE(expected, actual) \
    if ((expected) == (actual)) { \
        throw std::runtime_error("Assertion failed: " #expected " != " #actual); \
    }

#define ASSERT_THROWS(expression, exception_type) \
    try { \
        expression; \
        throw std::runtime_error("Expected exception " #exception_type " was not thrown"); \
    } catch (const exception_type&) { \
        /* Expected exception caught */ \
    }

// Basic smoke tests
#include "../include/System/Threading/AsyncLocal.hpp"
#include "../include/System/Threading/ThreadLocal.hpp"
#include "../include/System/Threading/Task.hpp"

using namespace System::Threading;

void test_basic_compilation() {
    // Just test that everything compiles and basic functionality works
    AsyncLocal<int> async_local;
    ThreadLocal<std::string> thread_local;
    
    async_local.set(42);
    ASSERT_EQ(42, async_local.get());
    
    thread_local.set("test");
    ASSERT_EQ("test", thread_local.get());
}

void test_task_basic() {
    auto task = Task::run([]() {
        return 42;
    });
    
    int result = task.get_result();
    ASSERT_EQ(42, result);
    ASSERT_TRUE(task.is_completed_successfully());
}

int main() {
    TestFramework framework;
    
    framework.add_test("Basic Compilation", test_basic_compilation);
    framework.add_test("Task Basic", test_task_basic);
    
    framework.run_all();
    
    return framework.get_failed_count();
}
