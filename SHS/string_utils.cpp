#include "string_utils.h"

std::vector<std::string> utils::split(std::string string, std::string delim, int count = -1) {
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
