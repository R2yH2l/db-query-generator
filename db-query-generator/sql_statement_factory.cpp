#include "sql_statement_factory.h"
#include <iostream>

// Creates a select statement and adds it to the list of statements
std::shared_ptr<sql_statement> sql_statement_factory::create_select_statement(const std::wstring& table, const std::vector<std::wstring>& columns) {
    auto stmt{ std::make_shared<select_statement>() }; // Create a new select_statement
    stmt->set_table(table); // Set the table name
    stmt->add_columns(columns); // Add the columns
    statements.push_back(stmt); // Add the statement to the list
    return stmt; // Return the created statement
}

// Creates a select all statement and adds it to the list of statements
std::shared_ptr<sql_statement> sql_statement_factory::create_select_all_statement(const std::wstring& table) {
    auto stmt{ std::make_shared<select_all_statement>() }; // Create a new select_all_statement
    stmt->set_table(table); // Set the table name
    statements.push_back(stmt); // Add the statement to the list
    return stmt; // Return the created statement
}

std::shared_ptr<sql_statement> sql_statement_factory::create_filter_statement(const std::wstring& table, const std::wstring& column, const std::wstring& operation, const std::wstring& value) {
    auto stmt{ std::make_shared<filter_statement>() }; // Create a new filter_statement
    stmt->set_table(table); // Set the table name
    stmt->set_column(column);
    stmt->set_operation(operation);
    stmt->set_value(value);
    statements.push_back(stmt); // Add the statement to the list
    return stmt; // Return the created statement
}

// Generates all SQL statements and prints them
std::vector<std::wstring>  sql_statement_factory::generate_all_sql() {
    std::vector<std::wstring> sql{};

    for (const auto& stmt : statements) {
        sql.emplace_back(stmt->generate_sql()); // Generate the SQL and print it
    }

    return sql;
}