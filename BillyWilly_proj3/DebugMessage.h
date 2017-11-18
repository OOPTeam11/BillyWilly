#pragma once
#include <debugapi.h>
#include <string>

void Debug(std::string str){
	OutputDebugString(str.c_str());
}