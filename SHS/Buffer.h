#pragma once

#include <memory>
#include <string>

class Buffer
{
private:
	std::unique_ptr<char[]> buffer;
	size_t length;

public:

	Buffer(std::unique_ptr<char[]> ptr, size_t length);
	Buffer(char* ptr, size_t length);
	Buffer(size_t length);
	Buffer(Buffer&& b);


	char get(int pos);
	void set(int pos, char item);

	//Copies the given string into the buffer
	void assign(const char* string, size_t length);
	void assign(std::string string);

	//Clear the buffer by writing zero to each value
	void clear();

	char* data();
	size_t getLength();

	char& operator[](size_t index);
	Buffer& operator=(Buffer&& b);
};
