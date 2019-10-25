FIND_LIBRARY(DLoopDetector_LIBRARY DLoopDetector
    PATHS /usr/local/lib
)
FIND_PATH(DLoopDetector_INCLUDE_DIR DLoopDetectorConfig.cmake
    PATHS /usr/local/include/DLoopDetector 
)
SET(DLoopDetector_LIBRARIES ${DLoopDetector_LIBRARY})
SET(DLoopDetector_LIBS ${DLoopDetector_LIBRARY})
SET(DLoopDetector_INCLUDE_DIRS ${DLoopDetector_INCLUDE_DIR})
