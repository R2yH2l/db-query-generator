#ifndef _SQL_STATEMENT_FACTORY_H
#define _SQL_STATEMENT_FACTORY_H

#include "sql_statements.h"

/* Type Definitions
************************************************************************/
struct row_info;
struct column_info;

/**
 * @struct field_info
 * @brief A struct that contains information about a database field.
 *
 * @var field_info::value
 * Member 'value' is a wstring that represents the value of a field in the database.
 *
 * @var field_info::row
 * Member 'row' is a shared_ptr to a row_info struct, representing information about the row that this field belongs to.
 *
 * @var field_info::column
 * Member 'column' is a shared_ptr to a column_info struct, representing information about the column that this field belongs to.
 */
struct field_info {
    /**
    * @brief Construct a new field info object.
    *
    * @param value The value of the field in the database.
    * @param row Shared pointer to the row_info struct this field belongs to.
    * @param column Shared pointer to the column_info struct this field belongs to.
    */
    field_info(std::wstring value, std::shared_ptr<row_info> row, std::shared_ptr<column_info> column) :
        value(value), row(row), column(column) {}

    std::wstring value{};                     ///< Represents the value of a field in the database.
    std::shared_ptr<row_info> row{};          ///< Shared pointer to the row_info struct this field belongs to.
    std::shared_ptr<column_info> column{};    ///< Shared pointer to the column_info struct this field belongs to.
};

struct row_info {
    std::vector<std::shared_ptr<field_info>> fields{};
};

struct column_info {

    column_info(std::wstring name, std::wstring data_type) :
        name(name), data_type(data_type) {}

    std::wstring name{}, data_type{};
};

struct table_info {

    table_info(std::wstring name, std::wstring schema) :
        name(name), schema(schema) {}

    std::wstring name{}, schema{};
    std::vector<std::shared_ptr<column_info>> columns{};
    std::vector<std::shared_ptr<row_info>> rows{};
};

// Defines a class responsible for creating and managing SQL statements
class sql_statement_factory {
    // Holds a list of created SQL statements
    std::vector<std::shared_ptr<sql_statement>> statements{};

public:

    /**
     * @brief Creates a SELECT SQL statement for specified columns from a table.
     *
     * @param table : The name of the table to select from.
     * @param columns : The names of the columns to select.
     * @return A shared pointer to the created SQL statement.
     */
    std::shared_ptr<sql_statement> create_select_statement(const std::wstring& table, const std::vector<std::wstring>& columns);

    /**
     * @brief Creates a SELECT * (all) SQL statement from a table.
     *
     * @param table : The name of the table to select from.
     * @return A shared pointer to the created SQL statement.
     */
    std::shared_ptr<sql_statement> create_select_all_statement(const std::wstring& table);

    std::shared_ptr<sql_statement> create_filter_statement(const std::wstring& table, const std::wstring& column, const std::wstring& operation, const std::wstring& value);

    /**
     * @brief Generates and prints all created SQL statements.
     * 
     * @return Vector of wstring each of which is a SQL statement
     */
    std::vector<std::wstring> generate_all_sql();
};

#endif // !_SQL_STATEMENT_FACTORY_H