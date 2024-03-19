cmake_minimum_required(VERSION 3.20)

project(zhukova-a-v)


set(VCPKG_ROOT "C:/Users/ALLA/vcpkg")
set(VCPKG_INSTALLED_DIR "C:/Users/ALLA/vcpkg/installed")
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
#add_subdirectory(prj.cw)

#set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install)