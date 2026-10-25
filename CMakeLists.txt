cmake_minimum_required(VERSION 3.16)
project(click-buffer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Geode SDK path - adjust if needed
set(GEODE_SDK_PATH "$ENV{HOME}/.geode/sdk" CACHE PATH "Path to Geode SDK")

find_package(Geode REQUIRED PATHS ${GEODE_SDK_PATH})

add_executable(${PROJECT_NAME} SHARED src/ClickBuffer.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Geode::Geode)

geode_add_mod(${PROJECT_NAME}
    ID "click-buffer"
    VERSION "1.0.0"
    NAME "Click Buffer"
    DEVELOPER "YourName"
    DESCRIPTION "Click Buffer - Expands click window for more forgiving inputs"
    SETTINGS_FILE "mod.json"
    RESOURCES_DIR "resources"
)
