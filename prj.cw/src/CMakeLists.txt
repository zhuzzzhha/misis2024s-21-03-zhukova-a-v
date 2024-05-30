
set(SOURCES
	test.cpp
)

# Указываем заголовочные файлы
set(HEADERS
    DiceCoefficient.h
    ContigencyTable.h
    ImageStatistics.h
    ImageReader.h
    MetricConstants.h
    Outputter.h
    ClassicMeasures.h
    Segmentation.h
    CohinKappaMetric.h
    GlobalConsistencyError.h
    JaccardCoefficientMetric.h
    RandIndexMetric.h
    GroundTruthGenerator.h
    ErrorImageGenerator.h
)

add_executable(src ${SOURCES})
file(COPY ${HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(src PRIVATE opencv_core opencv_imgproc opencv_highgui)