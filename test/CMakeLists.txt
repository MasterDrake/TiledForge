# Create an example executable
add_executable(TestTileForge main.cpp)

# Link the example executable to your library
target_link_libraries(TestTileForge PRIVATE TiledForge)
