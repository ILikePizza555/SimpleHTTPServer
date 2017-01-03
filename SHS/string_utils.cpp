#include "string_utils.h"

std::vector<std::string> utils::split(std::string string, std::string delim, int count) {
	std::vector<std::string> rv;

	int n = string.find(delim);

	while (n != std::string::npos && count != 0) {
		//Add the first part of the string to the vector
		rv.push_back(string.substr(0, n));
		//Erase the cut part (we include the delimnator this time
		string.erase(0, n + delim.length());

		//Set the variables
		n = string.find(delim);
		count--;
	}

	//Add the rest of the string to the vector, if not empty
	if (!string.empty()) rv.push_back(string);

	return rv;
}

std::string utils::concat(std::initializer_list<std::string> args, std::string delim, int beginOffset, int endOffset) {
	std::ostringstream os;
	std::copy(args.begin() + beginOffset, args.end() - endOffset, std::ostream_iterator<std::string>(os, delim.c_str()));
	os << *(args.end());

	return os.str();
}

std::string utils::concat(std::vector<std::string> args, std::string delim, int beginOffset, int endOffset) {
	std::ostringstream os;
	std::copy(args.begin() + beginOffset, args.end() - endOffset, std::ostream_iterator<std::string>(os, delim.c_str()));
	os << args.back();

	return os.str();
}

void utils::ltrim(std::string& string) {
	string.erase(string.begin(), std::find_if(string.begin(), string.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void utils::rtrim(std::string& string) {
	string.erase(std::find_if(string.rbegin(), string.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), string.end());
}

void utils::trim(std::string& string) {
	ltrim(string);
	rtrim(string);
}