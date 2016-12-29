#include "Buffer.h"

Buffer::Buffer(std::unique_ptr<char[]> ptr, size_t length) : buffer(std::move(ptr)), length(length) {}
Buffer::Buffer(char * ptr, size_t length) : buffer(ptr), length(length) {}
Buffer::Buffer(size_t length) : buffer(new char[length]), length(length) {}
Buffer::Buffer(Buffer&& b) : buffer(std::move(b.buffer)), length(b.length) {}

char Buffer::get(int pos) {
	return buffer[pos];
}

void Buffer::set(int pos, char item) {
	buffer[pos] = item;
}

void Buffer::assign(const char* string, size_t size) {
	for(int i = 0; i < size; i++) buffer[i] = string[i];
}

void Buffer::assign(std::string string) {
	assign(string.c_str(), string.length());
}

void Buffer::clear() {
	std::fill(buffer.get(), buffer.get() + length, 0);
}

char* Buffer::data() {
	return buffer.get();
}

size_t Buffer::getLength() {
	return length;
}

char& Buffer::operator[](size_t index) {
	return buffer[index];
}

Buffer& Buffer::operator=(Buffer&& b) {
	if (this != &b) {
		buffer = std::move(b.buffer);
		length = b.length;
	}
	return *this;
}