#ifndef _UTILS_PAYMENT_HPP_
#define _UTILS_PAYMENT_HPP_

class Payment {
public:
  unsigned int id;
  wxString date;
  double value;
  bool paid;
  unsigned int receiptId;
  wxString registerDateTime;
};

#endif /* _UTILS_PAYMENT_HPP_ */
