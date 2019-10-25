FIND_LIBRARY(DBoW2_LIBRARY DBoW2
    PATHS "/home/matrix/Downloads/IC-138/BinVocabulary/build/dependencies/install/lib"
)
FIND_PATH(DBoW2_INCLUDE_DIR DBoW2Config.cmake
    PATHS "/home/matrix/Downloads/IC-138/BinVocabulary/build/dependencies/install/include/DBoW2" 
)
SET(DBoW2_LIBRARIES ${DBoW2_LIBRARY})
SET(DBoW2_LIBS ${DBoW2_LIBRARY})
SET(DBoW2_INCLUDE_DIRS ${DBoW2_INCLUDE_DIR})
