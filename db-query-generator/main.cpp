/***********************************************************************
 *  Project: db-query-generator
 *  File: main.cpp
 *  Date: 2023-07-24
 *  Author: R2yH2l
 ***********************************************************************/

/* Includes
************************************************************************/
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SQLAPI.h>

#include "sql_statement_factory.h"

/* Data Structs
************************************************************************/
enum struct data_types {
    unknown,
    ext_bit,
    ext_tinyint,
    ext_smallint,
    ext_int,
    ext_bigint,
    ext_decimal,
    ext_numeric,
    ext_smallmoney,
    ext_money,
    aprx_float,
    aprx_real,
    dat_date,
    dat_datetime,
    dat_datetime2,
    dat_datetimeoffset,
    dat_smalldatetime,
    dat_time,
    str_char,
    str_varchar,
    str_text,
    str_nchar,
    str_nvarchar,
    str_ntext,
    uni_str_nchar,
    uni_str_nvarchar,
    uni_str_ntext
};

enum struct data_type_groups {
    unknown,
    exact_numeric,
    approximate_numeric,
    date_and_time,
    character_string,
    unicode_character_string
};

/* Function Definitions
************************************************************************/
data_types type_from_string(const std::wstring_view& type) {
    if (type == L"bit") {
        return data_types::ext_bit;
    }
    else if (type == L"tinyint") {
        return data_types::ext_tinyint;
    }
    else if (type == L"smallint") {
        return data_types::ext_smallint;
    }
    else if (type == L"int") {
        return data_types::ext_int;
    }
    else if (type == L"bigint") {
        return data_types::ext_bigint;
    }
    else if (type == L"decimal") {
        return data_types::ext_decimal;
    }
    else if (type == L"numeric") {
        return data_types::ext_numeric;
    }
    else if (type == L"smallmoney") {
        return data_types::ext_smallmoney;
    }
    else if (type == L"money") {
        return data_types::ext_money;
    }
    else if (type == L"float") {
        return data_types::aprx_float;
    }
    else if (type == L"real") {
        return data_types::aprx_real;
    }
    else if (type == L"date") {
        return data_types::dat_date;
    }
    else if (type == L"datetime") {
        return data_types::dat_datetime;
    }
    else if (type == L"datetime2") {
        return data_types::dat_datetime2;
    }
    else if (type == L"datetimeoffset") {
        return data_types::dat_datetimeoffset;
    }
    else if (type == L"smalldatetime") {
        return data_types::dat_smalldatetime;
    }
    else if (type == L"time") {
        return data_types::dat_time;
    }
    else if (type == L"char") {
        return data_types::str_char;
    }
    else if (type == L"varchar") {
        return data_types::str_varchar;
    }
    else if (type == L"text") {
        return data_types::str_text;
    }
    else if (type == L"nchar") {
        return data_types::str_nchar;
    }
    else if (type == L"nvarchar") {
        return data_types::str_nvarchar;
    }
    else if (type == L"ntext") {
        return data_types::str_ntext;
    }
    else if (type == L"nchar") {
        return data_types::uni_str_nchar;
    }
    else if (type == L"nvarchar") {
        return data_types::uni_str_nvarchar;
    }
    else if (type == L"ntext") {
        return data_types::uni_str_ntext;
    }
    else {
        return data_types::unknown;
    }
}

data_type_groups type_group_from_string(const std::wstring_view& type) {
    if (type == L"bit") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"tinyint") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"smallint") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"int") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"bigint") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"decimal") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"numeric") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"smallmoney") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"money") {
        return data_type_groups::exact_numeric;
    }
    else if (type == L"float") {
        return data_type_groups::approximate_numeric;
    }
    else if (type == L"real") {
        return data_type_groups::approximate_numeric;
    }
    else if (type == L"date") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"datetime") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"datetime2") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"datetimeoffset") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"smalldatetime") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"time") {
        return data_type_groups::date_and_time;
    }
    else if (type == L"char") {
        return data_type_groups::character_string;
    }
    else if (type == L"varchar") {
        return data_type_groups::character_string;
    }
    else if (type == L"text") {
        return data_type_groups::character_string;
    }
    else if (type == L"nchar") {
        return data_type_groups::character_string;
    }
    else if (type == L"nvarchar") {
        return data_type_groups::character_string;
    }
    else if (type == L"ntext") {
        return data_type_groups::character_string;
    }
    else if (type == L"nchar") {
        return data_type_groups::unicode_character_string;
    }
    else if (type == L"nvarchar") {
        return data_type_groups::unicode_character_string;
    }
    else if (type == L"ntext") {
        return data_type_groups::unicode_character_string;
    }
    else {
        return data_type_groups::unknown;
    }
}

/* Main Function
************************************************************************/
int main() {
    SAConnection conn{};
    std::vector<std::shared_ptr<table_info>> tables{};

    try {
        conn.Connect(L"localhost,1433@AdventureWorks2022;TrustServerCertificate=yes", L"", L"", SA_SQLServer_Client);
        std::wcout << L"Connected\n";

        SACommand cmd{ &conn,
            L"SELECT TABLE_NAME, TABLE_SCHEMA "
            L"FROM INFORMATION_SCHEMA.TABLES "
            L"WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_CATALOG='AdventureWorks2022';" };

        cmd.Execute();
        while (cmd.FetchNext()) {
            std::wstring table_name{ cmd.Field(L"TABLE_NAME").asString().GetWideChars() };
            std::wstring table_schema{ cmd.Field(L"TABLE_SCHEMA").asString().GetWideChars() };

            tables.emplace_back(std::make_shared<table_info>(table_info{ table_name, table_schema }));
        }

        for (const auto& table : tables) {
            SACommand cmd2{ &conn,
                L"SELECT COLUMN_NAME, DATA_TYPE "
                L"FROM INFORMATION_SCHEMA.COLUMNS "
                L"WHERE TABLE_NAME = :table AND TABLE_CATALOG='AdventureWorks2022';" };

            cmd2.Param(L"table").setAsString() = table->name.c_str();
            cmd2.Execute();

            while (cmd2.FetchNext()) {
                std::wstring column_name{ cmd2.Field(L"COLUMN_NAME").asString().GetWideChars() };
                std::wstring data_type{ cmd2.Field(L"DATA_TYPE").asString().GetWideChars() };

                table->columns.emplace_back(std::make_shared<column_info>(column_info{ column_name, data_type }));
            }

            SACommand cmd3{ &conn, std::wstring(L"SELECT * FROM " + table->schema + L"." + table->name).c_str()};
            cmd3.Execute();
            while (cmd3.FetchNext()) {
                std::shared_ptr<row_info> row{ std::make_shared<row_info>(row_info{}) };

                for (const auto& column : table->columns) {
                    std::wstring value{ cmd3.Field(column->name.c_str()).asString().GetWideChars() };
             
                    row->fields.emplace_back(std::make_shared<field_info>(field_info{ value, row, column }));
                }

                table->rows.emplace_back(row);
            }
        }

        conn.Disconnect();
        std::wcout << L"disconnected\n\n";

        // factory.generate_all_sql();
    }
    catch (SAException& err) {
        std::wcout << err.ErrText().GetMultiByteChars() << L"\n";
    }

    /* Generate SQL
    ************************************************************************/
    sql_statement_factory factory{};

    for (const auto& table : tables) {
        factory.create_select_all_statement(table->name);

        std::vector<std::wstring> prev_columns{};

        for (const auto& column : table->columns) {
            factory.create_select_statement(table->name, { column->name });

            if (column == *(table->columns.end() - 1)) {
                break;
            }

            prev_columns.emplace_back(column->name);

            if (prev_columns.size() > 1) {
                factory.create_select_statement(table->name, prev_columns);
            }
        }

        int limit{ 100 };
        std::vector<int> indices(table->rows.size()); // Create an array of indices

        // Initialize the indices
        std::iota(indices.begin(), indices.end(), 0);

        // Shuffle the indices
        std::random_device rd{};
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        // Use the shuffled indices to iterate over the rows
        for (int i = 0; i < limit && i < indices.size(); i++) {
            const auto& row = table->rows[indices[i]];

            for (const auto& field : row->fields) {
                switch (type_group_from_string(field->column->data_type)) {
                case data_type_groups::unknown:
                    break;
                case data_type_groups::exact_numeric:
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
                    break;
                case data_type_groups::approximate_numeric:
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
                    break;
                case data_type_groups::date_and_time:
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
                    break;
                case data_type_groups::character_string:
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
                    break;
                case data_type_groups::unicode_character_string:
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
                    factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
                    break;
                }
            }
        }
    }
    /***********************************************************************/

    std::wofstream out_file("statements.txt");

    if (!out_file.is_open()) {
        return 1;
    }

    const auto& sql{ factory.generate_all_sql() };

    for (const auto& stmt : sql) {
        out_file << stmt << '\n';
    }

    out_file.close();

    // "<string>"
    // is a key on the left hand side (lhs)
    // and a value when on the right hand side (rhs)
    //
    // {<data>}
    // can only be on the rhs
    // and contains a vector of data
    //
    // "<key>" : <value>
    // is a key-value pair
    // lhs is a key to data of rhs

    //std::wofstream out_file("advnwks2022.schema");

    //for (const auto& table : tables) {
    //    out_file << L'"' << table->schema << L'.' << table->name << L"\" : {\n";
    //    out_file << L"\t\"columns\" : {\n";

    //    for (const auto& column : table->columns) {
    //        out_file << L"\t\t\"" << column->name << L"\" : \"" << column->data_type << L"\"\n";
    //    }

    //    out_file << L"\t}\n";
    //    out_file << L"\t\"rows\" : {\n";

    //    for (const auto& row : table->rows) {
    //        out_file << L"\t\t\"row\" : {\n";

    //        for (const auto& field : row->fields) {
    //            out_file << L"\t\t\t\"" << field->column->name << L"\" : \"" << field->value << L"\"\n";
    //        }

    //        out_file << L"\t\t}\n";
    //    }

    //    out_file << L"\t}\n";
    //    out_file << L"}\n";
    //}

    return 0;
}
/***********************************************************************/