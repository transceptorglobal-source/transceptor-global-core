cmake_minimum_required(VERSION 3.16)
project(transceptor_core LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Каталоги заголовков
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/economics
    ${CMAKE_CURRENT_SOURCE_DIR}/core/engine/trust_policy
    ${CMAKE_CURRENT_SOURCE_DIR}/core/identity/bridges
    ${CMAKE_CURRENT_SOURCE_DIR}/core/network
    ${CMAKE_CURRENT_SOURCE_DIR}/core/security
)

# Исходные файлы библиотеки C++ Ядра
set(CORE_SOURCES
    core/engine/trust_policy/trust_policy_engine.cpp
    core/engine/trust_policy/agora_ai_oracle_filter.cpp
    core/engine/trust_policy/competence_passport_manager.cpp
    core/engine/trust_policy/zk_location_tracker.cpp
    core/identity/bridges/jni_bridge.cpp
)

# Сборка разделяемой библиотеки для связки с Python/FastAPI и NDK
add_library(transceptor_core SHARED ${CORE_SOURCES})

# Исполняемые файлы тестов
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main_security_audit.cpp")
    add_executable(main_security_audit main_security_audit.cpp)
    target_link_libraries(main_security_audit PRIVATE transceptor_core)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration_test.cpp")
    add_executable(integration_test integration_test.cpp)
    target_link_libraries(integration_test PRIVATE transceptor_core)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_gateway.cpp")
    add_executable(test_gateway test_gateway.cpp)
    target_link_libraries(test_gateway PRIVATE transceptor_core)
endif()