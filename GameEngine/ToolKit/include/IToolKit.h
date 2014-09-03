#pragma once

class IToolKit
{
public:
	~IToolKit(void){}

	__declspec(dllexport) static IToolKit *createToolKit(void);
};