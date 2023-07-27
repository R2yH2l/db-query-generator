/* db-query-generator
* Author: R2yH2l
* Purpose: Generate query data for use in machine learning
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SQLAPI.h>

struct query_info {
    std::wstring table{};
    int column_count{};
    std::wstring query{};

    friend bool operator<(const query_info& lhs, const query_info& rhs) {
        return lhs.column_count < rhs.column_count;
    }
};

int main() {
    SAConnection conn{};
    std::vector<std::wstring> table_names{}, column_names{};
    std::map<std::wstring, std::vector<query_info>> queries{}; // Use multimap to sort queries by column_count

    try {
        conn.Connect(L"localhost,1433@AdventureWorks2022;TrustServerCertificate=yes", L"", L"", SA_SQLServer_Client);
        std::cout << "Connected\n";

        SACommand cmd{ &conn,
            L"SELECT TABLE_NAME "
            L"FROM INFORMATION_SCHEMA.TABLES "
            L"WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_CATALOG='AdventureWorks2022';" };

        cmd.Execute();
        while (cmd.FetchNext()) {
            table_names.emplace_back(cmd.Field(L"TABLE_NAME").asString().GetWideChars());
        }

        for (const auto& table : table_names) {
            SACommand cmd2{ &conn,
                L"SELECT COLUMN_NAME "
                L"FROM INFORMATION_SCHEMA.COLUMNS "
                L"WHERE TABLE_NAME = :table AND TABLE_CATALOG='AdventureWorks2022';" };

            cmd2.Param(L"table").setAsString() = table.c_str();
            cmd2.Execute();

            column_names.clear(); // Clear the vector for the new set of columns
            while (cmd2.FetchNext()) {
                column_names.emplace_back(cmd2.Field(L"COLUMN_NAME").asString().GetWideChars());
            }

            // Now, generate the queries
            std::wstring previous_columns{};
            for (const auto& column : column_names) {
                std::wstring query{ L"SELECT " + column + L" FROM " + table + L";" };
                queries[table].emplace_back(query_info{ table, 1, query }); // single column query

                if (!previous_columns.empty()) {
                    std::wstring query{ L"SELECT " + previous_columns + column + L" FROM " + table + L";" };
                    queries[table].emplace_back(query_info{ table, static_cast<int>(previous_columns.size()) + 1, query }); // combined column query
                }

                previous_columns.append(column + L",");
            }
        }

        conn.Disconnect();
        std::cout << "disconnected\n\n-- Generated Queries --";

        // Print the generated queries
        std::wstring previous_table{};
        for (const auto& kv : queries) {
            // Sort to make output look better
            std::sort(queries[kv.first].begin(), queries[kv.first].end());

            std::wcout << "\n-- Table: " << kv.first << " --\n";
            for (const auto& qi : queries[kv.first]) {
                std::wcout << qi.query << '\n';
            }
        }
    }
    catch (SAException& err) {
        std::cout << err.ErrText().GetMultiByteChars() << "\n";
    }

    return 0;
}