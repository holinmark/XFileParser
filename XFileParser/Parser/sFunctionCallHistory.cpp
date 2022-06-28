#include "stdafx.h"

namespace ns_HoLin
{
	std::vector<std::string> sFunctionCallHistory::history;

	sFunctionCallHistory::sFunctionCallHistory(std::string currentfunction)
	{
		sFunctionCallHistory::history.emplace_back(currentfunction);
	}

	sFunctionCallHistory::~sFunctionCallHistory()
	{
		if (sFunctionCallHistory::history.size() > 0) {
			sFunctionCallHistory::history.pop_back();
		}
	}

	void sFunctionCallHistory::Insert(std::string_view name)
	{
		sFunctionCallHistory::history.emplace_back(name);
	}
	
	BOOL sFunctionCallHistory::PrintHistoryLog(BOOL ret)
	{
		std::clog << "\nFunction history log\n";
		for (auto it : sFunctionCallHistory::history)
			std::clog << it << "()\n";
		sFunctionCallHistory::history.clear();
		return ret;
	}
}
