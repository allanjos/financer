/**
 * Filesystem utilities.
 *
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _UTILS_FILESYSTEM_HPP_
#define _UTILS_FILESYSTEM_HPP_

#include "gui_lib.hpp"


typedef int FileType_t;

/**
 * Filesystem utilities class.
 */
class UtilsFilesystem {
  public:
    static const int FILE_TYPE_FOLDER      = 0;
    static const int FILE_TYPE_AUDIO       = 1;
    static const int FILE_TYPE_TEXT        = 2;
    static const int FILE_TYPE_AUDITUS_LOG = 3;
    static const int FILE_TYPE_CONF        = 4;
    static const int FILE_TYPE_NONE        = 5;

    static bool IsDir(wxString &path);

    static FileType_t GetFileType(wxString &path);

    static wxString GetDirSeparator();
};


#endif /* _UTILS_FILESYSTEM_HPP_ */
