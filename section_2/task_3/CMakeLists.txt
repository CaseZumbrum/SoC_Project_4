cmake_minimum_required(VERSION 3.5)
project( task_3 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Find the SystemC package

set(CMAKE_PREFIX_PATH "/opt/systemc")
find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})


add_executable( sc_main.o src/sc_main.cpp )
target_link_libraries( sc_main.o ${OpenCV_LIBS} )
target_link_libraries(sc_main.o SystemC::systemc)

set_target_properties(sc_main.o PROPERTIES
    ADDITIONAL_CLEAN_FILES
    "lines_Zumbrum.jpg"
)