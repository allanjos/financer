/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * database.c
 * Copyright (C) 2013 Allann Jones <alljols[at]yahoo.com.br>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Allann Jones'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * financer IS PROVIDED BY Allann Jones ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Allann Jones OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdlib.h>
#include "utils_database_sqlite.hpp"
#include "utils_database_postgresql.hpp"
#include "sqlite3.h"
#include "utils_config.hpp"
#include "utils_log.hpp"

// Command for SQLite database structure definition

const char *db_setup_cmd_list_sqlite[] = {
   // DDL
   "CREATE TABLE IF NOT EXISTS location_country ("
      "country_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL UNIQUE,"
      "nacionalidade varchar(255),"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS location_state ("
      "state_id integer PRIMARY KEY,"
      "country_id integer NOT NULL REFERENCES location_country(country_id),"
      "nome varchar(255) NOT NULL,"
      "sigla char(2),"
      "postkey varchar(128) UNIQUE,"
      "UNIQUE (nome, country_id)"
  ");",
  "CREATE TABLE IF NOT EXISTS location_city ("
      "city integer PRIMARY KEY,"
      "state_id integer NOT NULL REFERENCES location_state(state_id),"
      "nome varchar(255) NOT NULL,"
      "cep varchar(15),"
      "praca varchar(50),"
      "postkey varchar(128) UNIQUE,"
      "UNIQUE (nome, state_id)"
  ");",
  "CREATE TABLE IF NOT EXISTS location_district ("
      "district_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "city integer REFERENCES location_city(city),"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS location_timezone ("
      "timezone_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL UNIQUE,"
      "diff_gmt_hour smallint NOT NULL,"
      "diff_gmt_minutes smallint DEFAULT 0,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS measure ("
      "measure_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL UNIQUE,"
      "descricao text,"
      "sigla varchar(10) UNIQUE,"
      "measure_id_default integer REFERENCES measure(measure_id) ON DELETE SET NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS measure_rate ("
      "proporcao_id integer PRIMARY KEY,"
      "measure_id_from integer NOT NULL REFERENCES measure(measure_id),"
      "measure_id_to integer NOT NULL REFERENCES measure(measure_id),"
      "fator real NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS monetary_currency ("
      "currency_id integer PRIMARY KEY,"
      "sigla varchar(5) NOT NULL,"
      "nome varchar(255),"
      "padrao boolean,"
      "CHECK (sigla != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS accounting_account_subtype ("
      "subtipo_id integer PRIMARY KEY,"
      "nome varchar(255),"
      "description text,"
      "status char(1) DEFAULT 'A' NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS accounting_account ("
      "account_id integer PRIMARY KEY,"
      "conta_parent_id integer REFERENCES accounting_account(account_id),"
      "codigo varchar(50) UNIQUE,"
      "nome varchar(255),"
      "currency_id integer REFERENCES monetary_currency(currency_id),"
      "description text,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "tipo_id char(1) NOT NULL,"
      "subtipo_id integer REFERENCES accounting_account_subtype(subtipo_id),"
      "criacao_date date,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP"
  ");",
  "CREATE TABLE IF NOT EXISTS bank ("
      "bank_id integer PRIMARY KEY,"
      "compe varchar(10),"
      "nome varchar(255) NOT NULL,"
      "country_id integer REFERENCES location_country(country_id),"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "email varchar(255),"
      "filiado char(1) DEFAULT 'Y',"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS bank_account_type ("
      "account_type_id char(1) NOT NULL PRIMARY KEY,"
      "nome varchar(100) NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS bank_account ("
      "account_id integer PRIMARY KEY,"
      "account_type_id char(1) NOT NULL REFERENCES bank_account_type(account_type_id),"
      "bank_id integer NOT NULL REFERENCES bank(bank_id),"
      "accounting_account_id integer REFERENCES accounting_account(account_id),"
      "codigo TEXT NOT NULL,"
      "agency TEXT NOT NULL,"
      "titular_nome TEXT,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "creation_dt date,"
      "CHECK (titular_nome != ''),"
      "UNIQUE (codigo, agency)"
  ");",
  "CREATE TABLE IF NOT EXISTS bank_account_event_type ("
      "account_movement_type_id char(1) PRIMARY KEY,"
      "nome varchar(100) NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS bank_account_event ("
      "movement_id integer PRIMARY KEY,"
      "account_id integer NOT NULL REFERENCES bank_account(account_id),"
      "account_movement_type_id char(1) NOT NULL,"
      "valor real NOT NULL,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "operation_dt datetime"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_empresa ("
      "enterprise_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "cnpj_cpf varchar(25),"
      "inscr_estadual varchar(50),"
      "rua varchar(255),"
      "endr_numero varchar(6),"
      "bairro varchar(255),"
      "endr_complemento varchar(255),"
      "cep varchar(15),"
      "country_id integer REFERENCES location_country(country_id),"
      "state integer REFERENCES location_state(state_id),"
      "city integer REFERENCES location_city(city),"
      "homepage varchar(255),"
      "email varchar(255),"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "descricao text,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_empresas ("
      "enterprise_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "trading_name varchar(255),"
      "cnpj varchar(25),"
      "corporate_name varchar(255),"
      "state_registration varchar(50),"
      "homepage varchar(255),"
      "email varchar(255),"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_empresa_endereco ("
      "endereco_id integer PRIMARY KEY,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresas(enterprise_id) ON DELETE CASCADE,"
      "rua varchar(255),"
      "endr_numero varchar(10),"
      "endr_complemento varchar(255),"
      "bairro varchar(255),"
      "cep varchar(15),"
      "country_id integer REFERENCES location_country(country_id),"
      "state integer REFERENCES location_state(state_id),"
      "city integer REFERENCES location_city(city),"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_empresa_imagem ("
      "picture_id integer PRIMARY KEY,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresas(enterprise_id) ON DELETE CASCADE,"
      "nome varchar(255),"
      "description text,"
      "file_original_name varchar(100),"
      "file_content BLOB NOT NULL,"
      "file_mime varchar(20) NOT NULL,"
      "file_size integer,"
      "creation_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_empresa_telefone ("
      "phone_id integer PRIMARY KEY,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresas(enterprise_id) ON DELETE CASCADE,"
      "tipo char(1),"
      "numero varchar(20),"
      "ramal varchar(20),"
      "fax boolean DEFAULT 0,"
      "contact_name varchar(255),"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS empresa_endereco ("
      "endereco_id integer PRIMARY KEY,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresa(enterprise_id) ON DELETE CASCADE,"
      "rua varchar(255),"
      "endr_numero varchar(10),"
      "endr_complemento varchar(255),"
      "bairro varchar(255),"
      "cep varchar(15),"
      "country_id integer REFERENCES location_country(country_id),"
      "state integer REFERENCES location_state(state_id),"
      "city integer REFERENCES location_city(city),"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS person_type ("
      "tipo_id char(1) NOT NULL PRIMARY KEY,"
      "nome varchar(255) NOT NULL UNIQUE,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS person_title ("
      "titulo_id integer PRIMARY KEY,"
      "sigla varchar(10),"
      "nome varchar(255) NOT NULL,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS person_marital_status ("
      "marital_status_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS person ("
      "person_id integer PRIMARY KEY AUTOINCREMENT,"
      "nome varchar(80),"
      "tipo_id char(1) REFERENCES person_type(tipo_id),"
      "person_title_id integer REFERENCES person_title(titulo_id),"
      "identity_number varchar(20),"
      "identity_dt date,"
      "identity_institution varchar(255),"
      "identity_country varchar(255),"
      "identity_state varchar(255),"
      "identity_city varchar(255),"
      "security_social_number varchar(18),"
      "corporate_name varchar(255),"
      "state_registration varchar(50),"
      "trading_name varchar(255),"
      "carteira_trabalho varchar(30),"
      "doc_militar_ra varchar(20),"
      "doc_militar_csm varchar(10),"
      "titulo_eleitor_nr varchar(50),"
      "titulo_eleitor_zona varchar(5),"
      "titulo_eleitor_secao varchar(6),"
      "titulo_eleitor_cidade_id integer REFERENCES location_city(city),"
      "titulo_eleitor_cidade_other varchar(255),"
      "titulo_eleitor_dt date,"
      "birth_date date,"
      "naturalidade_cidade_id integer REFERENCES location_city(city),"
      "naturalidade_cidade_other varchar(255),"
      "naturalidade_uf_id integer REFERENCES location_state(state_id),"
      "naturalidade_uf_other varchar(255),"
      "nationality varchar(255),"
      "marital_status_id integer REFERENCES person_marital_status(marital_status_id),"
      "country_id integer REFERENCES location_country(country_id),"
      "state integer REFERENCES location_state(state_id),"
      "city integer REFERENCES location_city(city),"
      "email varchar(255),"
      "email_2 varchar(255),"
      "homepage varchar(255),"
      "gender char(1),"
      "profession varchar(255),"
      "carplate varchar(40),"
      "father_name varchar(255),"
      "mother_name varchar(255),"
      "deficiency text,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS pessoa_empresa_imagem ("
      "picture_id integer PRIMARY KEY,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresas(enterprise_id) ON DELETE CASCADE,"
      "nome varchar(255),"
      "description text,"
      "file_original_name varchar(100),"
      "file_content BLOB NOT NULL,"
      "file_mime varchar(20) NOT NULL,"
      "file_size integer,"
      "creation_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS person_address ("
      "endereco_id integer PRIMARY KEY,"
      "person_id integer NOT NULL REFERENCES person(person_id),"
      "rua varchar(255),"
      "endr_numero varchar(10),"
      "endr_complemento varchar(255),"
      "bairro varchar(255),"
      "cep varchar(15),"
      "country_id integer REFERENCES location_country(country_id),"
      "state integer REFERENCES location_state(state_id),"
      "city integer REFERENCES location_city(city),"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS person_picture ("
      "pessoa_imagem_id integer PRIMARY KEY,"
      "person_id integer NOT NULL REFERENCES person(person_id),"
      "nome varchar(255),"
      "description text,"
      "file_original_name varchar(100),"
      "file_content BLOB NOT NULL,"
      "file_mime varchar(20) NOT NULL,"
      "file_size integer,"
      "creation_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS person_phone ("
      "phone_id integer PRIMARY KEY,"
      "person_id integer NOT NULL REFERENCES person(person_id) ON DELETE CASCADE,"
      "numero varchar(20),"
      "ramal varchar(20),"
      "fax boolean DEFAULT 0,"
      "contact_name varchar(255),"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE,"
      "tipo char(1),"
      "CHECK (numero != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS owner ("
      "id integer PRIMARY KEY UNIQUE,"
      "name varchar(80),"
      "type_id char(1) REFERENCES person_type(tipo_id),"
      "title_id integer REFERENCES person_title(titulo_id),"
      "identity_number varchar(20),"
      "identity_dt date,"
      "identity_institution varchar(255),"
      "identity_country varchar(255),"
      "identity_state varchar(255),"
      "identity_city varchar(255),"
      "security_social_number varchar(18),"
      "enterprise_state_registration_nr varchar(50),"
      "enterprise_fantasy_name varchar(255),"
      "employee_doc_nr varchar(30),"
      "military_doc_ra varchar(20),"
      "military_doc_csm varchar(10),"
      "elector_doc_nr varchar(50),"
      "elector_doc_zona varchar(5),"
      "elector_doc_secao varchar(6),"
      "elector_doc_cidade_id integer REFERENCES location_city(city),"
      "elector_doc_cidade_other varchar(255),"
      "elector_doc_dt date,"
      "born_dt date,"
      "place_birth_city_id integer REFERENCES location_city(city),"
      "place_birth_city_other varchar(255),"
      "place_birth_state_id integer REFERENCES location_state(state_id),"
      "place_birth_state_other varchar(255),"
      "nationality_id integer,"
      "nationality_other varchar(255),"
      "marital_status_id integer REFERENCES person_marital_status(marital_status_id),"
      "country_id integer REFERENCES location_country(country_id),"
      "state_id integer REFERENCES location_state(state_id),"
      "city_id integer REFERENCES location_city(city),"
      "email varchar(255),"
      "email_2 varchar(255),"
      "homepage varchar(255),"
      "gender char(1),"
      "profession varchar(255),"
      "carplate varchar(40),"
      "father_name varchar(255),"
      "mother_name varchar(255),"
      "deficiency text,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS owner_picture ("
      "id integer PRIMARY KEY,"
      "nome varchar(255),"
      "description text,"
      "file_original_name varchar(100),"
      "file_content BLOB NOT NULL,"
      "file_mime varchar(20) NOT NULL,"
      "file_size integer,"
      "creation_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS owner_address ("
      "id integer PRIMARY KEY,"
      "owner_id integer NOT NULL REFERENCES owner(id) ON DELETE CASCADE,"
      "street varchar(255),"
      "nr varchar(10),"
      "complement varchar(255),"
      "country_id integer REFERENCES location_country(country_id),"
      "state_id integer REFERENCES location_state(state_id),"
      "city_id integer REFERENCES location_city(city),"
      "district varchar(255),"
      "postal_code varchar(15),"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS owner_phone ("
      "id integer PRIMARY KEY,"
      "owner_id integer NOT NULL REFERENCES owner(id) ON DELETE CASCADE,"
      "nr varchar(20),"
      "branch_line varchar(20),"
      "fax boolean DEFAULT 0,"
      "contact_name varchar(255),"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE,"
      "type_id char(1),"
      "CHECK (nr != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_tipo ("
      "tipo_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL UNIQUE,"
      "descricao text,"
      "status char(1),"
      "postkey varchar(128) UNIQUE,"
      "ordem integer,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_cliente ("
      "cliente_id integer PRIMARY KEY,"
      "tipo_id integer NOT NULL REFERENCES cliente_tipo(tipo_id),"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_empresa ("
      "cliente_id integer NOT NULL REFERENCES cliente_cliente(cliente_id) ON DELETE CASCADE,"
      "enterprise_id integer NOT NULL REFERENCES empresa_empresas(enterprise_id) ON DELETE CASCADE,"
      "PRIMARY KEY (cliente_id, enterprise_id)"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_observacao ("
      "observacao_id integer PRIMARY KEY,"
      "cliente_id integer NOT NULL REFERENCES cliente_cliente(cliente_id) ON DELETE CASCADE,"
      "conteudo text"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_pessoa ("
      "cliente_id integer NOT NULL REFERENCES cliente_cliente(cliente_id) ON DELETE CASCADE,"
      "person_id integer NOT NULL REFERENCES person(person_id) ON DELETE CASCADE,"
      "PRIMARY KEY (cliente_id, person_id)"
  ");",
  "CREATE TABLE IF NOT EXISTS cliente_telefone ("
      "phone_id integer PRIMARY KEY,"
      "cliente_id integer NOT NULL REFERENCES cliente_cliente(cliente_id) ON DELETE CASCADE,"
      "numero varchar(20) NOT NULL,"
      "ramal varchar(20),"
      "contact_name varchar(255),"
      "status char(1) DEFAULT 'A',"
      "postkey varchar(128) UNIQUE,"
      "tipo char(1),"
      "CHECK (numero != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS comercio_cfop ("
      "cfop_id integer PRIMARY KEY,"
      "cfop_parent_id integer REFERENCES comercio_cfop(cfop_id) ON DELETE CASCADE,"
      "codigo varchar(15) NOT NULL UNIQUE,"
      "descricao text NOT NULL,"
      "terminal boolean DEFAULT 1,"
      "status char(1) DEFAULT 'A',"
      "CHECK (codigo != ''),"
      "CHECK (descricao != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS comercio_ncm ("
      "ncm_id integer PRIMARY KEY,"
      "ncm_parent_id integer REFERENCES comercio_ncm(ncm_id),"
      "codigo varchar(15) NOT NULL UNIQUE,"
      "descricao text NOT NULL,"
      "terminal boolean DEFAULT 1,"
      "status char(1) DEFAULT 'A'"
  ");",
  "CREATE TABLE IF NOT EXISTS tributario_imposto ("
      "imposto_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "tipo_id char(1),"
      "aliquota_comum real,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "descricao text,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS tributario_imposto_icms_cst_a ("
      "codigo char(1) PRIMARY KEY,"
      "nome varchar(255)"
  ");",
  "CREATE TABLE IF NOT EXISTS tributario_imposto_icms_cst_b ("
      "codigo char(2) PRIMARY KEY,"
      "nome varchar(255)"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_categoria ("
      "categoria_id integer PRIMARY KEY,"
      "categoria_parent_id integer REFERENCES produto_categoria(categoria_id) ON DELETE CASCADE,"
      "nome varchar(255),"
      "UNIQUE (categoria_parent_id, nome)"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_fabricante ("
      "fabricante_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "homepage varchar(255),"
      "email varchar(255),"
      "telefone varchar(30),"
      "fax varchar(30),"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_lote ("
      "lote_id integer PRIMARY KEY,"
      "quantidade integer,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1)"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_marca ("
      "marca_id integer PRIMARY KEY,"
      "nome varchar(255),"
      "descricao text,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_tipo ("
      "tipo_id integer PRIMARY KEY,"
      "nome varchar(255),"
      "descricao text,"
      "status char(1)"
  ");",
  "CREATE TABLE IF NOT EXISTS produto_subtipo ("
      "subtipo_id integer PRIMARY KEY,"
      "tipo_id integer NOT NULL REFERENCES produto_tipo(tipo_id) ON DELETE CASCADE,"
      "nome varchar(255),"
      "descricao text,"
      "status char(1) DEFAULT 'A'"
  ");",
  "CREATE TABLE IF NOT EXISTS trabalhista_regime_trabalho ("
      "regime_trabalho_id integer PRIMARY KEY,"
      "nome varchar(255),"
      "carteira_assinada boolean DEFAULT 1,"
      "status char(1) DEFAULT 'A' NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS contato_telefone_tipo ("
      "telefone_tipo_id integer PRIMARY KEY,"
      "nome varchar(100) NOT NULL,"
      "postkey varchar(128) UNIQUE,"
      "CHECK (nome != '')"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_expense_category ("
      "expense_category_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_expense ("
      "expense_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "tipo_id char(1),"
      "expense_category_id integer REFERENCES finance_expense_category(expense_category_id),"
      "montante_fixo boolean DEFAULT 0,"
      "montante_valor real,"
      "account_id integer REFERENCES accounting_account(account_id),"
      "parcela_valor real,"
      "installment_periodicity char(1),"
      "quitada boolean DEFAULT 0,"
      "pagamento_dia smallint,"
      "pagamento_mes smallint,"
      "pagamento_ano smallint,"
      "pagamento_hour smallint,"
      "pagamento_minute smallint,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_expense_notes ("
      "expense_id integer PRIMARY KEY REFERENCES finance_expense(expense_id) ON DELETE CASCADE,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "observacao text,"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_expense_installment ("
      "installment_id integer PRIMARY KEY,"
      "expense_id integer NOT NULL REFERENCES finance_expense(expense_id) ON DELETE CASCADE,"
      "account_id integer REFERENCES accounting_account(account_id),"
      "pagamento_date date NOT NULL,"
      "pagamento_time time,"
      "pagamento_forma char(1),"
      "valor real NOT NULL,"
      "quitada boolean DEFAULT 0,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "descricao varchar(255),"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_income_category ("
      "income_category_id integer PRIMARY KEY,"
      "nome varchar(255) NOT NULL,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_income ("
      "income_id integer PRIMARY KEY,"
      "account_id integer REFERENCES accounting_account(account_id),"
      "nome varchar(255) NOT NULL,"
      "regime_trabalho_id integer REFERENCES trabalhista_regime_trabalho(regime_trabalho_id),"
      "income_category_id integer REFERENCES finance_income_category(income_category_id),"
      "montante_fixo boolean DEFAULT 0,"
      "montante_valor real,"
      "parcela_valor real,"
      "installment_periodicity char(1),"
      "quitada boolean DEFAULT 0,"
      "pagamento_dia smallint,"
      "pagamento_mes smallint,"
      "pagamento_ano smallint,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_income_notes ("
      "income_id integer NOT NULL REFERENCES finance_income(income_id) ON DELETE CASCADE,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "observacao text,"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS finance_income_installment ("
      "installment_id integer PRIMARY KEY,"
      "income_id integer NOT NULL REFERENCES finance_income(income_id),"
      "account_id integer REFERENCES accounting_account(account_id),"
      "pagamento_date date NOT NULL,"
      "pagamento_time time,"
      "pagamento_forma char(1),"
      "valor real NOT NULL,"
      "quitada boolean DEFAULT 0,"
      "register_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "descricao varchar(255),"
      "mobile_id integer UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS estate_item ("
      "estate_item_id integer PRIMARY KEY,"
      "codigo varchar(50) UNIQUE,"
      "nome varchar(255) NOT NULL,"
      "series_nr varchar(100),"
      "barcode varchar(100),"
      "net_weight real,"
      "gross_weight real,"
      "measure_id integer REFERENCES measure(measure_id),"
      "tipo_id integer REFERENCES produto_tipo(tipo_id),"
      "subtipo_id integer REFERENCES produto_subtipo(subtipo_id),"
      "categoria_id integer REFERENCES produto_categoria(categoria_id),"
      "comprimento real,"
      "largura real,"
      "altura real,"
      "fisico boolean DEFAULT 1,"
      "marca_id integer REFERENCES produto_marca(marca_id),"
      "fabricante_id integer REFERENCES produto_fabricante(fabricante_id),"
      "acquisition_date date,"
      "warranty_months integer,"
      "value_original real,"
      "value_market real,"
      "depreciation_rate real,"
      "depreciation_start_date date,"
      "depreciation_accumulated real,"
      "book_value real,"
      "descricao text,"
      "status char(1) DEFAULT 'A',"
      "insurance_company varchar(255),"
      "insurance_policy varchar(255),"
      "insurance_due_dt date,"
      "rfid_serial varchar(100) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS estate_item_picture ("
      "estate_picture_Id integer PRIMARY KEY,"
      "estate_item_id integer NOT NULL REFERENCES estate_item(estate_item_id) ON DELETE CASCADE,"
      "nome varchar(255),"
      "description text,"
      "file_original_name varchar(100),"
      "file_content BLOB NOT NULL,"
      "file_mime varchar(20) NOT NULL,"
      "file_size integer,"
      "creation_dt datetime DEFAULT CURRENT_TIMESTAMP,"
      "status char(1) DEFAULT 'A' NOT NULL,"
      "postkey varchar(128) UNIQUE"
  ");",
  "CREATE TABLE IF NOT EXISTS estate_item_info ("
      "estate_item_info_id integer PRIMARY KEY,"
      "estate_item_id integer NOT NULL REFERENCES estate_item(estate_item_id) ON DELETE CASCADE,"
      "conteudo text"
  ");",
  "CREATE TABLE IF NOT EXISTS configuration ("
      "configuration_id integer PRIMARY KEY DEFAULT 1,"
      "purchase_account_output_id integer REFERENCES accounting_account(account_id),"
      "sale_account_input_id integer REFERENCES accounting_account(account_id),"
      "currency_id integer REFERENCES monetary_currency(currency_id),"
      "date_format_id integer,"
      "number_format_id integer,"
      "sale_fee_validity_days integer,"
      "sale_fee real,"
      "sale_tax_rate_pis real,"
      "sale_tax_rate_cofins real,"
      "sale_tax_rate_csll real,"
      "sale_tax_rate_irpj real"
  ");",
  // DML
  // Countries
  "INSERT INTO location_country (country_id, nome) VALUES('1', 'Afghanistan');",
  "INSERT INTO location_country (country_id, nome) VALUES('2', 'Albania');",
  "INSERT INTO location_country (country_id, nome) VALUES('3', 'Algeria');",
  "INSERT INTO location_country (country_id, nome) VALUES('4', 'Andorra');",
  "INSERT INTO location_country (country_id, nome) VALUES('5', 'Angola');",
  "INSERT INTO location_country (country_id, nome) VALUES('6', 'Antigua and Barbuda');",
  "INSERT INTO location_country (country_id, nome) VALUES('7', 'Argentina');",
  "INSERT INTO location_country (country_id, nome) VALUES('8', 'Armenia');",
  "INSERT INTO location_country (country_id, nome) VALUES('9', 'Aruba');",
  "INSERT INTO location_country (country_id, nome) VALUES('10', 'Australia');",
  "INSERT INTO location_country (country_id, nome) VALUES('11', 'Austria');",
  "INSERT INTO location_country (country_id, nome) VALUES('12', 'Azerbaijan');",
  "INSERT INTO location_country (country_id, nome) VALUES('13', 'Bahamas, The');",
  "INSERT INTO location_country (country_id, nome) VALUES('14', 'Bahrain');",
  "INSERT INTO location_country (country_id, nome) VALUES('15', 'Bangladesh');",
  "INSERT INTO location_country (country_id, nome) VALUES('16', 'Barbados');",
  "INSERT INTO location_country (country_id, nome) VALUES('17', 'Belarus');",
  "INSERT INTO location_country (country_id, nome) VALUES('18', 'Belgium');",
  "INSERT INTO location_country (country_id, nome) VALUES('19', 'Belize');",
  "INSERT INTO location_country (country_id, nome) VALUES('20', 'Benin');",
  "INSERT INTO location_country (country_id, nome) VALUES('21', 'Bhutan');",
  "INSERT INTO location_country (country_id, nome) VALUES('22', 'Bolivia');",
  "INSERT INTO location_country (country_id, nome) VALUES('23', 'Bosnia and Herzegovina');",
  "INSERT INTO location_country (country_id, nome) VALUES('24', 'Botswana');",
  "INSERT INTO location_country (country_id, nome) VALUES('25', 'Brazil');",
  "INSERT INTO location_country (country_id, nome) VALUES('26', 'Brunei');",
  "INSERT INTO location_country (country_id, nome) VALUES('27', 'Bulgaria');",
  "INSERT INTO location_country (country_id, nome) VALUES('28', 'Burkina Faso');",
  "INSERT INTO location_country (country_id, nome) VALUES('29', 'Burma');",
  "INSERT INTO location_country (country_id, nome) VALUES('30', 'Burundi');",
  "INSERT INTO location_country (country_id, nome) VALUES('31', 'Cambodia');",
  "INSERT INTO location_country (country_id, nome) VALUES('32', 'Cameroon');",
  "INSERT INTO location_country (country_id, nome) VALUES('33', 'Canada');",
  "INSERT INTO location_country (country_id, nome) VALUES('34', 'Cape Verde');",
  "INSERT INTO location_country (country_id, nome) VALUES('35', 'Central African Republic');",
  "INSERT INTO location_country (country_id, nome) VALUES('36', 'Chad');",
  "INSERT INTO location_country (country_id, nome) VALUES('37', 'Chile');",
  "INSERT INTO location_country (country_id, nome) VALUES('38', 'China');",
  "INSERT INTO location_country (country_id, nome) VALUES('39', 'Colombia');",
  "INSERT INTO location_country (country_id, nome) VALUES('40', 'Comoros');",
  "INSERT INTO location_country (country_id, nome) VALUES('41', 'Congo, Democratic Republic of the');",
  "INSERT INTO location_country (country_id, nome) VALUES('42', 'Congo, Republic of the');",
  "INSERT INTO location_country (country_id, nome) VALUES('43', 'Costa Rica');",
  "INSERT INTO location_country (country_id, nome) VALUES('44', 'Cote d\'Ivoire');",
  "INSERT INTO location_country (country_id, nome) VALUES('45', 'Croatia');",
  "INSERT INTO location_country (country_id, nome) VALUES('46', 'Cuba');",
  "INSERT INTO location_country (country_id, nome) VALUES('47', 'Curacao');",
  "INSERT INTO location_country (country_id, nome) VALUES('48', 'Cyprus');",
  "INSERT INTO location_country (country_id, nome) VALUES('49', 'Czech Republic');",
  "INSERT INTO location_country (country_id, nome) VALUES('50', 'Denmark');",
  "INSERT INTO location_country (country_id, nome) VALUES('51', 'Djibouti');",
  "INSERT INTO location_country (country_id, nome) VALUES('52', 'Dominica');",
  "INSERT INTO location_country (country_id, nome) VALUES('53', 'Dominican Republic');",
  "INSERT INTO location_country (country_id, nome) VALUES('54', 'East Timor (see Timor-Leste)');",
  "INSERT INTO location_country (country_id, nome) VALUES('55', 'Ecuador');",
  "INSERT INTO location_country (country_id, nome) VALUES('56', 'Egypt');",
  "INSERT INTO location_country (country_id, nome) VALUES('57', 'El Salvador');",
  "INSERT INTO location_country (country_id, nome) VALUES('58', 'Equatorial Guinea');",
  "INSERT INTO location_country (country_id, nome) VALUES('59', 'Eritrea');",
  "INSERT INTO location_country (country_id, nome) VALUES('60', 'Estonia');",
  "INSERT INTO location_country (country_id, nome) VALUES('61', 'Ethiopia');",
  "INSERT INTO location_country (country_id, nome) VALUES('62', 'Fiji');",
  "INSERT INTO location_country (country_id, nome) VALUES('63', 'Finland');",
  "INSERT INTO location_country (country_id, nome) VALUES('64', 'France');",
  "INSERT INTO location_country (country_id, nome) VALUES('65', 'Gabon');",
  "INSERT INTO location_country (country_id, nome) VALUES('66', 'Gambia, The');",
  "INSERT INTO location_country (country_id, nome) VALUES('67', 'Georgia');",
  "INSERT INTO location_country (country_id, nome) VALUES('68', 'Germany');",
  "INSERT INTO location_country (country_id, nome) VALUES('69', 'Ghana');",
  "INSERT INTO location_country (country_id, nome) VALUES('70', 'Greece');",
  "INSERT INTO location_country (country_id, nome) VALUES('71', 'Grenada');",
  "INSERT INTO location_country (country_id, nome) VALUES('72', 'Guatemala');",
  "INSERT INTO location_country (country_id, nome) VALUES('73', 'Guinea');",
  "INSERT INTO location_country (country_id, nome) VALUES('74', 'Guinea-Bissau');",
  "INSERT INTO location_country (country_id, nome) VALUES('75', 'Guyana');",
  "INSERT INTO location_country (country_id, nome) VALUES('76', 'Haiti');",
  "INSERT INTO location_country (country_id, nome) VALUES('77', 'Holy See');",
  "INSERT INTO location_country (country_id, nome) VALUES('78', 'Honduras');",
  "INSERT INTO location_country (country_id, nome) VALUES('79', 'Hong Kong');",
  "INSERT INTO location_country (country_id, nome) VALUES('80', 'Hungary');",
  "INSERT INTO location_country (country_id, nome) VALUES('81', 'Iceland');",
  "INSERT INTO location_country (country_id, nome) VALUES('82', 'India');",
  "INSERT INTO location_country (country_id, nome) VALUES('83', 'Indonesia');",
  "INSERT INTO location_country (country_id, nome) VALUES('84', 'Iran');",
  "INSERT INTO location_country (country_id, nome) VALUES('85', 'Iraq');",
  "INSERT INTO location_country (country_id, nome) VALUES('86', 'Ireland');",
  "INSERT INTO location_country (country_id, nome) VALUES('87', 'Israel');",
  "INSERT INTO location_country (country_id, nome) VALUES('88', 'Italy');",
  "INSERT INTO location_country (country_id, nome) VALUES('89', 'Jamaica');",
  "INSERT INTO location_country (country_id, nome) VALUES('90', 'Japan');",
  "INSERT INTO location_country (country_id, nome) VALUES('91', 'Jordan');",
  "INSERT INTO location_country (country_id, nome) VALUES('92', 'Kazakhstan');",
  "INSERT INTO location_country (country_id, nome) VALUES('93', 'Kenya');",
  "INSERT INTO location_country (country_id, nome) VALUES('94', 'Kiribati');",
  "INSERT INTO location_country (country_id, nome) VALUES('95', 'Korea, North');",
  "INSERT INTO location_country (country_id, nome) VALUES('96', 'Korea, South');",
  "INSERT INTO location_country (country_id, nome) VALUES('97', 'Kosovo');",
  "INSERT INTO location_country (country_id, nome) VALUES('98', 'Kuwait');",
  "INSERT INTO location_country (country_id, nome) VALUES('99', 'Kyrgyzstan');",
  "INSERT INTO location_country (country_id, nome) VALUES('100', 'Laos');",
  "INSERT INTO location_country (country_id, nome) VALUES('101', 'Latvia');",
  "INSERT INTO location_country (country_id, nome) VALUES('102', 'Lebanon');",
  "INSERT INTO location_country (country_id, nome) VALUES('103', 'Lesotho');",
  "INSERT INTO location_country (country_id, nome) VALUES('104', 'Liberia');",
  "INSERT INTO location_country (country_id, nome) VALUES('105', 'Libya');",
  "INSERT INTO location_country (country_id, nome) VALUES('106', 'Liechtenstein');",
  "INSERT INTO location_country (country_id, nome) VALUES('107', 'Lithuania');",
  "INSERT INTO location_country (country_id, nome) VALUES('108', 'Luxembourg');",
  "INSERT INTO location_country (country_id, nome) VALUES('109', 'Macau');",
  "INSERT INTO location_country (country_id, nome) VALUES('110', 'Macedonia');",
  "INSERT INTO location_country (country_id, nome) VALUES('111', 'Madagascar');",
  "INSERT INTO location_country (country_id, nome) VALUES('112', 'Malawi');",
  "INSERT INTO location_country (country_id, nome) VALUES('113', 'Malaysia');",
  "INSERT INTO location_country (country_id, nome) VALUES('114', 'Maldives');",
  "INSERT INTO location_country (country_id, nome) VALUES('115', 'Mali');",
  "INSERT INTO location_country (country_id, nome) VALUES('116', 'Malta');",
  "INSERT INTO location_country (country_id, nome) VALUES('117', 'Marshall Islands');",
  "INSERT INTO location_country (country_id, nome) VALUES('118', 'Mauritania');",
  "INSERT INTO location_country (country_id, nome) VALUES('119', 'Mauritius');",
  "INSERT INTO location_country (country_id, nome) VALUES('120', 'Mexico');",
  "INSERT INTO location_country (country_id, nome) VALUES('121', 'Micronesia');",
  "INSERT INTO location_country (country_id, nome) VALUES('122', 'Moldova');",
  "INSERT INTO location_country (country_id, nome) VALUES('123', 'Monaco');",
  "INSERT INTO location_country (country_id, nome) VALUES('124', 'Mongolia');",
  "INSERT INTO location_country (country_id, nome) VALUES('125', 'Montenegro');",
  "INSERT INTO location_country (country_id, nome) VALUES('126', 'Morocco');",
  "INSERT INTO location_country (country_id, nome) VALUES('127', 'Mozambique');",
  "INSERT INTO location_country (country_id, nome) VALUES('128', 'Namibia');",
  "INSERT INTO location_country (country_id, nome) VALUES('129', 'Nauru');",
  "INSERT INTO location_country (country_id, nome) VALUES('130', 'Nepal');",
  "INSERT INTO location_country (country_id, nome) VALUES('131', 'Netherlands');",
  "INSERT INTO location_country (country_id, nome) VALUES('132', 'Netherlands Antilles');",
  "INSERT INTO location_country (country_id, nome) VALUES('133', 'New Zealand');",
  "INSERT INTO location_country (country_id, nome) VALUES('134', 'Nicaragua');",
  "INSERT INTO location_country (country_id, nome) VALUES('135', 'Niger');",
  "INSERT INTO location_country (country_id, nome) VALUES('136', 'Nigeria');",
  "INSERT INTO location_country (country_id, nome) VALUES('137', 'North Korea');",
  "INSERT INTO location_country (country_id, nome) VALUES('138', 'Norway');",
  "INSERT INTO location_country (country_id, nome) VALUES('139', 'Oman');",
  "INSERT INTO location_country (country_id, nome) VALUES('140', 'Pakistan');",
  "INSERT INTO location_country (country_id, nome) VALUES('141', 'Palau');",
  "INSERT INTO location_country (country_id, nome) VALUES('142', 'Palestinian Territories');",
  "INSERT INTO location_country (country_id, nome) VALUES('143', 'Panama');",
  "INSERT INTO location_country (country_id, nome) VALUES('144', 'Papua New Guinea');",
  "INSERT INTO location_country (country_id, nome) VALUES('145', 'Paraguay');",
  "INSERT INTO location_country (country_id, nome) VALUES('146', 'Peru');",
  "INSERT INTO location_country (country_id, nome) VALUES('147', 'Philippines');",
  "INSERT INTO location_country (country_id, nome) VALUES('148', 'Poland');",
  "INSERT INTO location_country (country_id, nome) VALUES('149', 'Portugal');",
  "INSERT INTO location_country (country_id, nome) VALUES('150', 'Qatar');",
  "INSERT INTO location_country (country_id, nome) VALUES('151', 'Romania');",
  "INSERT INTO location_country (country_id, nome) VALUES('152', 'Russia');",
  "INSERT INTO location_country (country_id, nome) VALUES('153', 'Rwanda');",
  "INSERT INTO location_country (country_id, nome) VALUES('154', 'Saint Kitts and Nevis');",
  "INSERT INTO location_country (country_id, nome) VALUES('155', 'Saint Lucia');",
  "INSERT INTO location_country (country_id, nome) VALUES('156', 'Saint Vincent and the Grenadines');",
  "INSERT INTO location_country (country_id, nome) VALUES('157', 'Samoa');",
  "INSERT INTO location_country (country_id, nome) VALUES('158', 'San Marino');",
  "INSERT INTO location_country (country_id, nome) VALUES('159', 'Sao Tome and Principe');",
  "INSERT INTO location_country (country_id, nome) VALUES('160', 'Saudi Arabia');",
  "INSERT INTO location_country (country_id, nome) VALUES('161', 'Senegal');",
  "INSERT INTO location_country (country_id, nome) VALUES('162', 'Serbia');",
  "INSERT INTO location_country (country_id, nome) VALUES('163', 'Seychelles');",
  "INSERT INTO location_country (country_id, nome) VALUES('164', 'Sierra Leone');",
  "INSERT INTO location_country (country_id, nome) VALUES('165', 'Singapore');",
  "INSERT INTO location_country (country_id, nome) VALUES('166', 'Sint Maarten');",
  "INSERT INTO location_country (country_id, nome) VALUES('167', 'Slovakia');",
  "INSERT INTO location_country (country_id, nome) VALUES('168', 'Slovenia');",
  "INSERT INTO location_country (country_id, nome) VALUES('169', 'Solomon Islands');",
  "INSERT INTO location_country (country_id, nome) VALUES('170', 'Somalia');",
  "INSERT INTO location_country (country_id, nome) VALUES('171', 'South Africa');",
  "INSERT INTO location_country (country_id, nome) VALUES('172', 'South Korea');",
  "INSERT INTO location_country (country_id, nome) VALUES('173', 'South Sudan');",
  "INSERT INTO location_country (country_id, nome) VALUES('174', 'Spain');",
  "INSERT INTO location_country (country_id, nome) VALUES('175', 'Sri Lanka');",
  "INSERT INTO location_country (country_id, nome) VALUES('176', 'Sudan');",
  "INSERT INTO location_country (country_id, nome) VALUES('177', 'Suriname');",
  "INSERT INTO location_country (country_id, nome) VALUES('178', 'Swaziland');",
  "INSERT INTO location_country (country_id, nome) VALUES('179', 'Sweden');",
  "INSERT INTO location_country (country_id, nome) VALUES('180', 'Switzerland');",
  "INSERT INTO location_country (country_id, nome) VALUES('181', 'Syria');",
  "INSERT INTO location_country (country_id, nome) VALUES('182', 'Taiwan');",
  "INSERT INTO location_country (country_id, nome) VALUES('183', 'Tajikistan');",
  "INSERT INTO location_country (country_id, nome) VALUES('184', 'Tanzania');",
  "INSERT INTO location_country (country_id, nome) VALUES('185', 'Thailand');",
  "INSERT INTO location_country (country_id, nome) VALUES('186', 'Timor-Leste');",
  "INSERT INTO location_country (country_id, nome) VALUES('187', 'Togo');",
  "INSERT INTO location_country (country_id, nome) VALUES('188', 'Tonga');",
  "INSERT INTO location_country (country_id, nome) VALUES('189', 'Trinidad and Tobago');",
  "INSERT INTO location_country (country_id, nome) VALUES('190', 'Tunisia');",
  "INSERT INTO location_country (country_id, nome) VALUES('191', 'Turkey');",
  "INSERT INTO location_country (country_id, nome) VALUES('192', 'Turkmenistan');",
  "INSERT INTO location_country (country_id, nome) VALUES('193', 'Tuvalu');",
  "INSERT INTO location_country (country_id, nome) VALUES('194', 'Uganda');",
  "INSERT INTO location_country (country_id, nome) VALUES('195', 'Ukraine');",
  "INSERT INTO location_country (country_id, nome) VALUES('196', 'United Arab Emirates');",
  "INSERT INTO location_country (country_id, nome) VALUES('197', 'United Kingdom');",
  "INSERT INTO location_country (country_id, nome) VALUES('198', 'United States');",
  "INSERT INTO location_country (country_id, nome) VALUES('199', 'Uruguay');",
  "INSERT INTO location_country (country_id, nome) VALUES('200', 'Uzbekistan');",
  "INSERT INTO location_country (country_id, nome) VALUES('201', 'Vanuatu');",
  "INSERT INTO location_country (country_id, nome) VALUES('202', 'Venezuela');",
  "INSERT INTO location_country (country_id, nome) VALUES('203', 'Vietnam');",
  "INSERT INTO location_country (country_id, nome) VALUES('204', 'Yemen');",
  "INSERT INTO location_country (country_id, nome) VALUES('205', 'Zambia');",
  "INSERT INTO location_country (country_id, nome) VALUES('206', 'Zimbabwe');",
  // Banks
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(1, 'Itaú Unibanco', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(2, 'Bradesco', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(3, 'Banco Safra', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(4, 'Banco do Brasil', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(5, 'Banrisul (state-owned)', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(6, 'Caixa Econômica Federal', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(7, 'Banestes (state-owned)', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(8, 'Banco do Estado de Pernambuco (Bandepe) Acquired by Banco Real', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(9, 'Banco do Estado do Rio de Janeiro (Banerj) Acquired by Itaú', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(10, 'Banco do Estado de Minas Gerais (Bemge) Acquired by Itaú', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(11, 'Banco do Estado do Paraná (Banestado) Acquired by Itaú', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(12, 'Banco do Estado de Santa Catarina (BESC) Acquired by Banco do Brasil', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(13, 'Banco do Estado do Piauí (BEP) Acquired by Banco do Brasil', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(14, 'Banco Real Owned by ABN AMRO; ownership will transfer to Banco Santander Banespa', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(15, 'Citibank (Citigroup)', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(16, 'HSBC Bank Brasil', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(17, 'Banco Santander Banespa, owned by Banco Santander', 25);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(18, 'JPMorgan Chase', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(19, 'Bank of America', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(20, 'Citigroup', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(21, 'Wells Fargo', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(22, 'The Bank of New York Mellon', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(23, 'U.S. Bancorp', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(24, 'HSBC Bank USA', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(25, 'PNC Financial Services', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(26, 'Capital One', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(27, 'State Street Corporation', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(28, 'TD Bank, N.A.', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(29, 'BB&T', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(30, 'SunTrust Banks', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(31, 'American Express', 198);",
  "INSERT INTO bank (bank_id, nome, country_id) VALUES(32, 'Ally Financial', 198);",
  // Currency
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (1, 'USD ($)', 'United States dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (2, 'BRL (R$)', 'Brazilian real', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (3, '$', 'British Virgin Islands dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (4, '$', 'Cook Islands dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (5, '$', 'Kiribati dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (6, '$', 'Micronesian dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (7, '$', 'Nauruan dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (8, '$', 'Niue dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (9, '$', 'Palauan dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (10, '$', 'Pitcairn Islands dollar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (11, '$', 'Tuvaluan dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (12, 'DRAM', 'Nagorno-Karabakh dram[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (13, 'APSAR (аҧсар)', 'Abkhazian apsar[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (14, 'Ptas', 'Sahrawi peseta', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (15, 'Sh', 'Somaliland shilling', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (16, 'kr', 'Faroese króna', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (17, '£', 'Alderney pound[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (18, '£', 'Ascension pound[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (19, '£', 'Guernsey pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (20, '£', 'South Georgia and the South Sandwich Islands pound[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (21, '£', 'Tristan da Cunha pound[A]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (22, 'AED (د.إ)', 'United Arab Emirates dirham', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (23, 'AFN (؋)', 'Afghan afghani', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (24, 'ALL (L)', 'Albanian lek', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (25, 'AMD (Դրամ)', 'Armenian dram', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (26, 'ANG (ƒ)', 'Netherlands Antillean guilder', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (27, 'AOA (Kz)', 'Angolan kwanza', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (28, 'ARS ($)', 'Argentine peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (29, 'AUD ($)', 'Australian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (30, 'AWG (ƒ)', 'Aruban florin', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (31, 'AZN (qəpik)', 'Azerbaijani manat', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (32, 'BAM (KM)', 'Bosnia and Herzegovina convertible mark', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (33, 'BBD ($)', 'Barbadian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (34, 'BDT (৳)', 'Bangladeshi taka', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (35, 'BGN (лв.)', 'Bulgarian lev', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (36, 'BHD (.د.ب)', 'Bahraini dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (37, 'BIF (Fr)', 'Burundian franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (38, 'BMD ($)', 'Bermudian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (39, 'BND ($)', 'Brunei dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (40, 'BOB (Bs.)', 'Bolivian boliviano', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (41, 'BSD ($)', 'Bahamian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (42, 'BTN (Nu.)', 'Bhutanese ngultrum', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (43, 'BWP (P)', 'Botswana pula', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (44, 'BYR (Br)', 'Belarusian ruble', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (45, 'BZD ($)', 'Belize dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (46, 'CAD ($)', 'Canadian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (47, 'CDF (Fr)', 'Congolese franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (48, 'CHF (Fr)', 'Swiss franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (49, 'CLP ($)', 'Chilean peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (50, 'CNY (¥)', 'Chinese yuan', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (51, 'COP ($)', 'Colombian peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (52, 'CRC (₡)', 'Costa Rican colón', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (53, 'CUC ($)', 'Cuban convertible peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (54, 'CUP ($)', 'Cuban peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (55, 'CVE (Esc or $)', 'Cape Verdean escudo', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (56, 'CZK (Kč)', 'Czech koruna', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (57, 'DJF (Fr)', 'Djiboutian franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (58, 'DKK (kr)', 'Danish krone', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (59, 'DOP ($)', 'Dominican peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (60, 'DZD (دج)', 'Algerian dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (61, 'EGP (E£)', 'Egyptian pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (62, 'ERN (Nfk)', 'Eritrean nakfa', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (63, 'ETB (Br)', 'Ethiopian birr', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (64, 'EUR (€)', 'Euro', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (65, 'FJD ($)', 'Fijian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (66, 'FKP (£)', 'Falkland Islands pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (67, 'GBP (£)', 'British pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (68, 'GBP (£)', 'British pound[B]', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (69, 'GEL (ლარი)', 'Georgian lari', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (70, 'GGP (£)', 'Guernsey pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (71, 'GHS (GH₵)', 'Ghana cedi', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (72, 'GIP (£)', 'Gibraltar pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (73, 'GMD (D)', 'Gambian dalasi', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (74, 'GNF (Fr)', 'Guinean franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (75, 'GTQ (Q)', 'Guatemalan quetzal', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (76, 'GYD ($)', 'Guyanese dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (77, 'HKD ($)', 'Hong Kong dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (78, 'HNL (L)', 'Honduran lempira', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (79, 'HRK (kn)', 'Croatian kuna', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (80, 'HTG (G)', 'Haitian gourde', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (81, 'HUF (Ft)', 'Hungarian forint', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (82, 'IDR (Rp)', 'Indonesian rupiah', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (83, 'ILS (₪)', 'Israeli new shekel', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (84, 'IMP[C] (£)', 'Manx pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (85, 'INR (Rs)', 'Indian rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (86, 'IQD (ع.د)', 'Iraqi dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (87, 'IRR (ریال ایران)', 'Iranian rial', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (88, 'ISK (kr)', 'Icelandic króna', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (89, 'JEP[C] (£)', 'Jersey pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (90, 'JMD ($)', 'Jamaican dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (91, 'JOD (دينار)', 'Jordanian dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (92, 'JPY (¥)', 'Japanese yen', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (93, 'KES (Sh)', 'Kenyan shilling', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (94, 'KGS (som)', 'Kyrgyzstani som', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (95, 'KHR (៛)', 'Cambodian riel', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (96, 'KMF (Fr)', 'Comorian franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (97, 'KPW (₩)', 'North Korean won', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (98, 'KRW (₩)', 'South Korean won', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (99, 'KWD (د.ك)', 'Kuwaiti dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (100, 'KYD ($)', 'Cayman Islands dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (101, 'KZT (₸)', 'Kazakhstani tenge', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (102, 'LAK (₭)', 'Lao kip', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (103, 'LBP (ل.ل)', 'Lebanese pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (104, 'LKR (Rs)', 'Sri Lankan rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (105, 'LRD ($)', 'Liberian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (106, 'LSL (L)', 'Lesotho loti', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (107, 'LYD (ل.د)', 'Libyan dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (108, 'MAD (د.م.)', 'Moroccan dirham', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (110, 'MDL (L)', 'Moldovan leu', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (111, 'MGA (Ar)', 'Malagasy ariary', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (112, 'MKD (ден)', 'Macedonian denar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (113, 'MMK (Ks)', 'Burmese kyat', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (114, 'MNT (₮)', 'Mongolian tögrög', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (115, 'MOP (P)', 'Macanese pataca', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (116, 'MRO (UM)', 'Mauritanian ouguiya', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (117, 'MUR (₨)', 'Mauritian rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (118, 'MVR (Rf)', 'Maldivian rufiyaa', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (119, 'MWK (MK)', 'Malawian kwacha', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (120, 'MXN ($)', 'Mexican peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (121, 'MYR (RM)', 'Malaysian ringgit', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (122, 'MZN (MT)', 'Mozambican metical', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (123, 'NAD ($)', 'Namibian dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (124, 'NGN (₦)', 'Nigerian naira', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (125, 'NIO (C$)', 'Nicaraguan córdoba', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (126, 'NOK (kr)', 'Norwegian krone', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (127, 'NPR (Rs)', 'Nepalese rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (128, 'NZD ($)', 'New Zealand dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (129, 'OMR (ر.ع.)', 'Omani rial', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (130, 'PAB (B/.)', 'Panamanian balboa', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (131, 'PEN (S/.)', 'Peruvian nuevo sol', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (132, 'PGK (K)', 'Papua New Guinean kina', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (133, 'PHP (₱)', 'Philippine peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (134, 'PKR (₨)', 'Pakistani rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (135, 'PLN (zł)', 'Polish złoty', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (136, 'PRB (ruble)', 'Transnistrian ruble', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (137, 'PYG (₲)', 'Paraguayan guaraní', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (138, 'QAR (ر.ق)', 'Qatari riyal', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (139, 'RON (lei)', 'Romanian leu', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (140, 'RSD (РСД)', 'Serbian dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (141, 'RUB (₽)', 'Russian ruble', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (144, 'RWF (Fr)', 'Rwandan franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (145, 'SAR (ر.س)', 'Saudi riyal', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (146, 'SBD ($)', 'Solomon Islands dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (147, 'SCR (SR)', 'Seychellois rupee', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (148, 'SDG (£)', 'Sudanese pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (149, 'SEK (kr)', 'Swedish krona', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (150, 'SGD ($)', 'Singapore dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (151, 'SHP (£)', 'Saint Helena pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (152, 'SLL (Le)', 'Sierra Leonean leone', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (153, 'SOS (Sh)', 'Somali shilling', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (154, 'SRD ($)', 'Surinamese dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (155, 'SSP (£)', 'South Sudanese pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (156, 'STD (Db)', 'São Tomé and Príncipe dobra', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (157, 'SYP (£)', 'Syrian pound', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (158, 'SZL (L)', 'Swazi lilangeni', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (159, 'THB (฿)', 'Thai baht', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (160, 'TJS (cомонӣ)', 'Tajikistani somoni', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (161, 'TMT (T)', 'Turkmenistan manat', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (162, 'TND (د.ت)', 'Tunisian dinar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (163, 'TOP (T$)', 'Tongan paʻanga', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (164, 'TRY (₺)', 'Turkish lira', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (165, 'TTD ($)', 'Trinidad and Tobago dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (166, 'TWD ($)', 'New Taiwan dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (167, 'TZS (Sh)', 'Tanzanian shilling', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (168, 'UAH (₴)', 'Ukrainian hryvnia', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (169, 'UGX (Sh)', 'Ugandan shilling', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (170, 'UYU ($)', 'Uruguayan peso', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (171, 'UZS (soʻm)', 'Uzbekistani som', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (172, 'VEF (Bs F)', 'Venezuelan bolívar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (173, 'VND (₫)', 'Vietnamese dong', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (174, 'VUV (Vt)', 'Vanuatu vatu', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (175, 'WST (T)', 'Samoan tālā', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (176, 'XAF (Fr)', 'Central African CFA franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (177, 'XCD ($)', 'East Caribbean dollar', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (178, 'XOF (Fr)', 'West African CFA franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (179, 'XPF (Fr)', 'CFP franc', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (180, 'YER (rial)', 'Yemeni rial', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (181, 'ZAR (R)', 'South African rand', '0');",
  "INSERT INTO monetary_currency (currency_id, sigla, nome, padrao) VALUES (182, 'ZMW (ZK)', 'Zambian kwacha', '0');",

  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(1, 'Car');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(2, 'Children');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(3, 'Clothes');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(4, 'Depreciation');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(5, 'Food');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(6, 'Fuel');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(7, 'Education');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(8, 'Health');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(9, 'Home');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(10, 'Insurance');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(11, 'Internet');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(12, 'Mortgage');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(13, 'Others');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(14, 'Parking');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(15, 'Phone');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(16, 'Recreation');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(17, 'Rent');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(18, 'School');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(19, 'Shopping');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(20, 'Tax');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(21,  'Transport');",
  "INSERT INTO finance_expense_category (expense_category_id, nome) VALUES(22, 'Travel');",

  "INSERT INTO finance_income_category (income_category_id, nome) VALUES(1, 'Salary and Wages');",
  "INSERT INTO finance_income_category (income_category_id, nome) VALUES(2, 'Earnings from Self-Employment');",
  "INSERT INTO finance_income_category (income_category_id, nome) VALUES(3, 'Interest');",
  "INSERT INTO finance_income_category (income_category_id, nome) VALUES(4, 'Dividend');"
};

#define DB_SETUP_CMD_LIST_COUNT_SQLITE 507


DBSchemaHash DBSqlite::dbSchemaHash;

/* Database class functions */

DBSqlite::DBSqlite()
{
  DBSqlite::InitializeSchema();
}


DBSqlite::~DBSqlite()
{
}


DBSqlite *DBSqlite::GetInstance()
{
  // This line only runs once, thus creating the only existent instance

  static DBSqlite *instance = new DBSqlite();

  return instance; // Always returns the same instance
}


void DBSqlite::InitializeSchema()
{
  // Configuration
  DBSqlite::dbSchemaHash["configuration"] = "configuration";

  DBSqlite::dbSchemaHash["location.country"] = "location_country";

  DBSqlite::dbSchemaHash["person.address"] = "person_address";

  // Monetary
  DBSqlite::dbSchemaHash["monetary.currency"] = "monetary_currency";

  // Financial
  DBSqlite::dbSchemaHash["financial.income"] = "finance_income";
  DBSqlite::dbSchemaHash["financial.income.notes"] = "finance_income_notes";
  DBSqlite::dbSchemaHash["financial.income.category"] = "finance_income_category";
  DBSqlite::dbSchemaHash["financial.income.payment.installment"] = "finance_income_installment";

  DBSqlite::dbSchemaHash["financial.expense"] = "finance_expense";
  DBSqlite::dbSchemaHash["financial.expense.notes"] = "finance_expense_notes";
  DBSqlite::dbSchemaHash["financial.expense.category"] = "finance_expense_category";
  DBSqlite::dbSchemaHash["financial.expense.payment.installment"] = "finance_expense_installment";

  DBSqlite::dbSchemaHash["accounting.account.subtype"] = "accounting_account_subtype";
  DBSqlite::dbSchemaHash["accounting.account"] = "accounting_account";

  // Estate
  DBSqlite::dbSchemaHash["estate.item"] = "estate_item";

  // Owner
  DBSqlite::dbSchemaHash["owner"] = "owner";
  DBSqlite::dbSchemaHash["owner.picture"] = "owner_picture";

  // Bank
  DBSqlite::dbSchemaHash["bank"] = "bank";
  DBSqlite::dbSchemaHash["bank.account"] = "bank_account";
  DBSqlite::dbSchemaHash["bank.account.type"] = "bank_account_type";
  DBSqlite::dbSchemaHash["bank.account.event"] = "bank_account_event";
  DBSqlite::dbSchemaHash["bank.account.event.type"] = "bank_account_event_type";

  DBSqlite::dbSchemaHash["accounting.account"] = "accounting_account";
  DBSqlite::dbSchemaHash["accounting.account.subtype"] = "accounting_account_subtype";

  DBSqlite::dbSchemaHash["person"] = "person";
  DBSqlite::dbSchemaHash["person.type"] = "person_type";

  DBSqlite::dbSchemaHash["estate.item"] = "estate_item";
}


int DBSqlite::Setup()
{
  DBSqliteQuery dbquery;

  LogMessenger *messenger = LogMessenger::GetInstance();

  sqlite3_stmt *stmt = NULL;

  for (int cmdIndex = 0; cmdIndex < DB_SETUP_CMD_LIST_COUNT_SQLITE; cmdIndex++) {
    messenger->Register(wxString::Format(wxT("Setup command %d: \"%s\""),
                        cmdIndex, db_setup_cmd_list_sqlite[cmdIndex]), LogMessage::MSG_DBG);

    // Prepare command

    if (sqlite3_prepare((sqlite3 *) DB::connection,
                        db_setup_cmd_list_sqlite[cmdIndex],
                        strlen(db_setup_cmd_list_sqlite[cmdIndex]),
                        &stmt,
                        0) != SQLITE_OK) {

      messenger->Register("Could not prepare database statement.", LogMessage::MSG_ERROR);

      continue;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
      messenger->Register(wxString::Format("Could not step on database statement."), LogMessage::MSG_ERROR);

      continue;
    }

    sqlite3_reset(stmt);
  }

  sqlite3_finalize(stmt);

  return 0;
}


wxString DBSqlite::GetTableName(wxString name)
{
  return DBSqlite::dbSchemaHash[name];
}


wxString DBSqlite::GetTableName(const char *name)
{
  return DBSqlite::GetTableName(wxString(name));
}


int DBSqlite::Connect(wxString filePath,
                      wxString arg2,
                      wxString arg3,
                      wxString arg4,
                      wxString arg5,
                      wxString arg6)
{
  DB::filePath = filePath;

  LogMessenger *messenger = LogMessenger::GetInstance();

  // Connecting with the database


  char filePathAnsi[strlen((const char *) DB::filePath.mb_str(wxConvUTF8)) + 1];

  strcpy(filePathAnsi, (const char *) DB::filePath.mb_str(wxConvUTF8));

  messenger->Register("Database path: \n" + filePath);

  int rc = sqlite3_open_v2(filePathAnsi,
                           (sqlite3 **) &DB::connection,
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                           NULL);

  if (rc) {
    messenger->Register(wxString::Format("Can't open database: %s\n", sqlite3_errmsg((sqlite3 *) DB::connection)));

    sqlite3_close((sqlite3 *) DB::connection);

    DB::connected = false;

    return -1;
  }

  DB::connected = true;

  /*
  // Enable foreign keys

  wxString sql = "PRAGMA foreign_keys = ON";

  DBSqliteQuery dbquery;

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Cannot enable foreign keys in database."), LogMessage::MSG_ERROR);
  }
  */

  return 0;
}


int DBSqlite::Connect()
{
  return DBSqlite::Connect(Config::Get("db:filepath"));
}


void DBSqlite::Disconnect()
{
  if (!DB::connected) {
    return;
  }

  sqlite3_close((sqlite3 *) DB::connection);

  DB::connected = false;
}


bool DBSqlite::IsConnected()
{
  return DB::connected;
}


/* Database [result / result set] class functions*/

DBSqliteResult::DBSqliteResult()
{
  this->res = NULL;

  this->freed = false;
}


DBSqliteResult::~DBSqliteResult()
{
  Free();
}


void DBSqliteResult::Free(void) {
  if (this->res == NULL) {
    return;
  }

  if (this->freed) {
    return;
  }

  sqlite3_reset((sqlite3_stmt *) this->res);

  this->res = NULL;

  this->freed = true;

  this->resColumnNames.clear();

  for (unsigned int rowIndex = 0; rowIndex < this->matrixData.size(); rowIndex++) {
    this->matrixData[rowIndex].clear();
  }

  this->matrixData.clear();
}


bool DBSqliteResult::IsOk()
{
  /*
  if (this->res == NULL) {
    return false;
  }

  ExecStatusType status = PQresultStatus((PGresult *) this->res);

  if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
    return false;
  }

  return true;
  */
  return true;
}


int DBSqliteResult::GetFieldsCount()
{
  if (this->matrixData.size() > 0)
    return this->matrixData[0].size();

  return 0;
}


int DBSqliteResult::GetRowsCount()
{
  return this->matrixData[0].size();
}


int DBSqliteResult::GetAffectedRowsCount()
{
  return 0;
}


int DBSqliteResult::GetColumnIndexByName(wxString name)
{
  int columnIndex = -1;

  for (unsigned int colIndex = 0; colIndex < this->resColumnNames.size(); colIndex++) {
    if (this->resColumnNames[colIndex].Cmp(name) == 0) {
      columnIndex = colIndex;
      //printf("Field name found at %d column\n", colIndex);
      break;
    }
  }

  return columnIndex;
}


int DBSqliteResult::GetFieldLengthByName(unsigned int rowIndex, wxString name)
{
  if (rowIndex >= this->matrixData.size()) {
    return -1;
  }

  int columnIndex = GetColumnIndexByName(name);

  if (columnIndex < 0) {
    return -1;
  }

  return this->matrixData[rowIndex][columnIndex].Length();
}


int DBSqliteResult::GetFieldLength(unsigned int rowIndex, unsigned int colIndex)
{
  return this->matrixData[rowIndex][colIndex].Length();
}


wxString DBSqliteResult::GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue)
{
  if (rowIndex >= this->matrixData.size()) {
    return defaultValue;
  }

  int columnIndex = GetColumnIndexByName(name);

  if (columnIndex < 0) {
    return defaultValue;
  }

  return this->matrixData[rowIndex][columnIndex];
}


const char *DBSqliteResult::GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue)
{
  if (rowIndex >= this->matrixData.size()) {
    return NULL;
  }

  int columnIndex = GetColumnIndexByName(name);

  if (columnIndex < 0) {
    return NULL;
  }

  char *fieldValue;

  fieldValue = (char *) malloc(strlen((const char *) this->matrixData[rowIndex][columnIndex].mb_str(wxConvUTF8)) + 1);

  strcpy(fieldValue, (const char *) this->matrixData[rowIndex][columnIndex].mb_str(wxConvUTF8));

  return (char *) fieldValue;
}


int DBSqliteResult::GetFieldIntByName(unsigned int rowIndex, wxString name)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0;
  }

  int colIndex = GetColumnIndexByName(name);

  if (colIndex < 0) {
    return 0;
  }

  if (this->matrixData[rowIndex][colIndex].Length() == 0) {
    return 0;
  }

  long fieldValue;

  this->matrixData[rowIndex][colIndex].ToLong(&fieldValue);

  return (int) fieldValue;
}


long DBSqliteResult::GetFieldLongByName(unsigned int rowIndex, wxString name)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0;
  }

  int colIndex = GetColumnIndexByName(name);

  if (colIndex < 0) {
    return 0;
  }

  if (this->matrixData[rowIndex][colIndex].Length() == 0) {
    return 0;
  }

  long fieldValue;

  this->matrixData[rowIndex][colIndex].ToLong(&fieldValue);

  return fieldValue;
}


float DBSqliteResult::GetFieldFloatByName(unsigned int rowIndex, wxString name)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0.0;
  }

  int colIndex = GetColumnIndexByName(name);

  if (colIndex < 0) {
    return 0.0;
  }

  if (this->matrixData[rowIndex][colIndex].Length() == 0) {
    return 0.0;
  }

  double fieldValue;

  this->matrixData[rowIndex][colIndex].ToDouble(&fieldValue);

  return (float) fieldValue;
}


double DBSqliteResult::GetFieldDoubleByName(unsigned int rowIndex, wxString name)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0.0;
  }

  int colIndex = GetColumnIndexByName(name);

  if (colIndex < 0) {
    return 0.0;
  }

  if (this->matrixData[rowIndex][colIndex].Length() == 0) {
    return 0.0;
  }

  double fieldValue;

  this->matrixData[rowIndex][colIndex].ToDouble(&fieldValue);

  return fieldValue;
}


wxString DBSqliteResult::GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue)
{
  if (rowIndex >= this->matrixData.size()) {
    return "";
  }

  if (colIndex >= this->matrixData[rowIndex].size()) {
    return "";
  }

  return this->matrixData[rowIndex][colIndex];
}


int DBSqliteResult::GetFieldInt(unsigned int rowIndex, unsigned int colIndex)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0;
  }

  if (colIndex >= this->matrixData[rowIndex].size()) {
    return 0;
  }

  long fieldValue;

  this->matrixData[rowIndex][colIndex].ToLong(&fieldValue);

  return (int) fieldValue;
}


long DBSqliteResult::GetFieldLong(unsigned int rowIndex, unsigned int colIndex)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0;
  }

  if (colIndex >= this->matrixData[rowIndex].size()) {
    return 0;
  }

  long fieldValue;

  this->matrixData[rowIndex][colIndex].ToLong(&fieldValue);

  return fieldValue;
}


float DBSqliteResult::GetFieldFloat(unsigned int rowIndex, unsigned int colIndex)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0.0;
  }

  if (colIndex >= this->matrixData[rowIndex].size()) {
    return 0.0;
  }

  double fieldValue;

  this->matrixData[rowIndex][colIndex].ToDouble(&fieldValue);

  return (float) fieldValue;
}


double DBSqliteResult::GetFieldDouble(unsigned int rowIndex, unsigned int colIndex)
{
  if (rowIndex >= this->matrixData.size()) {
    return 0.0;
  }

  if (colIndex >= this->matrixData[rowIndex].size()) {
    return 0.0;
  }

  double fieldValue;

  this->matrixData[rowIndex][colIndex].ToDouble(&fieldValue);

  return fieldValue;
}



DBSqliteQuery::DBSqliteQuery()
{
  if (this->params.size() > 0) {
    this->params.clear();
  }

  this->dbError = false;
}


DBSqliteQuery::~DBSqliteQuery()
{
  this->result.Free();
}


void DBSqliteQuery::Reset()
{
  this->result.Free();

  if (this->params.size() > 0) {
     this->params.clear();
  }

  this->messageStr = wxT("");

  this->dbError = false;
}


wxString DBSqliteQuery::GetMessageStr(void)
{
  if (DB::connection == NULL) {
    return "";
  }

  int error_code = sqlite3_errcode((sqlite3 *) DB::connection);

  if (sqlite3_errmsg((sqlite3 *) DB::connection) != NULL) {
    return wxString::Format("Code: %d, Message: %s", error_code, sqlite3_errmsg((sqlite3 *) DB::connection));
  }
  else {
    return "";
  }
}


void DBSqliteQuery::FreeResult()
{
  this->result.Free();
}


int DBSqliteQuery::Execute(wxString command)
{
  sqlite3_stmt *stmt;

  LogMessenger *messenger = LogMessenger::GetInstance();

  if (command.Length() < 1) {
    messenger->Register(wxT("Comando vazio passado como consulta ao banco de dados."), LogMessage::MSG_ERROR);

    return -1;
  }

  //messenger->Register("Database command: " + command, LogMessage::MSG_DBG);

  Reset();

  char sqlCommandAnsi[strlen((const char *) command.mb_str(wxConvUTF8)) + 1];

  strcpy(sqlCommandAnsi, (const char *) command.mb_str(wxConvUTF8));

  // Prepare command

  if (sqlite3_prepare((sqlite3 *) DB::connection,
                      (const char *) command.mb_str(wxConvUTF8),
                      -1,    // If than zero, then stmt is read up to the first nul terminator
                      &stmt, 0) != SQLITE_OK) {

    messenger->Register("Could not prepare statement.", LogMessage::MSG_ERROR);

    return -1;
  }

  // Execute command

  if (command.StartsWith("SELECT")) {
    int recordIndex = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      //messenger->Register(wxString::Format("Row %d", recordIndex), LogMessage::MSG_DBG);

      for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
        this->result.resColumnNames.push_back(wxString((const char *) sqlite3_column_name(stmt, colIndex),
                                                       wxConvUTF8,
                                                       strlen((const char *) sqlite3_column_name(stmt, colIndex))));
      }

      std::vector <wxString> colValues;

      for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
        printf("Col %d = %s\n", colIndex, sqlite3_column_text(stmt, colIndex));

        if (sqlite3_column_text(stmt, colIndex) == NULL) {
          colValues.push_back("");
        }
        else {
          colValues.push_back(wxString((const char *) sqlite3_column_text(stmt, colIndex),
                                                                    wxConvUTF8,
                                                                    strlen((const char *) sqlite3_column_text(stmt, colIndex))));
        }
      }

      this->result.matrixData.push_back(colValues);

      recordIndex++;
    }
  }
  else {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      messenger->Register(wxString::Format("Could not step on database statement."), LogMessage::MSG_ERROR);

      return -1;
    }
  }

  this->result.res = (void *) stmt;

  this->result.freed = false;

  return 0;
}


int DBSqliteQuery::Execute(wxString command,
                           std::vector<wxString> params,
                           std::vector<wxString> paramTypes)
{
  sqlite3_stmt *stmt;
  char *paramValues[params.size()];

  LogMessenger *messenger = LogMessenger::GetInstance();

  if (command.Length() < 1) {
    messenger->Register(wxT("Comando vazio passado como consulta ao banco de dados."), LogMessage::MSG_ERROR);

    return -1;
  }

  //messenger->Register("Database command: " + command, LogMessage::MSG_DBG);

  Reset();

  char sqlCommandAnsi[strlen((const char *) command.mb_str(wxConvUTF8)) + 1];

  strcpy(sqlCommandAnsi, (const char *) command.mb_str(wxConvUTF8));

  // Prepare command

  if (sqlite3_prepare((sqlite3 *) DB::connection,
                      (const char *) command.mb_str(wxConvUTF8),
                      -1,    // If than zero, then stmt is read up to the first nul terminator
                      &stmt,
                      0) != SQLITE_OK) {

    messenger->Register("Could not prepare statement.", LogMessage::MSG_ERROR);

    return -1;
  }

  // Bind parameters

  if (params.size() >0) {
    for (unsigned int paramIndex = 0; paramIndex < params.size(); paramIndex++) {
      if (params.at(paramIndex) == wxEmptyString) {
        paramValues[paramIndex] = NULL;

        if (sqlite3_bind_null(stmt, paramIndex + 1) != SQLITE_OK) {
          messenger->Register("Could not bind double.", LogMessage::MSG_ERROR);

          return 1;
        }
      }
      else {
        char *value = (char *) malloc(sizeof(char) * strlen((const char *) params.at(paramIndex).mb_str(wxConvUTF8)) + 1);

        strcpy(value, (const char *) params.at(paramIndex).mb_str(wxConvUTF8));

        paramValues[paramIndex] = value;

        if (sqlite3_bind_text(stmt,
                              paramIndex + 1,
                              paramValues[paramIndex],
                              strlen(paramValues[paramIndex]),
                              SQLITE_TRANSIENT) != SQLITE_OK) {
          printf("\nCould not bind parameter. Database error: %s\n", sqlite3_errmsg((sqlite3 *) DB::connection));

          return -1;
        }
      }
    }
  }

  // Execute command

  if (command.StartsWith("SELECT")) {
    int recordIndex = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      //messenger->Register(wxString::Format("Row %d", recordIndex), LogMessage::MSG_DBG);

      for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
        this->result.resColumnNames.push_back(wxString((const char *) sqlite3_column_name(stmt, colIndex),
                                                       wxConvUTF8,
                                                       strlen((const char *) sqlite3_column_name(stmt, colIndex))));
      }

      std::vector <wxString> colValues;

      for (int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
        //printf("Col %d = %s\n", colIndex, sqlite3_column_text(stmt, colIndex));

        if (sqlite3_column_text(stmt, colIndex) == NULL) {
          colValues.push_back("");
        }
        else {
          colValues.push_back(wxString((const char *) sqlite3_column_text(stmt, colIndex),
                                                                    wxConvUTF8,
                                                                    strlen((const char *) sqlite3_column_text(stmt, colIndex))));
        }
      }

      this->result.matrixData.push_back(colValues);

      recordIndex++;
    }
  }
  else {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      messenger->Register(wxString::Format("Could not step on database statement."), LogMessage::MSG_ERROR);

      return -1;
    }
  }

  this->result.res = (void *) stmt;

  this->result.freed = false;

  // Free memory allocated by parameters

  for (unsigned int paramIndex = 0; paramIndex < params.size(); paramIndex++) {
    if (paramValues[paramIndex] != NULL) {
      free(paramValues[paramIndex]);
    }
  }

  return 0;
}


int DBSqliteQuery::GetResultStatus()
{
  // ExecStatusType status = PQresultStatus((PGresult *) this->result.res);

  // if (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK) {
  //   return (int) DBSqliteResult::WX_DATABASE_RS_NO_ERROR;
  // }
  // else if (status == PGRES_COPY_OUT) {
  //   return (int) DBSqliteResult::WX_DATABASE_RS_COPY_OUT;
  // }
  // else if (status == PGRES_COPY_IN) {
  //   return (int) DBSqliteResult::WX_DATABASE_RS_COPY_IN;
  // }
  // else if (status == PGRES_NONFATAL_ERROR) {
  //   return (int) DBSqliteResult::WX_DATABASE_RS_NONFATAL_ERROR;
  // }
  // else {
  //   return (int) DBSqliteResult::WX_DATABASE_RS_ERROR;
  // }
  return 0;
}


int DBSqliteQuery::GetFieldsCount()
{
  if (this->result.matrixData.size() > 0)
    return this->result.matrixData[0].size();

  return 0;
}


int DBSqliteQuery::GetRowsCount()
{
  return this->result.matrixData.size();
}


int DBSqliteQuery::GetAffectedRowsCount()
{
  return this->result.GetAffectedRowsCount();
}


int DBSqliteQuery::GetFieldLengthByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldLengthByName(rowIndex, name);
}


int DBSqliteQuery::GetFieldLength(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldLength(rowIndex, colIndex);
}


const char *DBSqliteQuery::GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue)
{
  return this->result.GetFieldAnsiStrByName(rowIndex, name, defaultValue);
}


wxString DBSqliteQuery::GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue)
{
  return this->result.GetFieldStrByName(rowIndex, name, defaultValue);
}


int DBSqliteQuery::GetFieldIntByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldIntByName(rowIndex, name);
}


long DBSqliteQuery::GetFieldLongByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldLongByName(rowIndex, name);
}


float DBSqliteQuery::GetFieldFloatByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldFloatByName(rowIndex, name);
}


double DBSqliteQuery::GetFieldDoubleByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldDoubleByName(rowIndex, name);
}


wxString DBSqliteQuery::GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue)
{
  return this->result.GetFieldStr(rowIndex, colIndex, defaultValue);
}


int DBSqliteQuery::GetFieldInt(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldInt(rowIndex, colIndex);
}


long DBSqliteQuery::GetFieldLong(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldLong(rowIndex, colIndex);
}


float DBSqliteQuery::GetFieldFloat(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldFloat(rowIndex, colIndex);
}


double DBSqliteQuery::GetFieldDouble(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldDouble(rowIndex, colIndex);
}


wxString DBSqliteUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields)
{
  wxString insertFieldsStr;

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      insertFieldsStr += wxT(",") + fields.at(fieldIndex);
    else
      insertFieldsStr = fields.at(fieldIndex);
  }

  wxString sql = wxT("INSERT INTO ") + tableName + wxT(" (") + insertFieldsStr + wxT(") VALUES (");

  for (unsigned int valueIndex = 0; valueIndex < fields.size(); valueIndex++) {
    if (valueIndex > 0)
      sql += ",";

    sql += wxString::Format("$%d", valueIndex + 1);
  }

  sql += wxT(")");

  return sql;
}


wxString DBSqliteUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields,
                                 std::vector<wxString> valueModifiers)
{
  wxString insertFieldsStr = "";

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      insertFieldsStr += ",";

    insertFieldsStr += fields.at(fieldIndex);
  }

  wxString sql = "INSERT INTO " + tableName + " (" + insertFieldsStr + ") VALUES (";

  for (unsigned int valueIndex = 0; valueIndex < fields.size(); valueIndex++) {
    wxString value = wxString::Format("?");

    if (valueIndex > 0)
      sql << ",";

    if (valueModifiers.size() > valueIndex && valueModifiers[valueIndex].Length() > 0) {
      wxString valueModifier = valueModifiers[valueIndex];

      wxRegEx regex("\\$@", wxRE_EXTENDED|wxRE_ADVANCED);

      regex.Replace(&valueModifier, value);

      sql << valueModifier;
    }
    else {
      sql << value;
    }
  }

  sql << ")";

  //wxMessageBox(sql);

  return sql;
}


wxString DBSqliteUtils::MountSQLUpdate(wxString tableName,
                                 std::vector<wxString> fields,
                                 wxString whereClause)
{
  wxString updateFieldsStr;

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      updateFieldsStr += wxString::Format(wxT(",%s=$%d"), fields.at(fieldIndex).c_str(), fieldIndex + 1);
    else
      updateFieldsStr = wxString::Format(wxT("%s=$%d"), fields.at(fieldIndex).c_str(), fieldIndex + 1);
  }

  wxString sql = wxT("UPDATE ") + tableName + wxT(" SET ") + updateFieldsStr;

  if (whereClause.Length() > 0)
    sql += wxT(" WHERE ") + whereClause;

  return sql;
}


wxString DBSqliteUtils::MountSQLUpdate(wxString tableName,
                                 std::vector<wxString> fields,
                                 wxString whereClause,
                                 std::vector<wxString> valueModifiers)
{
  wxString updateFieldsStr = "";

  // Fields

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0) {
      updateFieldsStr += ",";
    }

    wxString value = wxString::Format("$%d", fieldIndex + 1);;

    if (valueModifiers.size() > fieldIndex && valueModifiers[fieldIndex].Length() > 0) {
      wxString valueModifier = valueModifiers[fieldIndex];

      wxRegEx regex("\\$@", wxRE_EXTENDED|wxRE_ADVANCED);

      regex.Replace(&valueModifier, value);

      value = valueModifier;
    }

    value = wxString::Format("%s=", fields.at(fieldIndex).c_str()) + value;

    updateFieldsStr += value;
  }

  wxString sql = wxT("UPDATE ") + tableName + wxT(" SET ") + updateFieldsStr;

  if (whereClause.Length() > 0)
    sql += wxT(" WHERE ") + whereClause;

  return sql;
}


long DBSqliteUtils::GetSerialKey(wxString tableName, wxString columnName)
{
  wxString sql = wxString::Format(wxT("SELECT MAX(%s) AS id FROM %s"),
                                  columnName.c_str(),
                                  tableName.c_str());

  DBSqliteQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    return -1;
  }
  else {
    return dbquery.GetFieldLongByName(0, wxT("id"));
  }

  return 0;
}


int DBSqliteTransaction::Begin()
{
  // DBSqliteQuery dbquery;

  // int rc = dbquery.Execute(wxT("BEGIN"));

  // if (rc != 0) {
  //   LogMessenger *messenger = LogMessenger::etInstance();

  //   messenger->Register(wxT("NÃ£o foi possÃ­vel inicar a transaÃ§Ã£o com o banco de dados.\n\n")
  //                       wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  // }

  // return rc;

  return 0;
}


int DBSqliteTransaction::Commit()
{
  // DBSqliteQuery dbquery;

  // int rc = dbquery.Execute(wxT("COMMIT"));

  // if (rc != 0) {
  //   LogMessenger *messenger = LogMessenger::GetInstance();

  //   messenger->Register(wxT("Ocorreu erro na tentativa de confirmar a transaÃ§Ã£o com o banco de dados.\n\n")
  //                       wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  // }

  // return rc;

  return 0;
}


int DBSqliteTransaction::Rollback()
{
  // DBSqliteQuery dbquery;

  // int rc = dbquery.Execute(wxT("COMMIT"));

  // if (rc != 0) {
  //   LogMessenger *messenger = LogMessenger::GetInstance();

  //   messenger->Register(wxT("Ocorreu erro na tentativa de cancelar a transaÃ§Ã£o com o banco de dados.\n\n")
  //                       wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  // }

  // return rc;

  return 0;
}