set(SOURCES
    lab07.cpp
    
)

# Указываем заголовочные файлы
set(HEADERS
    ground_truth_generator.h
    ContigencyTable.h
    segmentation.h
    ImageStatistics.h
    MetricConstants.h
    ClassicMeasures.h
    Outputter.h
)


add_executable(lab07 lab07.cpp)
target_link_libraries(lab07 ${OpenCV_LIBS} opencv_core opencv_imgproc opencv_highgui opencv_features2d4)