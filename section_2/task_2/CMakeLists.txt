cmake_minimum_required(VERSION 3.5)
project( task_2 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main.o edge_Zumbrum.cpp )
target_link_libraries( main.o ${OpenCV_LIBS} )

set_target_properties(main.o PROPERTIES
    ADDITIONAL_CLEAN_FILES
    "outImage_Zumbrum.jpg"
)