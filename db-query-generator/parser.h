#ifndef _PARSER_H
#define _PARSER_H

#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <variant>

#include "sql_statement_factory.h"

enum struct states {
	start,
	key,
	value,
	unknown
};

using value = std::variant<std::string, std::vector<std::string>>;

class parser {
	std::vector<std::shared_ptr<table_info>> tables{};

public:

	parser(std::string_view file_path);
};

#endif // !_PARSER_H