                                                                                                
# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)
           
# Project name
project(MyProject)
           
# Add source fileFILE(GLOB_RECURSE MyCSources ./*.c)
FILE(GLOB test_client_srcs srcs/client/socket_client.cpp)
FILE(GLOB test_server_srcs srcs/server/socket_server.cpp)
FILE(GLOB shared_srcs srcs/server/Server.cpp srcs/client/client.cpp srcs/channel/Channel.cpp)
FILE(GLOB server_srcs srcs/server/main.cpp ${shared_srcs})
FILE(GLOB client_srcs srcs/client/main.cpp ${shared_srcs})
           
           
           
           
# Add linker flags globally
add_link_options(-g)
           
           
           
# Add compiler flags globally
add_compile_options(-g -O0)
           
           
           
# Add an executable target
add_executable(test_server.exe ${test_server_srcs})
add_executable(test_client.exe ${test_client_srcs})
add_executable(server.exe ${server_srcs})
add_executable(client.exe ${client_srcs})
           
           
           
           
           
           
# Add extra targets
add_custom_target(re
        COMMAND make clean
        COMMAND  make
        VERBATIM
)          
           
           
           
add_custom_target(fclean
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning..."
        COMMAND make clean
        VERBATIM
        )  
