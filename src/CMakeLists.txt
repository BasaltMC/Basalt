
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${Basalt_SOURCE_DIR}/include/basalt/*.hpp")

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/sockpp" "build")
add_executable(Basalt main.cpp ${HEADER_LIST})
target_include_directories(Basalt PUBLIC "${PROJECT_SOURCE_DIR}/extern/sockpp")
target_link_libraries(Basalt sockpp-static)

#target_include_directories(Basalt PUBLIC ../include)


target_compile_features(Basalt PUBLIC cxx_std_17)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})