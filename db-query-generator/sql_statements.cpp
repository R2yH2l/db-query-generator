#include "sql_statements.h"
#include <iostream>

// --------------------
// START OF SELECT FUNCTIONS
// --------------------

// Sets the table name for the select statement
void select_statement::set_table(const std::wstring& table) {
    this->table = table; // 'this->table' refers to the class's table member
}

// Adds the list of columns to the select statement
void select_statement::add_columns(const std::vector<std::wstring>& new_columns) {
    for (const auto& column : new_columns) {
        columns.emplace_back(column); // Adds each column to the columns vector
    }
}

// Generates the SQL select statement
std::wstring select_statement::generate_sql() const {
    std::wostringstream ss{};
    ss << L"SELECT ";

    // Loop through each column
    for (size_t i{}; i < columns.size(); i++) {
        ss << columns[i]; // Add the column name to the string stream
        if (i != columns.size() - 1) ss << L", "; // Add a comma separator unless it's the last column
    }

    ss << L" FROM " << table << L";"; // Add the table name
    return ss.str(); // Convert the string stream to a string
}

// --------------------
// END OF SELECT FUNCTIONS
// --------------------
// --------------------
// START OF SELECT ALL FUNCTIONS
// --------------------

// Sets the table name for the select all statement
void select_all_statement::set_table(const std::wstring& table) {
    this->table = table;
}

// Generates the SQL select all statement
std::wstring select_all_statement::generate_sql() const {
    std::wostringstream ss{};
    ss << L"SELECT * FROM " << table << L";"; // Add the table name
    return ss.str(); // Convert the string stream to a string
}

// --------------------
// END OF SELECT ALL FUNCTIONS
// --------------------
// --------------------
// START OF FILTER FUNCTIONS
// --------------------

void filter_statement::set_table(const std::wstring& table) {
    this->table = table;
}

void filter_statement::set_column(const std::wstring& column) {
    this->column = column;
}

void filter_statement::set_operation(const std::wstring& op) {
    this->op = op;
}

void filter_statement::set_value(const std::wstring& value) {
    this->value = value;
}

std::wstring filter_statement::generate_sql() const {
    std::wostringstream ss{};
    ss << L"SELECT * FROM " << table << L" WHERE " << column << L' ' << op << L' ' << value << L";"; // Add the table name
    return ss.str(); // Convert the string stream to a string
};

// --------------------
// END OF FILTER FUNCTIONS
// --------------------