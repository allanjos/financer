# Sources to compile and integrate in executable

SET (SOURCES lib/gui.cpp
             lib/utils_filesystem.cpp
             lib/utils_math.cpp
             lib/utils_image.cpp
             lib/utils_datetime.cpp
             lib/utils_database_sqlite.cpp
             lib/utils_database.cpp
             lib/utils_log.cpp
             lib/utils_monetary.cpp
             lib/utils_tax.cpp
             lib/utils_product_category.cpp
             lib/utils_config.cpp
             lib/utils_config_server.cpp
             lib/utils_lang.cpp
             accounting/model_account_data.cpp
             accounting/panel_account_new.cpp
             accounting/panel_account_edit.cpp
             accounting/panel_account_main.cpp
             accounting/renderer_data_account.cpp
             app/app.cpp
             bank/dlg_bank_list.cpp
             bank/dlg_bank_account_balance_update.cpp
             bank/panel_bank_account_new.cpp
             bank/panel_bank_account_edit.cpp
             bank/panel_bank_main.cpp
             currency/panel_currency.cpp
             general/dlg_about.cpp
             general/dlg_calendar.cpp
             general/dlg_preferences.cpp
             general/dlg_config_server.cpp
             general/dlg_main.cpp
             general/dlg_ncm_choose.cpp
             estate/panel_estate_main.cpp
             estate/panel_estate_new.cpp
             estate/panel_estate_edit.cpp
             expense/dlg_expense_payment_add.cpp
             expense/dlg_expense_payment_edit.cpp
             expense/panel_expense_new.cpp
             expense/panel_expense_edit.cpp
             income/dlg_income_payment_add.cpp
             income/dlg_income_payment_edit.cpp
             income/panel_income_new.cpp
             income/panel_income_edit.cpp
             income/panel_income_expense.cpp
             log/dlg_log_entry_view.cpp
             log/dlg_log_view.cpp
             owner/panel_owner_edit.cpp
             person/panel_person_new.cpp
             person/panel_person_edit.cpp
             person/panel_person_main.cpp
             product/dlg_product_category_choose.cpp
             taxes/panel_cfop_main.cpp
             taxes/panel_ncm_main.cpp
             taxes/panel_tax_main.cpp
             #taxes/dlg_cfop_choose.cpp
             help/panel_guide.cpp
             canvas_graph_income_expense.cpp)

# Top level headers

INCLUDE_DIRECTORIES (${APPLICATION_SOURCE_DIR}/include)

# wxWidgets include headers

INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIRS})

# SQLite include headers

INCLUDE_DIRECTORIES (${SQLITE_INCLUDE_DIRS})

# Expat

INCLUDE_DIRECTORIES (${EXPAT_DIR}/include)
LINK_DIRECTORIES (${EXPAT_DIR}/lib)

# Resources

ADD_CUSTOM_COMMAND(OUTPUT app_rc.o
                   COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
                                       -I${WXWIDGETS_DIR}/build/win32/lib
                                       -I${WXWIDGETS_DIR}/build/win32/include
                                       -I${WXWIDGETS_DIR}/build/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_SUFFIX}
                                       -I${WXWIDGETS_DIR}/build/win32/lib/mswu
                                       -I${WXWIDGETS_DIR}/build/win32/lib/wx/include/msw-unicode${WXWIDGETS_RELEASE_PART_NAME}-static-${WXWIDGETS_VERSION_SUFFIX}
                                       -i${CMAKE_CURRENT_SOURCE_DIR}/app/app.rc
                                       -o ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)


# Main executable

IF (UNIX)
  ADD_EXECUTABLE (${APP_NAME} ${SOURCES})
ELSE (UNIX)
  #ADD_EXECUTABLE (${APP_NAME} WIN32 ${SOURCES})
  ADD_EXECUTABLE (${APP_NAME} WIN32 ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)
  #ADD_EXECUTABLE (${APP_NAME} WIN32 ${SOURCES})
ENDIF (UNIX)

#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE "")
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_DEBUG "")
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "")

# Base 64 library

FIND_LIBRARY (B64_LIBRARY NAMES b64 PATHS ${LIBRARY_OUTPUT_PATH})

IF (B64_LIBRARY)
  SET (B64_FOUND TRUE)
  TARGET_LINK_LIBRARIES (${APP_NAME} ${B64_LIBRARY})
ENDIF (B64_LIBRARY)

# SQLite library

FIND_LIBRARY (SQLITE_LIBRARY NAMES sqlite_internal PATHS ${LIBRARY_OUTPUT_PATH})

IF (SQLITE_LIBRARY)
  TARGET_LINK_LIBRARIES (${APP_NAME} ${SQLITE_LIBRARY})
ENDIF (SQLITE_LIBRARY)

# wxWidgets library linking

TARGET_LINK_LIBRARIES (${APP_NAME} ${WXWIDGETS_LIBRARIES})


IF (B64_LIBRARY})
  TARGET_LINK_LIBRARIES(${B64_LIBRARY}})
ENDIF (B64_LIBRARY})

# Link system libraries

IF (UNIX)
  TARGET_LINK_LIBRARIES (${APP_NAME} m rt)
ELSE (UNIX)
ENDIF (UNIX)

# Link GCC compiler statically

IF (NOT UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")
ENDIF(NOT UNIX)


#IF (NOT UNIX)
#  INCLUDE_DIRECTORIES (${ICONV_LIBRARIES_DIR})
#ENDIF (NOT UNIX)