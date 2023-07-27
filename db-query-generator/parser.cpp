#include "parser.h"

parser::parser(std::string_view file_path) {
	std::wifstream ifile(file_path.data());

	if (!ifile.is_open()) {
		return;
	}

	states state{};
	size_t scope{ 0 }, cur_table_idx{};
	std::wstring line{}, buffer{};
	while (std::getline(ifile, line)) {
		bool lhs{};

		for (const auto& ltr : line) {
			switch (state) {
			case states::start:
				if (isspace(ltr)) {
					state = states::start;
					continue;
				}
				if (ltr == L'{') {
					scope++;
					state = states::value;
					continue;
				}
				if (ltr == L'"') {
					if (lhs) {
						state = states::key;
						continue;
					}
					else {
						state = states::value;
						continue;
					}
				}
				if (ltr == L':') {
					lhs = false;
					continue;
				}
				state = states::unknown;
				break;
			case states::key:
				if (isalpha(ltr) || (ltr == L'.') || (ltr == L'_')) {
					buffer.push_back(ltr);
					state = states::key;
					continue;
				}
				if (ltr == L'"') {
					switch (scope) {
					case 0: {
						size_t split{ buffer.find_first_of(L'.') };
						std::wstring table_schema{ buffer.substr(0, split) };
						std::wstring table_name{ buffer.substr(split + 1, buffer.length() - split) };
						tables.emplace_back(std::make_shared<table_info>(table_info{ table_name, table_schema }));
						buffer.clear();
						state = states::start;
					}
					default:
						state = states::unknown;
						break;
					}
					state = states::start;
					continue;
				}
				state = states::unknown;
				break;
			}
		}
	}
}