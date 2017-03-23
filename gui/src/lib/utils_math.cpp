/**
 * Mathematical utilities.
 *
 * \file  utils_math.cpp
 * \version 1.0
 * \since 20/07/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include <cstdio>
#include "utils_math.hpp"

double MathUtils::Percent(double total, double percent) {
  return (percent * total / 100);
}

double MathUtils::Round(double x) {
  double y = ceil(x);

  //printf("  x = %f\n", x);
  //printf("  y = %f\n", y);
  //printf("  y - x = %f\n", y - x);

  if (y - x < 0.5) {
    //printf("  rounded: %f\n", y);
    return y;
  }
  else {
    //printf("  rounded: %f\n", floor(x));
    return floor(x);
  }
}
