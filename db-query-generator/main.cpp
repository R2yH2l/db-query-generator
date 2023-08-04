/***********************************************************************
 *  Project: db-query-generator
 *  File: main.cpp
 *  Date: 2023-07-24
 *  Author: R2yH2l
 ***********************************************************************/

 /* Includes
 ************************************************************************/
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
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

/* Functions
************************************************************************/

// Custom deleter for XMLCh*
struct XMLChDeleter {
    void operator()(XMLCh* p) const noexcept { xercesc::XMLString::release(&p); }
};

using XMLChPtr = std::unique_ptr<XMLCh, XMLChDeleter>;

// Function to transcode std::string to XMLCh*
XMLChPtr transcode(const std::string& str) {
    return XMLChPtr{ xercesc::XMLString::transcode(str.c_str()) };
}

/* Main Function
************************************************************************/
int main() {
    SAConnection conn{};
    std::vector<std::shared_ptr<table_info>> tables{};
    // unique schema names
    std::set<std::wstring> schema_names{};

    try {
        conn.Connect(L"localhost,1433@AdventureWorks2022;TrustServerCertificate=yes", L"", L"", SA_SQLServer_Client);
        std::wcout << L"[+] Connected to database.\n[-] Parsing database...\n\n";

        SACommand cmd{ &conn,
            L"SELECT TABLE_NAME, TABLE_SCHEMA "
            L"FROM INFORMATION_SCHEMA.TABLES "
            L"WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_CATALOG='AdventureWorks2022';" };

        cmd.Execute();
        while (cmd.FetchNext()) {
            std::wstring table_name{ cmd.Field(L"TABLE_NAME").asString().GetWideChars() };
            std::wstring table_schema{ cmd.Field(L"TABLE_SCHEMA").asString().GetWideChars() };
            schema_names.insert(table_schema);

            tables.emplace_back(std::make_shared<table_info>(table_info{ table_name, table_schema }));
        }

        std::wcout << L"[+] Found " << tables.size() << L" tables.\n[-] Parsing tables...\n\n";

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

            SACommand cmd3{ &conn, std::wstring(L"SELECT * FROM " + table->schema + L"." + table->name).c_str() };
            cmd3.Execute();
            while (cmd3.FetchNext()) {
                std::shared_ptr<row_info> row{ std::make_shared<row_info>(row_info{}) };

                for (const auto& column : table->columns) {
                    std::wstring value{ cmd3.Field(column->name.c_str()).asString().GetWideChars() };

                    row->fields.emplace_back(std::make_shared<field_info>(field_info{ value, row, column }));
                }

                table->rows.emplace_back(row);
            }

            std::wcout << L"[+] Table: " << table->schema + L'.' + table->name << L" completed.\n";
            std::wcout << L"    Column Count: " << table->columns.size() << L'\n';
            std::wcout << L"    Row Count: " << table->rows.size() << L"\n\n";
        }

        std::wcout << L"[+] Finished parsing the database.\n";

        conn.Disconnect();
        std::wcout << L"[+] Disconnected from database\n" << std::endl;

        // factory.generate_all_sql();
    }
    catch (SAException& err) {
        std::wcout << err.ErrText().GetMultiByteChars() << L"\n";
    }

    /* Generate SQL
    ********************************************************************/
    sql_statement_factory factory{};

    std::wcout << L"[-] Generating SQL statments...\n";

    for (const auto& table : tables) {
        factory.create_select_all_statement(table->schema + L'.' + table->name);

        std::vector<std::wstring> prev_columns{};

        for (const auto& column : table->columns) {
            factory.create_select_statement(table->schema + L'.' + table->name, { column->name });

            if (column == *(table->columns.end() - 1)) {
                break;
            }

            prev_columns.emplace_back(column->name);

            if (prev_columns.size() > 1) {
                factory.create_select_statement(table->schema + L'.' + table->name, prev_columns);
            }
        }

        //    int limit{ 100 };
        //    std::vector<int> indices(table->rows.size()); // Create an array of indices

        //    // Initialize the indices
        //    std::iota(indices.begin(), indices.end(), 0);

        //    // Shuffle the indices
        //    std::random_device rd{};
        //    std::mt19937 g(rd());
        //    std::shuffle(indices.begin(), indices.end(), g);

        //    // Use the shuffled indices to iterate over the rows
        //    for (int i = 0; i < limit && i < indices.size(); i++) {
        //        const auto& row = table->rows[indices[i]];

        //        for (const auto& field : row->fields) {
        //            switch (type_group_from_string(field->column->data_type)) {
        //            case data_type_groups::unknown:
        //                break;
        //            case data_type_groups::exact_numeric:
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
        //                break;
        //            case data_type_groups::approximate_numeric:
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
        //                break;
        //            case data_type_groups::date_and_time:
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L">=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"<=", field->value);
        //                break;
        //            case data_type_groups::character_string:
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
        //                break;
        //            case data_type_groups::unicode_character_string:
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"=", field->value);
        //                factory.create_filter_statement(table->schema + L'.' + table->name, field->column->name, L"!=", field->value);
        //                break;
        //            }
        //        }
        //    }
    }
    /***********************************************************************/

    std::wcout << L"[+] Finished generating SQL statments.\n\n";

    try {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& caught) {
        std::u16string s16{ caught.getMessage() };
        std::wstring message{ s16.begin(), s16.end() };
        std::wcout << L"[!] XML toolkit initialization error: " << message << std::endl;
        return 1;
    }

    try {
        std::wcout << L"[-] Writing SQL statments to file...\n";

        // Define constants for the tag names and attributes
        XMLChPtr ATTR_count{ transcode("count") };
        XMLChPtr ATTR_name{ transcode("name") };

        XMLChPtr TAG_root{ transcode("sql_info") };

        XMLChPtr TAG_schemas{ transcode("schemas") };
        XMLChPtr TAG_schema{ transcode("schema") };

        XMLChPtr TAG_tables{ transcode("tables") };
        XMLChPtr TAG_table { transcode("table") };

        XMLChPtr TAG_column{ transcode("column") };

        XMLChPtr TAG_statements{ transcode("statements") };
        XMLChPtr TAG_statement{ transcode("statement") };
        XMLChPtr TAG_query{ transcode("query") };
        XMLChPtr TAG_label{ transcode("label") };

        xercesc::DOMImplementation* impl{ xercesc::DOMImplementationRegistry::getDOMImplementation(transcode("LS").get()) };

        xercesc::DOMDocument* doc = impl->createDocument(
            0,              // root element namespace URI.
            TAG_root.get(), // root element name
            0);             // document type object (DTD).

        xercesc::DOMElement* root_elem{ doc->getDocumentElement() };

        xercesc::DOMElement* schema_group_elem{ doc->createElement(TAG_schemas.get()) };
        root_elem->appendChild(schema_group_elem);

        XMLChPtr schema_count{ transcode(std::to_string(schema_names.size())) };
        schema_group_elem->setAttribute(ATTR_count.get(), schema_count.get());

        for (const auto& schema : schema_names) {
            xercesc::DOMElement* schema_elem{ doc->createElement(TAG_schema.get()) };
            schema_group_elem->appendChild(schema_elem);

            std::u16string temp_schema{ schema.begin(), schema.end() };
            schema_elem->appendChild(doc->createTextNode(temp_schema.c_str()));
        }

        xercesc::DOMElement* table_group_elem{ doc->createElement(TAG_tables.get()) };
        root_elem->appendChild(table_group_elem);

        XMLChPtr table_count{ transcode(std::to_string(tables.size())) };
        table_group_elem->setAttribute(ATTR_count.get(), table_count.get());

        for (const auto& table : tables) {
            xercesc::DOMElement* table_elem{ doc->createElement(TAG_table.get()) };
            table_group_elem->appendChild(table_elem);

            std::u16string qualified_table{ table->schema.begin(), table->schema.end() };
            std::u16string table_name{ table->name.begin(), table->name.end() };
            qualified_table.append(u'.' + table_name);
            table_elem->appendChild(doc->createTextNode(qualified_table.c_str()));
        }

        const auto& queries{ factory.generate_all_sql() };
        const auto& labels{ factory.generate_all_labels() };

        assert(queries.size() == labels.size());

        xercesc::DOMElement* statement_group_elem{ doc->createElement(TAG_statements.get()) };
        root_elem->appendChild(statement_group_elem);

        XMLChPtr statement_count{ transcode(std::to_string(queries.size())) };
        statement_group_elem->setAttribute(ATTR_count.get(), statement_count.get());

        size_t idx{};

        for (const auto& query : queries) {
            const auto& label{ labels[idx] };

            xercesc::DOMElement* statement_elem{ doc->createElement(TAG_statement.get()) };
            statement_group_elem->appendChild(statement_elem);

            xercesc::DOMElement* query_elem{ doc->createElement(TAG_query.get()) };
            statement_elem->appendChild(query_elem);

            std::u16string temp_query{ query.begin(), query.end() };
            query_elem->appendChild(doc->createTextNode(temp_query.c_str()));

            xercesc::DOMElement* label_elem{ doc->createElement(TAG_label.get()) };
            statement_elem->appendChild(label_elem);

            std::u16string temp_label{ label.begin(), label.end() };
            label_elem->appendChild(doc->createTextNode(temp_label.c_str()));

            idx++;
        }

        xercesc::DOMLSSerializer* the_serializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();

        if (the_serializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
            the_serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);

        xercesc::XMLFormatTarget* my_form_target = new xercesc::LocalFileFormatTarget("statements.xml");
        xercesc::DOMLSOutput* the_output = ((xercesc::DOMImplementationLS*)impl)->createLSOutput();
        the_output->setByteStream(my_form_target);

        the_serializer->write(doc, the_output);

        the_output->release();
        the_serializer->release();
        delete my_form_target;

        std::wcout << L"[+] Finished writing SQL statments to file.\n\n";
    }
    catch (const xercesc::DOMException& caught) {
        std::u16string s16{ caught.getMessage() };
        std::wstring message{ s16.begin(), s16.end() };
        std::wcout << L"[!] DOMException: " << message << std::endl;
        return 1;
    }
    catch (const xercesc::XMLException& caught) {
        std::u16string s16{ caught.getMessage() };
        std::wstring message{ s16.begin(), s16.end() };
        std::wcout << L"[!] XMLException: " << message << std::endl;
        return 1;
    }

    try {
        std::wcout << L"[-] Converting database to XML...\n";

        // Define constants for the tag names and attributes
        XMLChPtr ATTR_name{ transcode("name") };
        XMLChPtr ATTR_type{ transcode("type") };

        XMLChPtr TAG_root{ transcode("database") };
        XMLChPtr ATTR_table_count{ transcode("number_of_tables") };

        XMLChPtr TAG_table{ transcode("table") };
        XMLChPtr ATTR_column_count{ transcode("number_of_columns") };
        XMLChPtr ATTR_row_count{ transcode("number_of_rows") };

        XMLChPtr TAG_column{ transcode("column") };

        XMLChPtr TAG_row{ transcode("row") };

        XMLChPtr TAG_field{ transcode("field") };
        XMLChPtr ATTR_value{ transcode("value") };
        XMLChPtr ATTR_parent_column{ transcode("parent_column") };

        xercesc::DOMImplementation* impl{ xercesc::DOMImplementationRegistry::getDOMImplementation(transcode("LS").get()) };

        xercesc::DOMDocument* doc = impl->createDocument(
            0,              // root element namespace URI.
            TAG_root.get(), // root element name
            0);             // document type object (DTD).

        xercesc::DOMElement* root_elem{ doc->getDocumentElement() };

        XMLChPtr table_count{ transcode(std::to_string(tables.size())) };
        root_elem->setAttribute(ATTR_table_count.get(), table_count.get());

        for (const auto& table : tables) {

            xercesc::DOMElement* table_elem{ doc->createElement(TAG_table.get()) };
            root_elem->appendChild(table_elem);

            std::u16string table_schema{ table->schema.begin(), table->schema.end() };
            std::u16string table_name{ table->name.begin(), table->name.end() };
            table_schema.append(u'.' + table_name);
            table_elem->setAttribute(ATTR_name.get(), table_schema.c_str());

            XMLChPtr column_count{ transcode(std::to_string(table->columns.size())) };
            table_elem->setAttribute(ATTR_column_count.get(), column_count.get());

            XMLChPtr row_count{ transcode(std::to_string(table->rows.size())) };
            table_elem->setAttribute(ATTR_row_count.get(), row_count.get());

            for (const auto& column : table->columns) {
                xercesc::DOMElement* column_elem{ doc->createElement(TAG_column.get()) };
                table_elem->appendChild(column_elem);

                std::u16string column_name{ column->name.begin(), column->name.end() };
                column_elem->setAttribute(ATTR_name.get(), column_name.c_str());

                std::u16string column_type{ column->data_type.begin(), column->data_type.end() };
                column_elem->setAttribute(ATTR_type.get(), column_type.c_str());
            }

            for (const auto& row : table->rows) {
                xercesc::DOMElement* row_elem{ doc->createElement(TAG_row.get()) };
                table_elem->appendChild(row_elem);

                for (const auto& field : row->fields) {
                    xercesc::DOMElement* field_elem{ doc->createElement(TAG_field.get()) };
                    row_elem->appendChild(field_elem);

                    std::u16string field_value{ field->value.begin(), field->value.end() };
                    field_elem->setAttribute(ATTR_value.get(), field_value.c_str());

                    std::u16string parent_column{ field->column->name.begin(), field->column->name.end() };
                    field_elem->setAttribute(ATTR_parent_column.get(), parent_column.c_str());
                }
            }
        }

        std::wcout << L"[+] Finished converting database to XML.\n\n[-] Writing XML to file...\n";

        xercesc::DOMLSSerializer* the_serializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();

        //if (the_serializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
        //    the_serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);

        xercesc::XMLFormatTarget* my_form_target = new xercesc::LocalFileFormatTarget("advnwks2022.xml");
        xercesc::DOMLSOutput* the_output = ((xercesc::DOMImplementationLS*)impl)->createLSOutput();
        the_output->setByteStream(my_form_target);

        the_serializer->write(doc, the_output);

        std::wcout << L"[+] Finished writing XML file.\n\n[-] Finishing up...\n";

        the_output->release();
        the_serializer->release();
        delete my_form_target;
    }
    catch (const xercesc::DOMException& caught) {
        std::u16string s16{ caught.getMessage() };
        std::wstring message{ s16.begin(), s16.end() };
        std::wcout << L"[!] DOMException: " << message << std::endl;
        return 1;
    }
    catch (const xercesc::XMLException& caught) {
        std::u16string s16{ caught.getMessage() };
        std::wstring message{ s16.begin(), s16.end() };
        std::wcout << L"[!] XMLException: " << message << std::endl;
        return 1;
    }

    xercesc::XMLPlatformUtils::Terminate();

    std::wcout << L"[+] Done. Have a great day!" << std::endl;

    return 0;
}
/***********************************************************************/