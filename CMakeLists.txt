cmake_minimum_required(VERSION 3.15)
project(TiledForge VERSION 1.0.0 LANGUAGES CXX)

# Configure CMake options for optional dependencies

# Add the library
add_library(TiledForge  src/FreeFuncs.cpp
						src/ImageLayer.cpp
						src/Map.cpp
						src/Object.cpp
						src/ObjectGroup.cpp
						src/Property.cpp
						src/TileLayer.cpp
						src/LayerGroup.cpp
						src/Tileset.cpp
						src/ObjectTypes.cpp)
						
target_include_directories(TiledForge PUBLIC include)
target_compile_features(TiledForge PUBLIC cxx_std_14)

# Required dependency
find_package(pugixml REQUIRED)
target_link_libraries(TiledForge PUBLIC pugixml::pugixml)

find_package(miniz REQUIRED)
target_link_libraries(TiledForge PUBLIC miniz)

find_package(ZLIB REQUIRED)
target_link_libraries(TiledForge PUBLIC ZLIB::ZLIB)

# Optional dependencies
if(USE_ZSTD)
    find_package(zstd  REQUIRED)
    target_link_libraries(TiledForge PUBLIC zstd::libzstd)
    target_compile_definitions(TiledForge PUBLIC USE_ZSTD)
endif()

# Alias target for find_package support
add_library(TiledForge::TiledForge ALIAS TiledForge)

# Installation setup
include(GNUInstallDirs)
install(TARGETS TiledForge
    EXPORT TiledForgeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the package for find_package
install(EXPORT TiledForgeTargets
    FILE TiledForgeConfig.cmake
    NAMESPACE TiledForge::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiledForge
)

# Add a version file for compatibility
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TiledForgeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TiledForgeConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiledForge
)
