#ifndef _GUI_CONTROLS_HPP_
#define _GUI_CONTROLS_HPP_

/**
 * Unique identifier for graphical controls.
 */
typedef enum {
  DLG_MAIN_MENU_LOG_VIEW_ID = 6000,
  DLG_MAIN_MENU_FULLSCREEN_ID,
  DLG_MAIN_MENU_CONFIG_LOCAL_ID,
  DLG_MAIN_MENU_CONFIG_GENERAL_ID,
  DLG_MAIN_MENU_CONF_CURRENCY_ID,
  DLG_MAIN_MENU_CONF_LOCALITY_ID,
  DLG_MAIN_MENU_PRODUCT_MAIN_ID,
  DLG_MAIN_MENU_ACCOUNT_MAIN_ID,
  DLG_MAIN_MENU_TAXES_MAIN_ID,

  DLG_MAIN_MENU_MODULES_ENTERPRISE_ID,
  DLG_MAIN_MENU_MODULES_ESTATE_ID,
  DLG_MAIN_MENU_MODULES_ACCOUNTING_SUMMARY_ID,
  DLG_MAIN_MENU_MODULES_ACCOUNTS_CHART_ID,
  DLG_MAIN_MENU_MODULES_BANKS_MAIN_ID,
  DLG_MAIN_MENU_MODULES_INCOMES_EXPENSE_MAIN_ID,
  DLG_MAIN_MENU_MODULES_TAXES_MAIN_ID,
  DLG_MAIN_MENU_MODULES_CONTACTS_MAIN_ID,
  DLG_MAIN_MENU_MODULES_DOCUMENTS_MAIN_ID,

  DLG_MAIN_MENU_HELP_MAIN_ID,
  DLG_MAIN_MENU_UPDATE_MAIN_ID,

  DLG_MAIN_TOOLBAR_ACCOUNT_MAIN_ID,
  DLG_MAIN_TOOLBAR_BANK_ACCOUNTS_ID,
  DLG_MAIN_TOOLBAR_ENTERPRISE_MAIN_ID,
  DLG_MAIN_TOOLBAR_CONFIG_MAIN_ID,
  DLG_MAIN_TOOLBAR_FINANCES_MAIN_ID,
  DLG_MAIN_TOOLBAR_INCOMES_EXPENSE_MAIN_ID,
  DLG_MAIN_TOOLBAR_INCOMES_MAIN_ID,
  DLG_MAIN_TOOLBAR_EXPENSES_MAIN_ID,
  DLG_MAIN_TOOLBAR_CONTACTS_MAIN_ID,
  DLG_MAIN_TOOLBAR_ESTATE_ID,

  DLG_MAIN_NOTEBOOK_MAIN_ID,

  DLG_LOG_VIEW_CHOICE_EVENT_TYPE_ID,
  DLG_LOG_VIEW_CLEAR_ID,
  DLG_LOG_VIEW_REFRESH_ID,

  DLG_CONFIG_TOOL_SAVE_ID,
  DLG_CONFIG_TOOL_CLOSE_ID,

  DLG_CONFIG_SERVER_TOOL_SAVE_ID,
  DLG_CONFIG_SERVER_TOOL_CLOSE_ID,

  DLG_ACCOUNT_MAIN_TOOL_NEW_ID,
  DLG_ACCOUNT_MAIN_TOOL_CLOSE_ID,
  DLG_ACCOUNT_MAIN_TOOL_REFRESH_ID,
  DLG_ACCOUNT_MAIN_TEDIT_SEARCH_ID,
  DLG_ACCOUNT_MAIN_BTN_SEARCH_ID,
  DLG_ACCOUNT_MAIN_DATAVIEW_ACCOUNTS_ID,

  PANEL_ACCOUNT_NEW_TOOL_SAVE_ID,
  PANEL_ACCOUNT_NEW_BTN_BALANCE_DATE_ID,
  PANEL_ACCOUNT_NEW_RADIOBUTTON_ACTIVE_ID,
  PANEL_ACCOUNT_NEW_RADIOBUTTON_PASSIVE_ID,
  PANEL_ACCOUNT_NEW_RADIOBUTTON_OF_RESULTS_ID,
  PANEL_ACCOUNT_NEW_TREECTRL_ACCOUNT_LIST_ID,

  PANEL_ACCOUNT_EDIT_TOOL_SAVE_ID,
  PANEL_ACCOUNT_EDIT_TOOL_DELETE_ID,
  PANEL_ACCOUNT_EDIT_BTN_CREATION_DATE_ID,
  PANEL_ACCOUNT_EDIT_BTN_BALANCE_DATE_ID,
  PANEL_ACCOUNT_EDIT_TREECTRL_ACCOUNT_LIST_ID,
  PANEL_ACCOUNT_EDIT_BTN_MOVEMENT_SEARCH_ID,
  PANEL_ACCOUNT_EDIT_RADIOBUTTON_ACTIVE_ID,
  PANEL_ACCOUNT_EDIT_RADIOBUTTON_PASSIVE_ID,
  PANEL_ACCOUNT_EDIT_RADIOBUTTON_OF_RESULTS_ID,

  PANEL_BANK_MAIN_TOOL_NEW_ID,
  PANEL_BANK_MAIN_TOOL_REFRESH_ID,
  PANEL_BANK_MAIN_TOOL_BANK_LIST_ID,

  PANEL_BANK_ACCOUNT_NEW_TOOL_SAVE_ID,
  PANEL_BANK_ACCOUNT_NEW_TEDIT_CREATION_DATE_ID,
  PANEL_BANK_ACCOUNT_NEW_BUTTON_CREATION_DATE_CHOOSE_ID,

  PANEL_BANK_ACCOUNT_EDIT_TOOL_SAVE_ID,
  PANEL_BANK_ACCOUNT_EDIT_TOOL_DELETE_ID,
  PANEL_BANK_ACCOUNT_EDIT_TEDIT_CREATION_DATE_ID,
  PANEL_BANK_ACCOUNT_EDIT_BUTTON_CREATION_DATE_CHOOSE_ID,
  PANEL_BANK_ACCOUNT_EDIT_TOOL_BALANCE_UPDATE_ID,
  PANEL_BANK_ACCOUNT_EDIT_CHOICE_ACCOUNT_TYPE_ID,
  PANEL_BANK_ACCOUNT_EDIT_CHOICE_BANK_ID,

  PANEL_CURRENCY_BTN_SAVE_ID,
  PANEL_CURRENCY_BTN_CLOSE_ID,

  DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_SAVE_ID,
  DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_CLOSE_ID,

  DLG_NCM_CHOOSE_BTN_SELECT_ID,
  DLG_NCM_CHOOSE_BTN_CLOSE_ID,
  DLG_NCM_CHOOSE_TREECTRL_LISTING_ID,

  DLG_TAX_MAIN_BTN_CFOP_ID,
  DLG_TAX_MAIN_BTN_NCM_ID,
  DLG_TAX_MAIN_BTN_IPI_ID,

  DLG_PRODUCT_CATEGORY_MAIN_TREECTRL_LISTING_ID,
  DLG_PRODUCT_CATEGORY_CHOOSE_BTN_SELECT_ID,
  DLG_PRODUCT_CATEGORY_CHOOSE_TOOL_CLOSE_ID,

  DLG_ESTATE_RFID_READ_BTN_CARD_READ_ID,

  PANEL_ESTATE_MAIN_BTN_SUPPLIER_NEW_ID,
  PANEL_ESTATE_MAIN_BTN_REFRESH_ID,
  PANEL_ESTATE_MAIN_BTN_CLOSE_ID,
  PANEL_ESTATE_MAIN_BTN_SEARCH_ID,
  PANEL_ESTATE_MAIN_LISTCTRL_ID,
  PANEL_ESTATE_MAIN_BTN_RFID_READ_ID,

  DLG_ESTATE_NEW_TOOL_SAVE_ID,
  DLG_ESTATE_NEW_BTN_CHOOSE_IMAGE_FILE_ID,
  DLG_ESTATE_NEW_BTN_NCM_CHOOSE_ID,
  DLG_ESTATE_NEW_BTN_ACQUISITION_DATE_ID,
  DLG_ESTATE_NEW_BTN_EXPIRY_DATE_ID,
  DLG_ESTATE_NEW_BTN_DEPRECIATION_BEGIN_DATE_ID,
  DLG_ESTATE_NEW_BTN_RFID_GET_ID,
  DLG_ESTATE_NEW_BTN_RFID_WRITE_ID,

  DLG_ESTATE_EDIT_TOOL_SAVE_ID,
  DLG_ESTATE_EDIT_TOOL_DELETE_ID,
  DLG_ESTATE_EDIT_CHOICE_PRODUCT_TYPE_ID,
  DLG_ESTATE_EDIT_BTN_PRODUCT_TYPE_ADD_ID,
  DLG_ESTATE_EDIT_BTN_CHOOSE_CATEGORY_ID,
  DLG_ESTATE_EDIT_BTN_CHOOSE_IMAGE_FILE_ID,
  DLG_ESTATE_EDIT_BTN_PRODUCT_COMPONENT_SELECT_ID,
  DLG_ESTATE_EDIT_BTN_COMPOSITION_PRODUCT_ADD_ID,
  DLG_ESTATE_EDIT_BTN_NCM_CHOOSE_ID,
  DLG_ESTATE_EDIT_BTN_ACQUISITION_DATE_ID,
  DLG_ESTATE_EDIT_BTN_EXPIRY_DATE_ID,
  DLG_ESTATE_EDIT_BTN_RFID_GET_ID,
  DLG_ESTATE_EDIT_BTN_RFID_WRITE_ID,
  DLG_ESTATE_EDIT_BTN_DEPRECIATION_BEGIN_DATE_ID,

  DLG_INCOME_EXPENSE_MAIN_TOOL_INCOME_NEW_ID,
  DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID,
  DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSE_NEW_ID,
  DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID,
  DLG_INCOME_EXPENSE_MAIN_BTN_REFRESH_ID,
  DLG_INCOME_EXPENSE_MAIN_BTN_CLOSE_ID,
  DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
  DLG_INCOME_EXPENSE_MAIN_LISTCTRL_EXPENSES_ID,

  DLG_INCOME_NEW_CHOICE_PAYMENT_DAY_ID,
  DLG_INCOME_NEW_CHOICE_PAYMENT_MONTH_ID,
  DLG_INCOME_NEW_CHOICE_PAYMENT_YEAR_ID,
  DLG_INCOME_NEW_CHOICE_PAYMENT_INTERVAL_ID,
  DLG_INCOME_NEW_CHOICE_AMOUNT_NOT_FIXED_ID,
  DLG_INCOME_NEW_RADIO_EXPENSE_FIXED_ID,
  DLG_INCOME_NEW_RADIO_EXPENSE_VARIABLE_ID,
  DLG_INCOME_NEW_CHOICE_EXPENSE_CATEGORY_ID,

  DLG_INCOME_PAYMENT_ADD_TOOL_SAVE_ID,
  DLG_INCOME_PAYMENT_ADD_TOOL_CLOSE_ID,
  DLG_INCOME_PAYMENT_ADD_TEDIT_VALUE_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBTN_PAID_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBTN_NOT_PAID_ID,
  DLG_INCOME_PAYMENT_ADD_BTN_PAYMENT_DATE_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
  DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,

  DLG_INCOME_PAYMENT_EDIT_TOOL_SAVE_ID,
  DLG_INCOME_PAYMENT_EDIT_TOOL_DELETE_ID,
  DLG_INCOME_PAYMENT_EDIT_TOOL_CLOSE_ID,
  DLG_INCOME_PAYMENT_EDIT_TEDIT_VALUE_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBTN_PAID_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBTN_NOT_PAID_ID,
  DLG_INCOME_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
  DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,

  DLG_EXPENSE_NEW_CHOICE_PAYMENT_DAY_ID,
  DLG_EXPENSE_NEW_CHOICE_PAYMENT_MONTH_ID,
  DLG_EXPENSE_NEW_CHOICE_PAYMENT_YEAR_ID,
  DLG_EXPENSE_NEW_CHOICE_PAYMENT_INTERVAL_ID,
  DLG_EXPENSE_NEW_CHOICE_AMOUNT_NOT_FIXED_ID,
  DLG_EXPENSE_NEW_RADIO_EXPENSE_FIXED_ID,
  DLG_EXPENSE_NEW_RADIO_EXPENSE_VARIABLE_ID,
  DLG_EXPENSE_NEW_CHOICE_EXPENSE_CATEGORY_ID,

  DLG_INCOME_EDIT_TOOL_SAVE_ID,
  DLG_INCOME_EDIT_TOOL_TAXES_MISC_ADD_ID,
  DLG_INCOME_EDIT_TOOL_TAXES_MISC_DELETE_ID,
  DLG_INCOME_EDIT_CHOICE_EXPENSE_CATEGORY_ID,
  DLG_INCOME_EDIT_CHOICE_PAYMENT_MONTH_ID,
  DLG_INCOME_EDIT_CHOICE_PAYMENT_YEAR_ID,
  DLG_INCOME_EDIT_CHOICE_PAYMENT_INTERVAL_ID,
  DLG_INCOME_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID,
  DLG_INCOME_EDIT_RADIO_EXPENSE_FIXED_ID,
  DLG_INCOME_EDIT_RADIO_EXPENSE_VARIABLE_ID,
  DLG_INCOME_EDIT_TOOL_INSTALLMENT_ADD_ID,
  DLG_INCOME_EDIT_LISTCTRL_INSTALLMENTS_ID,

  DLG_EXPENSE_EDIT_TOOL_SAVE_ID,
  DLG_EXPENSE_EDIT_TOOL_DELETE_ID,
  DLG_EXPENSE_EDIT_RADIO_EXPENSE_FIXED_ID,
  DLG_EXPENSE_EDIT_RADIO_EXPENSE_VARIABLE_ID,
  DLG_EXPENSE_EDIT_CHOICE_EXPENSE_CATEGORY_ID,
  DLG_EXPENSE_EDIT_CHOICE_PAYMENT_MONTH_ID,
  DLG_EXPENSE_EDIT_CHOICE_PAYMENT_YEAR_ID,
  DLG_EXPENSE_EDIT_CHOICE_PAYMENT_INTERVAL_ID,
  DLG_EXPENSE_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID,
  DLG_EXPENSE_EDIT_TOOL_TAXES_MISC_ADD_ID,
  DLG_EXPENSE_EDIT_TOOL_TAXES_MISC_DELETE_ID,
  DLG_EXPENSE_EDIT_TOOL_INSTALLMENT_ADD_ID,
  DLG_EXPENSE_EDIT_LISTCTRL_INSTALLMENTS_ID,

  DLG_EXPENSE_PAYMENT_ADD_TOOL_SAVE_ID,
  DLG_EXPENSE_PAYMENT_ADD_TOOL_CLOSE_ID,
  DLG_EXPENSE_PAYMENT_ADD_TEDIT_VALUE_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBTN_PAID_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBTN_NOT_PAID_ID,
  DLG_EXPENSE_PAYMENT_ADD_BTN_PAYMENT_DATE_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
  DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,

  DLG_EXPENSE_PAYMENT_EDIT_TOOL_SAVE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_TOOL_CLOSE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_TOOL_DELETE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_TEDIT_VALUE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBTN_PAID_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBTN_NOT_PAID_ID,
  DLG_EXPENSE_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
  DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,

  PANEL_PERSON_MAIN_TOOL_NEW_ID,
  PANEL_PERSON_MAIN_TOOL_REFRESH_ID,
  //PANEL_PERSON_MAIN_TOOL_TYPE_MAIN_ID,
  PANEL_PERSON_MAIN_LISTCTRL_CLIENT_LIST_ID,

  PANEL_PERSON_NEW_BTN_SAVE_ID,
  PANEL_PERSON_NEW_TEDIT_BORN_DATE_ID,
  PANEL_PERSON_NEW_BUTTON_BORN_DATE_CHOOSE_ID,
  PANEL_PERSON_NEW_CHOICE_ADDRESS_COUNTRY_ID,
  PANEL_PERSON_NEW_CHOICE_ADDRESS_UF_ID,
  PANEL_PERSON_NEW_CHOICE_ADDRESS_CITY_ID,
  PANEL_PERSON_NEW_CHOICE_RG_COUNTRY_ID,
  PANEL_PERSON_NEW_CHOICE_RG_CITY_ID,
  PANEL_PERSON_NEW_CHOICE_IDENT_STATE,
  PANEL_PERSON_NEW_TEDIT_RG_DATE_ID,
  PANEL_PERSON_NEW_CHOICE_PERSON_TYPE_ID,
  PANEL_PERSON_NEW_BTN_PHOTO_CHOOSE_ID,

  PANEL_PERSON_EDIT_BTN_SAVE_ID,
  PANEL_PERSON_EDIT_BTN_DELETE_ID,
  PANEL_PERSON_EDIT_TEDIT_BORN_DATE_ID,
  PANEL_PERSON_EDIT_BUTTON_BORN_DATE_CHOOSE_ID,
  PANEL_PERSON_EDIT_CHOICE_ADDRESS_COUNTRY_ID,
  PANEL_PERSON_EDIT_CHOICE_ADDRESS_UF_ID,
  PANEL_PERSON_EDIT_CHOICE_ADDRESS_CITY_ID,
  PANEL_PERSON_EDIT_CHOICE_RG_COUNTRY_ID,
  PANEL_PERSON_EDIT_CHOICE_RG_CITY_ID,
  PANEL_PERSON_EDIT_CHOICE_IDENT_STATE_ID,
  PANEL_PERSON_EDIT_TEDIT_RG_DATE_ID,
  PANEL_PERSON_EDIT_CHOICE_PERSON_TYPE_ID,
  PANEL_PERSON_EDIT_BTN_PHOTO_CHOOSE_ID,

  PANEL_OWNER_EDIT_BUTTON_SAVE_ID,
  PANEL_OWNER_EDIT_BTN_PHOTO_CHOOSE_ID,
  PANEL_OWNER_EDIT_CHOICE_PERSON_TYPE_ID,
  PANEL_OWNER_EDIT_TEDIT_RG_DATE_ID,
  PANEL_OWNER_EDIT_CHOICE_RG_COUNTRY_ID,
  PANEL_OWNER_EDIT_CHOICE_IDENT_STATE_ID,
  PANEL_OWNER_EDIT_CHOICE_RG_CITY_ID,
  PANEL_OWNER_EDIT_TEDIT_BORN_DATE_ID,
  PANEL_OWNER_EDIT_BUTTON_BORN_DATE_CHOOSE_ID,
  PANEL_OWNER_EDIT_CHOICE_ADDRESS_COUNTRY_ID,
  PANEL_OWNER_EDIT_CHOICE_ADDRESS_UF_ID,
  PANEL_OWNER_EDIT_CHOICE_ADDRESS_CITY_ID,

  DLG_CALENDAR_CALENDAR_CTRL_ID

} ICT_GUIControl_ID_t;

#endif /* _GUI_CONTROLS_HPP_ */