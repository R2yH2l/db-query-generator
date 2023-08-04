#ifndef _SQL_STATEMENTS_H
#define _SQL_STATEMENTS_H

#include <string>
#include <vector>
#include <sstream>
#include <memory>

// Base class for SQL statements
class sql_statement {
public:

	// Pure virtual function for generating SQL
	virtual std::wstring generate_sql() const = 0;
	virtual std::wstring generate_label() const = 0;

};

// Class for select SQL statements
class select_statement : public sql_statement {
	std::wstring table{}; // Name of the table to select from
	std::vector<std::wstring> columns{}; // Names of the columns to select

public:

	/**
	 * @brief Sets the name of the table for the 'SELECT' statement.
	 *
	 * @param table : The name of the table.
	 */
	void set_table(const std::wstring& table);

	/**
	 * @brief Adds column names for the 'SELECT' statement.
	 *
	 * @param new_columns : A vector of column names.
	 */
	void add_columns(const std::vector<std::wstring>& new_columns);

	/**
	 * @brief Generates a 'SELECT <column1>, ... FROM <table>' statement.
	 *
	 * @return The generated SQL statement as a string.
	 */
	std::wstring generate_sql() const override;

	/**
	 * @brief Generates a label for a 'SELECT' statement.
	 *
	 * @return The generated label as a string.
	 */
	std::wstring generate_label() const override;
};

// Class for select all (*) SQL statements
class select_all_statement : public sql_statement {
	std::wstring table{}; // Name of the table to select from

public:

	/**
	 * @brief Sets the name of the table for the 'SELECT ALL' statement.
	 *
	 * @param table : The name of the table.
	 */
	void set_table(const std::wstring& table);

	/**
	 * @brief Generates a 'SELECT * FROM <table>' SQL statement.
	 *
	 * @return The generated SQL statement as a string.
	 */
	std::wstring generate_sql() const override;

	/**
	 * @brief Generates a label for a 'SELECT ALL' statement.
	 *
	 * @return The generated label as a string.
	 */
	std::wstring generate_label() const override;
};

class filter_statement : public sql_statement {
	std::wstring table{};
	std::wstring column{};
	std::wstring op{};
	std::wstring value{};

public:

	/**
      * @brief Sets the name of the table for the 'FILTER' statement.
      *
      * @param table : The name of the table.
      */
	void set_table(const std::wstring& table);

	/**
      * @brief Sets the name of the column for the 'FILTER' statement.
      *
      * @param column : The name of the column.
      */
	void set_column(const std::wstring& column);

	/**
	 * @brief Sets the operation for the 'FILTER' statement.
	 *
	 * @param column : The operator e.g. (=, <, >, etc.).
	 */
	void set_operation(const std::wstring& op);

	/**
	 * @brief Sets the value for the 'FILTER' statement.
	 *
	 * @param column : The value to filter against.
	 */
	void set_value(const std::wstring& value);

	/**
	 * @brief Generates a 'SELECT * FROM <table> WHERE <column> <op> <value>;' SQL statement.
	 *
	 * @return The generated SQL statement as a string.
	 */
	std::wstring generate_sql() const override;

	/**
	 * @brief Generates a label for a 'FILTER' statement.
	 *
	 * @return The generated label as a string.
	 */
	std::wstring generate_label() const override;
};

#endif // !_SQL_STATEMENTS_H
