cmake_minimum_required(VERSION 3.15)
project(TiledForge VERSION 1.0.0 LANGUAGES CXX)

# Configure CMake options for optional dependencies
option(TILEDFORGE_BUILD_STATIC "Build tiledforge as a static library" ON)
option(TILEDFORGE_BUILD_TEST "Build test" OFF)
# Add the library
# Set library type based on user options
if(TILEDFORGE_BUILD_STATIC)
    set(LIBRARY_TYPE "STATIC")
else()
    set(LIBRARY_TYPE "SHARED")
endif()

add_library(tiledforge ${LIBRARY_TYPE} src/FreeFuncs.cpp
						src/ImageLayer.cpp
						src/Map.cpp
						src/Object.cpp
						src/ObjectGroup.cpp
						src/Property.cpp
						src/TileLayer.cpp
						src/LayerGroup.cpp
						src/Tileset.cpp
						src/ObjectTypes.cpp)
						
target_include_directories(tiledforge
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(tiledforge PUBLIC cxx_std_14)

if(TILEDFORGE_BUILD_STATIC)
	target_compile_definitions(tiledforge PUBLIC TILEDFORGE_STATIC)
endif()

if(TILEDFORGE_BUILD_TEST)
    add_subdirectory(test)
endif()

# Required dependency
find_package(pugixml CONFIG REQUIRED)
target_link_libraries(tiledforge PUBLIC pugixml::pugixml)

find_package(miniz CONFIG REQUIRED)
target_link_libraries(tiledforge PUBLIC miniz::miniz)

find_package(ZLIB CONFIG REQUIRED)
target_link_libraries(tiledforge PUBLIC ZLIB::ZLIB)

# Optional dependencies
if(USE_ZSTD)
    message(STATUS "Building with zstd support.")
    find_package(zstd REQUIRED)
    target_link_libraries(tiledforge PUBLIC zstd::libzstd)
    target_compile_definitions(tiledforge PUBLIC USE_ZSTD)
endif()

# Alias target for find_package support
add_library(tiledforge::tiledforge ALIAS tiledforge)

# Installation setup
include(GNUInstallDirs)
install(TARGETS tiledforge
    EXPORT tiledforgeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the package for find_package
install(EXPORT tiledforgeTargets
    FILE tiledforgeConfig.cmake
    NAMESPACE tiledforge::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tiledforge
)

# Add a version file for compatibility
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/tiledforgeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/tiledforgeConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tiledforge
)
