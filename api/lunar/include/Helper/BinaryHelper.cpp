#pragma once
#include "BinaryHelper.h"
#include <cmath>

using namespace std;
using namespace osomi;

inline char BinaryHelper::getChar(char c) {
	if (c < 10)return '0' + c;
	return 'A' + c - 10;
}
template<class I> string BinaryHelper::toHex(I i) {
	string s(sizeof(i) * 2, '0');
	I j = 0;
	while (i > 0) {
		s[s.size() - 1 - j] = getChar(i % 16);
		i /= 16;
		j++;
	}
	return s;
}
unsigned int BinaryHelper::asBinary(float f) {
	return *((unsigned int*)&f);
}
unsigned long long BinaryHelper::asBinary(double d) {
	return *((unsigned long long*)&d);
}
string BinaryHelper::asHex(unsigned int i) {
	return toHex<unsigned int>(i);
}
string BinaryHelper::asHex(unsigned long long l) {
	return toHex<unsigned long long>(l);
}
unsigned int BinaryHelper::readUInt(ifstream &input, unsigned int end, unsigned int length) {
	if (length > 4) {
		printf("Tried to write an unsigned int with a length of > 4\n");
		return 0;
	}
	if (length > end) {
		printf("Tried to get an element with end < length\n");
		return 0;
	}
	input.seekg(input.end);
	unsigned int l = (unsigned int)input.tellg();
	if (l < end + length) {
		printf("File out of bounds\n");
		return 0;
	}
	input.seekg(input.beg + end - length);
	char *buffer = new char[length];
	input.read(buffer, length);
	unsigned int i = 0;
	for (unsigned int k = 0; k < length; k++)
		i |= buffer[0] << (length - 1 - k);
	delete[] buffer;
	return i;
}
void BinaryHelper::writeUInt(ofstream &output, unsigned int length, unsigned int j) {
	if (length > 4) {
		printf("Tried to write an unsigned int with a length of > 4\n");
		return;
	}
	char *buf = new char[length];
	for (int i = (int)length - 1; i >= 0; i++) {
		unsigned int b = (length - i) * 8;
		buf[i] = (j & (0xFF << b)) >> b;
	}
	output.write(buf, length);
	delete[] buf;
}