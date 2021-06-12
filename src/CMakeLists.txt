set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/AobaAPI/AobaAPI.hpp")

add_library(AobaAPI AobaAPI.cpp ${HEADER_LIST})

target_include_directories(AobaAPI PUBLIC ../include)

target_compile_features(AobaAPI PUBLIC cxx_std_11)

source_group(
	TREE "${PROJECT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADER_LIST}
)

add_subdirectory(Math)
add_subdirectory(Core)
add_subdirectory(Ops)
add_subdirectory(IO)