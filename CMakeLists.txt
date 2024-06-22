cmake_minimum_required(VERSION 3.20)

project(zhukova-a-v)

# Указание пути к VCPKG из командной строки
if(NOT VCPKG_ROOT)
  set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
endif()

set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT}/installed")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)


set(OpenCV_DIR "${VCPKG_ROOT}/installed/x64-windows/share/opencv4")
set(Protobuf_DIR "${VCPKG_ROOT}/installed/x64-windows/share/protobuf")
set(quirc_DIR "${VCPKG_ROOT}/installed/x64-windows/share/quirc")
set(TIFF_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include/tiff")
set(TIFF_LIBRARY "${VCPKG_ROOT}/installed/x64-windows/lib/tiff")

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
include_directories("${VCPKG_ROOT}/installed/x64-windows/include" "${VCPKG_ROOT}/buildtrees")
add_subdirectory(prj.lab)
add_subdirectory(prj.cw)

install(TARGETS src DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install)
install(FILES prj.cw/src/test.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install)
