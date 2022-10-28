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
			std::clog << "History log.\n";
			for (auto it : sFunctionCallHistory::history)
				std::clog << it.c_str() << '\n';
			sFunctionCallHistory::history.clear();
		}
		return ret;
	}
}
