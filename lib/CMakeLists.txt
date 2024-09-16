AUX_SOURCE_DIRECTORY(src server_SRCS)

add_library(Server ${server_SRCS})
target_include_directories(Server PUBLIC include)

target_link_libraries(Server ${Protobuf_LIBRARIES})

add_subdirectory(message)
target_link_libraries(Server message)

add_subdirectory(tests)