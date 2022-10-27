#include "stdafx.h"

namespace ns_HoLin
{
	std::vector<std::string> sFunctionCallHistory::history;

	sFunctionCallHistory::sFunctionCallHistory(const char *current_function_name)
	{
		sFunctionCallHistory::history.emplace_back(std::move(std::string(current_function_name)));
	}

	sFunctionCallHistory::~sFunctionCallHistory()
	{
		if (sFunctionCallHistory::history.size() > 0) {
			sFunctionCallHistory::history.pop_back();
		}
	}

	BOOL sFunctionCallHistory::PrintHistoryLog(BOOL ret)
	{
		if (sFunctionCallHistory::history.size() > 0) {
			ns_HoLin::WriteToConsole(L"%s\r\n", L"History log.");
			for (auto it : sFunctionCallHistory::history)
				ns_HoLin::WriteToConsoleA("%s\r\n", it.c_str());
			sFunctionCallHistory::history.clear();
		}
		return ret;
	}
}
