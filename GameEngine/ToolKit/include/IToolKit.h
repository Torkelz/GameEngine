#pragma once

class IToolKit
{
public:
	virtual ~IToolKit(void){}

	__declspec(dllexport) static IToolKit *createToolKit(void);

	virtual void hubbabubba(void) = 0;
};