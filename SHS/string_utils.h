#pragma once

#include <algorithm>
#include <cctype>
#include <functional> 
#include <initializer_list>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

namespace utils {
	namespace string {
		std::vector<std::string> split(std::string string, std::string delim, int count = -1);

		std::string concat(std::initializer_list<std::string> args, std::string delim = "", int beginOffset = 0, int endOffset = 1);
		std::string concat(std::vector<std::string> args, std::string delim = "", int beginOffset = 0, int endOffset = 1);

		void ltrim(std::string& string);
		void rtrim(std::string& string);
		void trim(std::string& string);

		bool beginsWith(std::string str, std::string begin);

		void replace(std::string& src, std::string sub, std::string replace, int count = -1);
	}
}