#pragma once
#ifndef MODULEFINDER_H
#define MODULEFINDER_H

#include "SYSHeader.h"

class ModuleFinder
{
public:
	bool GetModule(DWORD Pid, const std::wstring& ModuleName, MODULEENTRY32W& Entry);
	
private:

};

#endif // !MODULEFINDER_H
