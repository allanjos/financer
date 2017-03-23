# Sources to compile and integrate in executable

SET (SOURCES cencode.cpp
             cdecode.cpp)


# Main executable
ADD_LIBRARY (b64 STATIC ${SOURCES})


#TARGET_LINK_LIBRARIES (b64)
