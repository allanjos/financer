#include "utils_filesystem.hpp"

#if defined(__UNIX__)
#  include <sys/types.h>
#  include <sys/stat.h>
#endif


#if defined(__UNIX__)
#define DIRECTORY_SEPARATOR "/"
#else
#define DIRECTORY_SEPARATOR "\\"
#endif

bool UtilsFilesystem::IsDir(wxString &path)
{
#if defined(__UNIX__)
  struct stat statInfo;

  stat(path.mb_str(wxConvUTF8), &statInfo);

  if (S_ISDIR(statInfo.st_mode)) {
    return true;
  }
#else
  if (wxFileName::DirExists(path)) {
    return true;
  }
#endif

  return false;
}


FileType_t UtilsFilesystem::GetFileType(wxString &path)
{
  wxRegEx regex;

  if (UtilsFilesystem::IsDir(path)) {
    return FILE_TYPE_FOLDER;
  }
  else if (regex.Compile(wxT("\\.(wav|mp3|ogg|sph)$"), wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(path)) {
    return FILE_TYPE_AUDIO;
  }
  else if (regex.Compile(wxT("aud_log_R[0-9]+\\.txt$"), wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(path)) {
    return FILE_TYPE_AUDITUS_LOG;
  }
  else if (regex.Compile(wxT("\\.(cfg|cnf|conf)$"), wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(path)) {
    return FILE_TYPE_CONF;
  }
  else if (regex.Compile(wxT("\\.(txt)$"), wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(path)) {
    return FILE_TYPE_CONF;
  }
  else {
    return FILE_TYPE_NONE;
  }
}

wxString UtilsFilesystem::GetDirSeparator()
{
#if defined(__UNIX__)
  return wxT("/");
#else
  return wxT("\\");
#endif
}
