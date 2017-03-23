#ifndef _APP_HPP_
#define _APP_HPP_

// GUI library
#include "gui.hpp"
// Utility libraries
#include "utils_include.hpp"


class App {
  private:

    static wxFileName execPathInfo;

    // Directory where application executable resides
    static wxString execDirectory;

  public:

    static void Init();

    static wxString GetExecDir();
};


/**
 * Main application
 */
class MainApplication: public wxApp
{
  public:

    /**
     * Called on dialog initialization (after instantiation).
     */
    virtual bool OnInit(); 
};


DECLARE_APP(MainApplication)


#endif