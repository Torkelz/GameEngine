#pragma once

#include <string>

//
// class IResourceExtraData		- Chapter 8, page 224 (see notes below)
//
//   This isn't mentioned specifically on that page, but it is a class that can attach extra data to 
//   a particular resource. Best example is storing the length and format of a sound file. 
//   There's a great discussion of this in Chapter 13, "Game Audio"
//
class IResourceExtraData
{
public:
	virtual std::string toString() = 0;
};