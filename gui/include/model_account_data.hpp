/**
 * \brief Accounts main dialog.
 *
 * \file  dlg_account_main.hpp
 * \version 1.0
 * \since 14/11/2008
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _MODEL_ACCOUNT_DATA_HPP_
#define _MODEL_ACCOUNT_DATA_HPP_


#include "utils_include.hpp"
#include "wx/dataview.h"


class AccountDataModelNode;

WX_DEFINE_ARRAY_PTR(AccountDataModelNode*, AccountDataModelNodes);

class AccountDataModelNode
{
  public:

    AccountDataModelNode(AccountDataModelNode* parent,
                         int accountId,
                         const wxString &account_name,
                         const wxString &type,
                         const wxString &balance);

    AccountDataModelNode(AccountDataModelNode* parent,
                     const wxString &account_name);

    ~AccountDataModelNode();

    void SetAsContainer(bool isContainer);

    bool IsContainer();

    AccountDataModelNode* GetParent();

    AccountDataModelNodes &GetChildren();

    AccountDataModelNode* GetNthChild(unsigned int n);

    void Insert(AccountDataModelNode* child, unsigned int n);

    void Append(AccountDataModelNode* child);

    unsigned int GetChildCount();

    int accountId;

    wxString accountName;

    wxString accountType;

    wxString accountBalance;

    AccountDataModelNodes m_children;

  private:

    AccountDataModelNode  *m_parent;

    bool m_isContainer;
};



class AccountDataModel: public wxDataViewModel
{
  public:

    AccountDataModelNode* m_root;

    AccountDataModelNode *dataModelNodeAccountActiveRoot;

    AccountDataModelNode *dataModelNodeAccountPassiveRoot;

    AccountDataModelNode *dataModelNodeAccountOfResultRoot;

    // constructor

    AccountDataModel();

    ~AccountDataModel();

    int DeleteNodeChildren(AccountDataModelNode* node);

    AccountDataModelNode *GetRoot();

    int LoadData();

    int GetAccountId(const wxDataViewItem &item) const;

    wxString GetTitle(const wxDataViewItem &item) const;


    int LoadAccountChildList(AccountDataModelNode* dataModelNode, const char *accountType = NULL);


    // helper methods to change the model

    void Delete(const wxDataViewItem &item);

    // override sorting to always sort branches ascendingly

    int Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
                unsigned int column, bool ascending);

    // implementation of base class virtuals to define model

    virtual unsigned int GetColumnCount() const;

    virtual wxString GetColumnType(unsigned int col) const;

    virtual bool HasContainerColumns(const wxDataViewItem &item) const;

    virtual void GetValue(wxVariant &variant,
                          const wxDataViewItem &item, unsigned int col) const;

    virtual bool GetAttr(unsigned int row, unsigned int col, wxDataViewItemAttr &attr);

    virtual bool SetValue(const wxVariant &variant,
                          const wxDataViewItem &item, unsigned int col);

    virtual wxDataViewItem GetParent(const wxDataViewItem &item) const;

    virtual bool IsContainer(const wxDataViewItem &item) const;

    virtual unsigned int GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const;

    // DnD

    virtual bool IsDraggable(const wxDataViewItem &item);

    virtual size_t GetDragDataSize(const wxDataViewItem &item, const wxDataFormat &WXUNUSED(format));

    virtual bool GetDragData(const wxDataViewItem &item, const wxDataFormat &WXUNUSED(format),
                             void* dest, size_t WXUNUSED(size));

    wxDataViewItem GetNinthItem();

  private:

    AccountDataModelNode *m_test;

    AccountDataModelNode* m_pop;

    AccountDataModelNode* m_classical;

    AccountDataModelNode* m_ninth;

    bool m_classicalMusicIsKnownToControl;
};


#endif /* _MODEL_ACCOUNT_DATA_HPP_ */