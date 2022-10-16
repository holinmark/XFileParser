#pragma once

#ifndef SERRORMESSAGEEXCEPTION_H
#define SERRORMESSAGEEXCEPTION_H

#include <Windows.h>
#include <string>
#include <exception>
#include "sFunctionCallHistory.h"

namespace ns_HoLin
{
	struct sErrorMessageException : public std::exception
	{
		BOOL b_has_error_msg_outputed;
		DWORD number_of_errors;
		std::string error_messages;
		
		sErrorMessageException();
		sErrorMessageException(const char*, unsigned int);
		sErrorMessageException(const char*, unsigned int, ns_HoLin::sFunctionCallHistory*);
		~sErrorMessageException();
		void DisplayError(ns_HoLin::sFunctionCallHistory&);
		const char* what() { return error_messages.c_str(); }
	};
}

#endif
