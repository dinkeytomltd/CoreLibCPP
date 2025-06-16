
# Examples CMakeLists.txt

# AsyncLocal example
add_executable(AsyncLocalExample AsyncLocalExample.cpp)
target_link_libraries(AsyncLocalExample CoreLibCPP)

# ThreadLocal example
add_executable(ThreadLocalExample ThreadLocalExample.cpp)
target_link_libraries(ThreadLocalExample CoreLibCPP)

# Barrier example
add_executable(BarrierExample BarrierExample.cpp)
target_link_libraries(BarrierExample CoreLibCPP)

# CountdownEvent example
add_executable(CountdownEventExample CountdownEventExample.cpp)
target_link_libraries(CountdownEventExample CoreLibCPP)

# ReaderWriterLockSlim example
add_executable(ReaderWriterLockSlimExample ReaderWriterLockSlimExample.cpp)
target_link_libraries(ReaderWriterLockSlimExample CoreLibCPP)

# Task example
add_executable(TaskExample TaskExample.cpp)
target_link_libraries(TaskExample CoreLibCPP)

# CancellationToken example
add_executable(CancellationTokenExample CancellationTokenExample.cpp)
target_link_libraries(CancellationTokenExample CoreLibCPP)
