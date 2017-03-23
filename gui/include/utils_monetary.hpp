#ifndef _UTILS_MONETARY_HPP_
#define _UTILS_MONETARY_HPP_

#include "gui_lib.hpp"
#include "utils_types.hpp"

/**
 * \file utils_monetary.hpp
 * \author Allann Jones <alljols[at]yahoo.com.br>
 *
 * VALUE is a float/double value.
 * NUMBER is a string representing a number with locale representation.
 * MONETARY is a string represeting a monetary amount.
 */

class Price {
  public:
    Price();

    double value;
    wxString currencyStr;
    int currencyId;
};


class Currency {
  public:
    int id;
    wxString name;
    wxString abbr;
};


class Monetary {
  public:

    static HashStringMap fracSep;
    static HashStringMap thousandSep;

    static HashStringMap fracSepRegexChar;
    static HashStringMap thousandSepRegexChar;

    static void Init();

    static int LoadCurrencyList(wxChoice *choiceCtrl);

    static int GetDefaultCurrency(Currency &currency);

    static double ValueFromStr(wxString str);

    /**
     *
     */
    static double ValueFromStr(const char *str);

    /**
     * Convert a value in number format to a number in string format.
     */
    static wxString Value2Str(double value, short precision = 2);

    static wxString NumberNormalize(wxString str);

    static bool NumberIsValid(wxString str);

    static bool MonetaryIsValid(wxString str);

    /**
     * Normalize a string representing a number to display visually on user interface.
     * \param str The string with a number in database / system format.
     */
    static wxString Normalize(wxString str);

    /**
     * Convert a number in a monetary string
     */
    static wxString Number2Monetary(double number, int precision = 2);

    /**
     * Convert a number in a monetary string
     */
    static wxString Number2Monetary(wxString str);

    /**
     * Convert a string number to database number format.
     */
    static wxString Number2Database(wxString str);

    /**
     * Convert a string number to database number format.
     */
    static wxString Number2Database(double number, int precision = 2);

    /**
     * Convert a string number to system number format
     */
    static wxString Number2System(wxString str);

    /**
     * Convert a monetary string to database number format.
     */
    static wxString Monetary2Database(wxString monetaryStr);

    /**
     * Convert a monetary string to number string.
     */
    static wxString Monetary2Number(wxString monetaryStr);
};

#endif /* _UTILS_MONETARY_HPP_ */