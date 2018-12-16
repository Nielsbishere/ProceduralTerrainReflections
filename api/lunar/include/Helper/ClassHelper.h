#pragma once
//Returns whether or not you can cast the object to a class
//@param O1 the object you are casting
//@template param 0 the class you are casting to
//@return boolean castable
template<typename O2, typename O1> bool instanceof(O1 &obj);

//Returns whether or not you can cast the object to a class
//@param O1 the pointer to the object you are casting
//@template param 0 the class you are casting to
//@return boolean castable
template<typename O2, typename O1> bool instanceof(O1 *obj);