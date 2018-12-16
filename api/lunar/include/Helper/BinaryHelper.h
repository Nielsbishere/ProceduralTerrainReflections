#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace osomi {
	class BinaryHelper {
	private:
		static inline char getChar(char c);
		template<class I> static std::string toHex(I i);
	public:
		//Converts a single precision floating point to binary
		//@param float f
		//@return unsigned int i
		static unsigned int asBinary(float f);

		//Converts a double precision floating point to binary
		//@param double f
		//@return unsigned long long l
		static unsigned long long asBinary(double d);

		//Converts an unsigned int to hex
		//@param unsigned int i
		//@return string hex
		static std::string asHex(unsigned int i);

		//Converts an UINT64 to hex
		//@param unsigned long long (UINT64) i
		//@return string hex
		static std::string asHex(unsigned long long l);

		//Reads part of a stream and converts it to an unsigned int
		//@param ifstream file
		//@param unsigned int end, end of the unsigned int
		//@param unsigned int length, length of the unsigned int
		//@return unsigned int result
		static unsigned int readUInt(std::ifstream &input, unsigned int end, unsigned int length);

		//Writes an unsigned int to a file
		//@param ofstream file
		//@param unsigned int length, length of the unsigned int
		//@param unsigned int j, value to write
		static void writeUInt(std::ofstream &output, unsigned int length, unsigned int j);
	};
}