/**
 * Allann Jones <alljols[at]yahoo.com.br>
 * \since 20/12/2008
 */

#ifndef _UTILS_ACCOUNT_HPP_
#define _UTILS_ACCOUNT_HPP_


#include <gui_lib.hpp>


class TreeItemDataAccount: public wxTreeItemData
{
private:
  int accountId;

public:
  TreeItemDataAccount(const int& accountId) { this->accountId = accountId; }

  int GetAccountId(void) { return this->accountId; }
};


#endif /* _UTILS_ACCOUNT_HPP_ */
