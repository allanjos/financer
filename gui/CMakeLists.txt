#
# Personal finance management application.
#
# Allann Jones
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (APPLICATION)

SET (APP_NAME "financer")

SET (EXECUTABLE_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/lib)

SET (WXWIDGET_VERSION_MAJOR 3.1)

IF (UNIX)
  SET (OPERATING_SYSTEM "unix")
ELSE (UNIX)
  SET (OPERATING_SYSTEM "windows")
ENDIF (UNIX)

# Linux

IF (UNIX)
  # Subfolder inside folder third/wxWidgets/build/linux/ (eg. fc20, debian73)
  SET (OS_DISTRO fc25)
ELSE (UNIX)
  # win32, win64
  SET (OS_DISTRO win32)
  SET (OS_BUILDTYPE "msys")
ENDIF (UNIX)

SET (CMAKE_VERBOSE_MAKEFILE "ON")

# Compiler flags for release mode
SET (CMAKE_C_FLAGS_RELEASE "")
SET (CMAKE_CXX_FLAGS_RELEASE "")
SET (CMAKE_C_FLAGS_RELEASE_INIT "")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT "")

# Compiler flags for debug mode
SET (CMAKE_C_FLAGS_DEBUG "")
SET (CMAKE_CXX_FLAGS_DEBUG "")
SET (CMAKE_C_FLAGS_DEBUG_INIT "")
SET (CMAKE_CXX_FLAGS_DEBUG_INIT "")

# Release | Debug
SET (CMAKE_BUILD_TYPE Release)

SET (LIBSNDFILE_ACTIVATED FALSE)
SET (SNDLIB_ACTIVATED FALSE)
SET (ICONV_ACTIVATED FALSE)

# wxWidgets configuration

SET (WXWIDGETS_DIR ${APPLICATION_SOURCE_DIR}/third/wxWidgets)

# Expat configuration

SET (WXWIDGETS_DIR ${APPLICATION_SOURCE_DIR}/third/wxWidgets)

# Expat

#SET (EXPAT_VERSION "2.0.1")
SET (EXPAT_DIR ${APPLICATION_SOURCE_DIR}/third/expat)

#IF (UNIX)
#  INCLUDE_DIRECTORIES (${EXPAT_DIR}/build/linux/include)
#ELSE (UNIX)
#  INCLUDE_DIRECTORIES (${EXPAT_DIR}/build/${OS_DISTRO}/include)
#ENDIF (UNIX)

# XML parser

#INCLUDE_DIRECTORIES (${APPLICATION_SOURCE_DIR}/src/XML/parser/include)

# Base64 algorithm library

SET (LIBB64_DIR ${APPLICATION_SOURCE_DIR}/third/b64)
SET (LIBB64_INCLUDE_DIRS ${LIBB64_DIR}/include)

# SQLite

SET (SQLITE_DIR ${APPLICATION_SOURCE_DIR}/third/sqlite)
SET (SQLITE_INCLUDE_DIRS ${SQLITE_DIR}/include)


# Libiconv

IF (ICONV_ACTIVATED)
  IF (UNIX)
  ELSE (UNIX)
    SET (ICONV_DIR ${APPLICATION_SOURCE_DIR}/third/libiconv/windows)

    SET (ICONV_INCLUDE_DIR ${APPLICATION_SOURCE_DIR}/third/libiconv/windows/include)
    SET (ICONV_LIBRARIES_DIR ${APPLICATION_SOURCE_DIR}/third/libiconv/windows/lib)
  ENDIF (UNIX)
ENDIF (ICONV_ACTIVATED)

# wxWidgets

SET (WXWIDGETS_INCLUDE_DIRS "")

IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  IF (NOT MSVC)
    ADD_COMPILE_OPTIONS (-g -Wall)
  ENDIF ()
ELSE ()
  ADD_COMPILE_OPTIONS (-Wall -DwxDEBUG_LEVEL=0)

  IF (NOT MSVC)
    ADD_DEFINITIONS(-DNDEBUG)
  ELSE ()
    ADD_DEFINITIONS(-NDEBUG)
  ENDIF ()
ENDIF ()

# Trigraphs

ADD_COMPILE_OPTIONS (-std=gnu++11)
ADD_COMPILE_OPTIONS (-trigraphs)

# wxWidgets - Linux / Unix

IF (UNIX)
  SET (WXWIDGETS_VERSION_SUFFIX ${WXWIDGET_VERSION_MAJOR})
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")
  # SET (WXWIDGETS_RELEASE_PART_NAME "")
  SET (WXWIDGETS_RELEASE_PART_NAME "")

  SET (WXWIDGETS_PLATFORM_SUFFIX "gtk3")

  #ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -g -pthread -Wall -DwxDEBUG_LEVEL=0)
  ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__)

  ADD_COMPILE_OPTIONS(-g -pthread)

  IF (WXWIDGETS_PLATFORM_SUFFIX STREQUAL "gtk2")
    SET (WXWIDGETS_LIBRARIES
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_richtext-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_adv-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_html-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_aui-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_propgrid-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_core-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_qa-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_gl-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_xrc-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_media-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_ribbon-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_xml-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_net-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu-${WXWIDGETS_VERSION_SUFFIX}
                             wxregexu-${WXWIDGETS_VERSION_SUFFIX}

                             gtk-x11-2.0
                             gdk-x11-2.0
                             pangocairo-1.0
                             atk-1.0
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             pangoft2-1.0
                             pango-1.0
                             gobject-2.0
                             fontconfig
                             freetype
                             gthread-2.0
                             glib-2.0
                             X11
                             Xxf86vm
                             SM
                             gtk-x11-2.0
                             gdk-x11-2.0
                             pangocairo-1.0
                             atk-1.0
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             pangoft2-1.0
                             pango-1.0
                             gobject-2.0
                             glib-2.0
                             fontconfig
                             freetype
                             notify
                             gdk_pixbuf-2.0
                             gio-2.0
                             gobject-2.0
                             glib-2.0
                             png
                             z
                             jpeg
                             tiff
                             pthread
                             expat
                             z
                             dl
                             m
                             z
                             dl
                             m)
  ELSE ()
    SET (WXWIDGETS_LIBRARIES
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_richtext-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_adv-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_html-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_aui-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_propgrid-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_core-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_qa-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_gl-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_xrc-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_media-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_ribbon-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_xml-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_net-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu-${WXWIDGETS_VERSION_SUFFIX}
                             wxregexu-${WXWIDGETS_VERSION_SUFFIX}

                             expat
                             gtk-3
                             gdk-3
                             pangocairo-1.0
                             pango-1.0
                             atk-1.0
                             cairo-gobject
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             gobject-2.0
                             gthread-2.0
                             glib-2.0
                             X11
                             Xxf86vm
                             SM
                             notify
                             webkitgtk-3.0
                             soup-2.4
                             javascriptcoregtk-3.0
                             png
                             z
                             jpeg
                             tiff
                             pthread
                             dl
                             m)
  ENDIF ()

    #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-release-static-${WXWIDGETS_VERSION_SUFFIX}
    #                     ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/include/wx-${WXWIDGET_VERSION_MAJOR})

  SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_INCLUDE_DIRS}
  # gtk2-unicode-3.0
  # ./lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-3.0/wx/setup.h
                              ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-static-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/include/wx-${WXWIDGET_VERSION_MAJOR})


  LINK_DIRECTORIES (/usr/X11R6/lib
                    ${WXWIDGETS_DIR}/build/linux/${OS_DISTRO}/lib)

# wxWidgets - Windows

ELSE (UNIX)
  SET (WXWIDGETS_VERSION_INCLUDE_DIR "3.1")
  SET (WXWIDGETS_VERSION_SUFFIX "3.1")
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")

  #ADD_DEFINITIONS(-DwxDEBUG_LEVEL=0 -D__WXMSW__ -mwindows -mthreads)
  #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -D__WXMSW__ -MD -MP -DNOPCH -DWXUSINGDLL -D_UNICODE -DUNICODE -mwindows -mthreads")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXMSW__ -D_UNICODE -DUNICODE -D_LARGEFILE_SOURCE=unknown -mwindows -mthreads")

  LINK_DIRECTORIES (${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib)

  IF (OS_BUILDTYPE STREQUAL "msys")
    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib/wx/include/msw-unicode-static-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib/mswu
                                ${WXWIDGETS_DIR}/build/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib/msw${WXWIDGETS_UNICODE_SUFFIX})
    SET(WXWIDGETS_LIBRARIES wx_mswu_aui-3.1
                            wx_baseu_xml-3.1
                            wx_mswu_html-3.1
                            wx_mswu_adv-3.1
                            wx_mswu_core-3.1
                            wx_baseu-3.1
                            wxtiff-3.1
                            wxjpeg-3.1
                            wxpng-3.1
                            wxzlib-3.1
                            wxregexu-3.1
                            wxexpat-3.1

                            rpcrt4
                            oleaut32
                            ole32
                            uuid
                            winspool
                            winmm
                            shell32
                            shlwapi
                            comctl32
                            comdlg32
                            advapi32
                            version
                            wsock32
                            gdi32
                            uxtheme
                            oleacc)
  ELSE ()
    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib
                                ${WXWIDGETS_DIR}/build/${OS_DISTRO}/include
                                ${WXWIDGETS_DIR}/build/${OS_DISTRO}/lib/mswu)

    SET(WXWIDGETS_LIBRARIES wxmsw31u_adv
                            wxmsw31u_html
                            wxmsw31u_aui
                            wxbase31u_xml
                            wxmsw31u_core
                            wxbase31u
                            wxtiff
                            wxjpeg
                            wxpng
                            wxzlib
                            wxregexu
                            wxexpat
                            kernel32
                            user32
                            gdi32
                            comdlg32
                            winspool
                            winmm
                            shell32
                            comctl32
                            ole32
                            oleaut32
                            uuid
                            rpcrt4
                            advapi32
                            wsock32
                            wininet)
  ENDIF ()

  #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/include
  #                     ${WXWIDGETS_DIR}/lib/vc_lib/msw${WXWIDGETS_UNICODE_SUFFIX}${WXWIDGETS_DEBUG_SUFFIX})
ENDIF (UNIX)


#SUBDIRS (src tests PREORDER third)

ADD_SUBDIRECTORY(third)
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(tests)
