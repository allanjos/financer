/**
 * Mathematical utilities.
 *
 * \file  math_utils.hpp
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

/** \defgroup Mathematical Mathematical utilities. */
/**@{*/

#ifndef _MATH_UTILS_HPP_
#define _MATH_UTILS_HPP_

#include <cmath>
#ifdef WIN32
#  include <windows.h>
#endif

//#include <iostream>

//using namespace std;

/**
 * Mathematical utilities class.
 */
class MathUtils {
  public:
    /**
     * Returns the percentual from a total.
     *
     * \param total The `total' amount.
     * \param percent The percentage to calculate from `total'.
     */
    static double Percent(double total, double percent);

    /**
     * Rounds a float number.
     *
     * \param x The number to round.
     *
     * \return The rounded number.
     */
    static double Round(double x);
};


#endif /* _MATH_UTILS_HPP_ */


/**@}*/
