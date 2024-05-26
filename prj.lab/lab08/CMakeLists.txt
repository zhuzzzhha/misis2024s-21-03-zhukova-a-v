set(SOURCES
    lab08.cpp
    
)

# Указываем заголовочные файлы
set(HEADERS
)


add_executable(lab08 lab08.cpp)
target_link_libraries(lab08 ${OpenCV_LIBS} opencv_core opencv_imgproc opencv_highgui opencv_features2d4)