/**
 * About dialog.
 *
 * \file  dlg_log_view.hpp
 * \version 1.0
 * \since 02/22/2012
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _UTILS_LOG_HPP_
#define _UTILS_LOG_HPP_


#include "wx/dynarray.h"
#include "gui_lib.hpp"


/**
 * Log message type.
 */ 
typedef short int LogMessageType;


/**
 * Log message.
 *
 * @TODO Add a timestamp to the message.
 */

class LogMessage {
  private:
  
    wxString message;  /**< The log message string. */

    LogMessageType type;   /**< The message type. */

    time_t timestamp;

  public:

    static const LogMessageType MSG_ERROR = 0x0001;
    static const LogMessageType MSG_WARN  = 0x0002;
    static const LogMessageType MSG_INFO  = 0x0004;
    static const LogMessageType MSG_DBG   = 0x0008;

    LogMessage(wxString message, LogMessageType type);

    wxString GetMessage();

    LogMessageType GetType();

    time_t GetTime();
};


/**< A type for a array of log messages. */

WX_DECLARE_EXPORTED_OBJARRAY(LogMessage, LogMessageArray);


class LogMessenger
{
  public:
      // The magic function, which allows access to the class from anywhere
      // To get the value of the instance of the class, call:
      //     LogMessenger::Instance().GetString();
      static LogMessenger *GetInstance();

      LogMessageArray messages;

      void Register(wxString message, LogMessageType type = LogMessage::MSG_DBG);

      void Clear();

      int SaveToFile();

  private:
      // We need to make some given functions private to finish the definition of the singleton
      LogMessenger(); // default constructor available only to members or friends of this class

      //static LogMessenger *instance = null;

      // Note that the next two functions are not given bodies, thus any attempt
      // to call them implicitly will return as compiler errors. This prevents
      // accidental copying of the only instance of the class.
      LogMessenger(const LogMessenger &old); // disallow copy constructor

      const LogMessenger &operator=(const LogMessenger &old); //disallow assignment operator

      // Note that although this should be allowed,
      // some compilers may not implement private destructors
      // This prevents others from deleting our one single instance, which was otherwise created on the heap
      ~LogMessenger();
};


#endif  /* _UTILS_LOG_HPP_ */