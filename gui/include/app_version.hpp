/**
 * Informations about the application.
 *
 * \file  app_version.hpp
 * \version 1.0
 * \since 24/06/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */
#ifndef _APP_VERSION_HPP_
#define _APP_VERSION_HPP_

#define APP_NAME "financer"
#define APP_OWNER "Allann Jones"
#define APP_URL "https://www.olivum.com.br"

/** Application major version. */
#define APP_VERSION_MAJOR "0"
/** Application minor version. */
#define APP_VERSION_MINOR "5"
/** Application release control. */
#define APP_RELEASE_CANDIDATE "2"

/** Application version string. */
#ifndef APP_RELEASE_CANDIDATE
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR ".0"
#else
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_RELEASE_CANDIDATE
#endif

#endif /* _APP_VERSION_HPP_ */