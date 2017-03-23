#include "utils_image.hpp"


int UtilsImage::Rescale(wxImage &image, int maxWidth, int maxHeight)
{
  if (!image.IsOk() || image.GetWidth() < 0 || image.GetHeight() < 0) {
    return -1;
  }

  // Rescale image

  //int maxWidth = staticBmpProductPicture->GetSize().GetWidth();
  //int maxHeight = staticBmpProductPicture->GetSize().GetHeight();
  int width = image.GetWidth();
  int height = image.GetHeight();
  double proportion = (double) image.GetWidth() / (double) image.GetHeight();

  //wxMessageBox(wxString::Format(wxT("proportion: %d / %d = %f"), image.GetWidth(), image.GetHeight(), proportion));

  //wxMessageBox(wxString::Format(wxT("%dx%d"), width, height));

  if (width > maxWidth) {
    width = maxWidth;

    height = width / proportion;
  }

  //wxMessageBox(wxString::Format(wxT("%dx%d"), width, height));

  if (height > maxHeight) {
    height = maxHeight;

    width = height * proportion;
  }

  //wxMessageBox(wxString::Format(wxT("%dx%d"), width, height));

  image.Rescale(width, height, wxIMAGE_QUALITY_HIGH);

  return 0;
}
