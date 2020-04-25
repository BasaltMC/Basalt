
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${Basalt_SOURCE_DIR}/include/basalt/*.hpp")

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/sockpp" "extern/sockpp")
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/entt" "extern/entt")
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/robin-map" "extern/robin-map")
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/spdlog" "extern/spdlog")
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/cpp-taskflow" "extern/cpp-taskflow")

add_executable(Basalt main.cpp ${HEADER_LIST})

#target_include_directories(Basalt PUBLIC "${PROJECT_SOURCE_DIR}/extern/sockpp")
target_link_libraries(Basalt sockpp-static)
target_link_libraries(Basalt EnTT)
target_link_libraries(Basalt robin_map)
target_link_libraries(Basalt spdlog)
target_link_libraries(Basalt Cpp-Taskflow)

target_include_directories(Basalt PUBLIC ../include)

target_compile_features(Basalt PUBLIC cxx_std_17)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})