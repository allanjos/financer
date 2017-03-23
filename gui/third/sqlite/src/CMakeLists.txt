# Sources to compile and integrate in executable

SET (SOURCES sqlite3.c)

# Main executable

ADD_LIBRARY (sqlite_internal STATIC ${SOURCES})
