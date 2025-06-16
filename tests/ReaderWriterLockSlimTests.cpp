
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <chrono>
#include "../include/System/Threading/ReaderWriterLockSlim.hpp"

using namespace System::Threading;

void test_basic_read_lock() {
    ReaderWriterLockSlim lock;
    
    lock.enter_read_lock();
    assert(lock.is_read_lock_held());
    assert(!lock.is_write_lock_held());
    assert(!lock.is_upgradeable_read_lock_held());
    
    lock.exit_read_lock();
    assert(!lock.is_read_lock_held());
    
    std::cout << "Basic read lock test passed" << std::endl;
}

void test_basic_write_lock() {
    ReaderWriterLockSlim lock;
    
    lock.enter_write_lock();
    assert(lock.is_write_lock_held());
    assert(!lock.is_read_lock_held());
    assert(!lock.is_upgradeable_read_lock_held());
    
    lock.exit_write_lock();
    assert(!lock.is_write_lock_held());
    
    std::cout << "Basic write lock test passed" << std::endl;
}

void test_basic_upgradeable_read_lock() {
    ReaderWriterLockSlim lock;
    
    lock.enter_upgradeable_read_lock();
    assert(lock.is_upgradeable_read_lock_held());
    assert(lock.is_read_lock_held()); // Upgradeable read is also a read lock
    assert(!lock.is_write_lock_held());
    
    lock.exit_upgradeable_read_lock();
    assert(!lock.is_upgradeable_read_lock_held());
    assert(!lock.is_read_lock_held());
    
    std::cout << "Basic upgradeable read lock test passed" << std::endl;
}

void test_multiple_readers() {
    ReaderWriterLockSlim lock;
    std::vector<std::thread> threads;
    std::atomic<int> concurrent_readers(0);
    std::atomic<int> max_concurrent_readers(0);
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&lock, &concurrent_readers, &max_concurrent_readers]() {
            lock.enter_read_lock();
            
            int current = concurrent_readers.fetch_add(1) + 1;
            int expected = max_concurrent_readers.load();
            while (current > expected && !max_concurrent_readers.compare_exchange_weak(expected, current)) {
                expected = max_concurrent_readers.load();
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            
            concurrent_readers.fetch_sub(1);
            lock.exit_read_lock();
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Should have had multiple concurrent readers
    assert(max_concurrent_readers.load() > 1);
    
    std::cout << "Multiple readers test passed" << std::endl;
}

void test_writer_exclusivity() {
    ReaderWriterLockSlim lock;
    std::atomic<bool> writer_active(false);
    std::atomic<bool> reader_blocked(true);
    
    // Start writer thread
    std::thread writer([&lock, &writer_active]() {
        lock.enter_write_lock();
        writer_active.store(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        writer_active.store(false);
        lock.exit_write_lock();
    });
    
    // Give writer time to acquire lock
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    // Start reader thread (should be blocked)
    std::thread reader([&lock, &writer_active, &reader_blocked]() {
        lock.enter_read_lock();
        // If we get here while writer is active, the test fails
        if (writer_active.load()) {
            reader_blocked.store(false);
        }
        lock.exit_read_lock();
    });
    
    writer.join();
    reader.join();
    
    assert(reader_blocked.load()); // Reader should have been blocked
    
    std::cout << "Writer exclusivity test passed" << std::endl;
}

void test_try_locks() {
    ReaderWriterLockSlim lock;
    
    // Try read lock (should succeed)
    assert(lock.try_enter_read_lock());
    assert(lock.is_read_lock_held());
    
    // Try write lock while read lock held (should fail)
    assert(!lock.try_enter_write_lock());
    
    lock.exit_read_lock();
    
    // Try write lock (should succeed)
    assert(lock.try_enter_write_lock());
    assert(lock.is_write_lock_held());
    
    // Try read lock while write lock held (should fail)
    assert(!lock.try_enter_read_lock());
    
    lock.exit_write_lock();
    
    std::cout << "Try locks test passed" << std::endl;
}

void test_recursion_policy_no_recursion() {
    ReaderWriterLockSlim lock(LockRecursionPolicy::NoRecursion);
    
    lock.enter_read_lock();
    
    // Should throw when trying to acquire write lock
    try {
        lock.enter_write_lock();
        assert(false); // Should not reach here
    } catch (const LockRecursionException&) {
        // Expected
    }
    
    lock.exit_read_lock();
    
    std::cout << "No recursion policy test passed" << std::endl;
}

int main() {
    std::cout << "Running ReaderWriterLockSlim tests..." << std::endl;
    
    test_basic_read_lock();
    test_basic_write_lock();
    test_basic_upgradeable_read_lock();
    test_multiple_readers();
    test_writer_exclusivity();
    test_try_locks();
    test_recursion_policy_no_recursion();
    
    std::cout << "All ReaderWriterLockSlim tests passed!" << std::endl;
    return 0;
}
