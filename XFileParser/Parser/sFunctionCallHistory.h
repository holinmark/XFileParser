#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <utility>
#include <fstream>

namespace ns_HoLin
{
	struct sFunctionCallHistory
	{
		static std::vector<std::string> history;

		sFunctionCallHistory() {}
		sFunctionCallHistory(const char*);
		~sFunctionCallHistory();
		
		static BOOL PrintHistoryLog(BOOL = FALSE);
	};
}
