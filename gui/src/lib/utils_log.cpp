/**
 * Log utilities library.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 * \since 02/22/2012
 */


#include "app.hpp"
#include "gui_lib.hpp"
#include "utils_log.hpp"
#include "utils_config.hpp"
#include "app_version.hpp"


LogMessage::LogMessage(wxString message, LogMessageType type) {
  this->message = message;

  this->type = type;

  wxDateTime dateTime = wxDateTime::Now();

  this->timestamp = dateTime.GetTicks();
}


wxString LogMessage::GetMessage() {
  return this->message;
}


LogMessageType LogMessage::GetType() {
  return this->type;
}

time_t LogMessage::GetTime() {
  return this->timestamp;
}


#include "wx/arrimpl.cpp"
WX_DEFINE_EXPORTED_OBJARRAY(LogMessageArray);


LogMessenger::LogMessenger() {
}


LogMessenger::~LogMessenger() {
  this->messages.Clear();
}


LogMessenger *LogMessenger::GetInstance() {
  // This line only runs once, thus creating the only instance in existence
  static LogMessenger *instance = new LogMessenger();

  // dereferencing the variable here, saves the caller from having to use
  // the arrow operator, and removes tempation to try and delete the
  // returned instance.
  return instance; // always returns the same instance
}


void LogMessenger::Register(wxString message, LogMessageType type)
{
  LogMessage logMessage(message, type);

  //wxLogMessage("Log messages count: %d", this->messages.GetCount());

  this->messages.Add(logMessage);

  wxLogMessage(message);
}


void LogMessenger::Clear()
{
  this->messages.Clear();
}


int LogMessenger::SaveToFile()
{
  wxString filePath = App::GetExecDir() + wxFileName::GetPathSeparator() + APP_NAME + wxT(".log");

  return 0;
}