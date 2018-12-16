#pragma once
#include <vector>
#include <string>
namespace osomi {
	class ArrayHelper {
	public:
		//Returns whether or not the array contains an element
		//@param vector<T*> *v, address of the pointer array
		//@param T *t, address of the object
		//@param unsigned int maxOff, max offset of the object in the array
		//@return bool contains
		template<class T> static bool contains(std::vector<T*> *v, T *t, unsigned int maxOff);

		//Returns whether or not the array contains an element
		//@param vector<T*> *v, address of the pointer array
		//@param T *t, address of the object
		//@return bool contains
		template<class T> static bool contains(std::vector<T*> *v, T *t);

		//Selects all arrays that contain a specific object
		//@param vector<vector<T>> arr, search set
		//@param T element, element to search for
		//@return vector<vector<T>> result
		template<class T> static std::vector<std::vector<T>> select(std::vector<std::vector<T>> arr, T element);

		//Selects all string that contain a specific character
		//@param vector<string> arr, search set
		//@param char element, character to search for
		//@return vector<string> result
		static std::vector<std::string> selectStrings(std::vector<std::string> arr, char element);
	};
}