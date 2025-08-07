#pragma once

class IFormatController {
public:
	virtual void GenCodes(std::vector<std::wstring>& codes) = 0;
};