
set(SOURCES
    main.cpp
    image_generator.cpp
    log_detector.cpp
    cnctd_comps_detector.cpp
    evaluator.cpp
)

# Указываем заголовочные файлы
set(HEADERS
    image_generator.h
    log_detector.cpp
    cnctd_comps_detector.h
    evaluator.h
)

add_executable(lab04 ${SOURCES})
file(COPY ${HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(lab04 ${OpenCV_LIBS} opencv_core4 opencv_imgproc opencv_highgui opencv_features2d)