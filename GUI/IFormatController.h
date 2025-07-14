#pragma once
#include <vector>
#include <string>

class IFormatController {
public:
	virtual void GenCodes(std::vector<std::wstring>& codes) = 0;
};