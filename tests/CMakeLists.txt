
# Tests CMakeLists.txt

# Find or download a testing framework (using a simple approach here)
# In a real project, you might use Google Test, Catch2, etc.

# Simple test runner
add_executable(TestRunner TestRunner.cpp)
target_link_libraries(TestRunner CoreLibCPP)

# Individual test files
add_executable(AsyncLocalTests AsyncLocalTests.cpp)
target_link_libraries(AsyncLocalTests CoreLibCPP)

add_executable(ThreadLocalTests ThreadLocalTests.cpp)
target_link_libraries(ThreadLocalTests CoreLibCPP)

add_executable(BarrierTests BarrierTests.cpp)
target_link_libraries(BarrierTests CoreLibCPP)

add_executable(CountdownEventTests CountdownEventTests.cpp)
target_link_libraries(CountdownEventTests CoreLibCPP)

add_executable(ReaderWriterLockSlimTests ReaderWriterLockSlimTests.cpp)
target_link_libraries(ReaderWriterLockSlimTests CoreLibCPP)

add_executable(TaskTests TaskTests.cpp)
target_link_libraries(TaskTests CoreLibCPP)

add_executable(CancellationTokenTests CancellationTokenTests.cpp)
target_link_libraries(CancellationTokenTests CoreLibCPP)

# Add tests to CTest
add_test(NAME AsyncLocalTests COMMAND AsyncLocalTests)
add_test(NAME ThreadLocalTests COMMAND ThreadLocalTests)
add_test(NAME BarrierTests COMMAND BarrierTests)
add_test(NAME CountdownEventTests COMMAND CountdownEventTests)
add_test(NAME ReaderWriterLockSlimTests COMMAND ReaderWriterLockSlimTests)
add_test(NAME TaskTests COMMAND TaskTests)
add_test(NAME CancellationTokenTests COMMAND CancellationTokenTests)
