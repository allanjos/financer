#ifndef _UTILS_IMAGE_HPP_
#define _UTILS_IMAGE_HPP_

#include "gui_lib.hpp"

class UtilsImage {
  public:
    static int Rescale(wxImage &image, int maxWidth, int maxHeight);
};


#endif /* _UTILS_IMAGE_HPP_ */
