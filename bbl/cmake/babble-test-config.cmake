# Define variables for running the in-built build tests
# Without this, tests will fail because the echo linker is not defined

# Define an "echo" linker that we'll use to output link libraries to a text file
set(CMAKE_ECHO_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES})
set(CMAKE_ECHO_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_ECHO_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS})
set(CMAKE_ECHO_IMPLICIT_LINK_DIRECTORIES ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
set(
    CMAKE_ECHO_LINK_EXECUTABLE
    "<CMAKE_COMMAND> -E echo \"<LINK_LIBRARIES>\" > <TARGET>"
)
