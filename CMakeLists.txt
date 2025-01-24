cmake_minimum_required(VERSION 3.10)

# Set the project name
project(rtdataLayer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configura il percorso per trovare i pacchetti installati tramite vcpkg
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/vcpkg/installed/x64-windows")

# Add vcpkg toolchain (se non già configurato globalmente)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# Find and include Boost
cmake_policy(SET CMP0167 NEW)
find_package(Boost 1.86 REQUIRED CONFIG)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(hiredis CONFIG REQUIRED)
find_package(Threads REQUIRED)

if(Boost_FOUND)
    # Include Boost headers
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost found: ${Boost_VERSION}")
    message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found. Please verify the BOOST_ROOT path.")
endif()

# Configure Mosquitto manually
set(MOSQUITTO_INCLUDE_DIR "${CMAKE_PREFIX_PATH}/include")
set(MOSQUITTO_LIB "${CMAKE_PREFIX_PATH}/lib/mosquitto.lib")

# Add the executable
add_executable(rtdataLayer
    src/main.cpp
    src/MqttClient.cpp
)

include_directories(include)

# Include project-specific directories
target_include_directories(rtdataLayer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${MOSQUITTO_INCLUDE_DIR}
)

# Link Boost and other libraries to the target
target_link_libraries(rtdataLayer PRIVATE
    ${Boost_LIBRARIES}
    ${MOSQUITTO_LIB}
    hiredis::hiredis
    nlohmann_json::nlohmann_json
    Threads::Threads
)

