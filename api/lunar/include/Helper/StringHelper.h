#pragma once
#include <vector>
#include <string>
namespace osomi {
	class StringHelper {
	public:
		//Returns whether or not a string ends with a certain value
		//@param string s, search string
		//@param string end, end string
		//@return bool endsWith
		static bool endsWith(std::string s, std::string end);

		//Returns whether or not a string starts with a certain value
		//@param string s, search string
		//@param string start, start string
		//@return bool startsWith
		static bool startsWith(std::string s, std::string begin);

		//Returns the contents from a file to string
		//@param string file, file path
		//@return string contents
		static std::string fromFile(std::string file);

		//Returns a string of a number with prefix and postfix
		//@param unsigned int i, number
		//@param string prefix
		//@param string postfix
		//@return string result
		static std::string numString(unsigned int i, std::string prefix="", std::string postfix="");

		//Returns a substring of s's copy
		//@param string s, text
		//@param unsigned int i, offset
		//@return string result
		static std::string substring(std::string s, unsigned int i);

		//Returns a substring of s's copy
		//@param string s, text
		//@param unsigned int st, offset
		//@param unsigned int en, offset back
		//@return string result
		static std::string substring(std::string s, unsigned int st, unsigned int en);

		//Returns an array of strings by splitting the string by a character
		//@param string s, text
		//@param char c, delim
		//@return vector<string> result
		static std::vector<std::string> split(std::string s, char c);

		//Grabs the path of a file
		//@param string s, file path
		//@return string directory path
		static std::string grabPath(std::string s);

		//Grabs the file path without extension
		//@param string s, file path
		//@return string path with file name
		static std::string grabFileNoExt(std::string s);

		//Grabs the file name and extension of a file path
		//@param string s, file path
		//@return string file
		static std::string grabFile(std::string s);

		//Trims a string
		//@param string s, to trim
		//@param dubbleWhitespace, whether or not it allows dubble whitespace
		//@return string trimmed string
		static std::string trim(std::string s, bool dubbleWhitespace = false);

		//Parses an unsigned int from a string
		//@param string s
		//@return unsigned int val
		static unsigned int parseInt(std::string s);

		//Parses a signed int from a string
		//@param string s
		//@return int val
		static int parseSInt(std::string s);
	};
}