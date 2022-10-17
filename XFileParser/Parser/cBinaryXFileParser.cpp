#include "stdafx.h"

namespace ns_HoLin
{
	cBinaryXFileParser::cBinaryXFileParser()
	{
	}
	
	cBinaryXFileParser::~cBinaryXFileParser()
	{
	}
	
	BOOL cBinaryXFileParser::ParseFile(PHANDLE hfile)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		unsigned short token = 0;
		
		if (*hfile) {
			try {
				sfile.SetFileHandle(hfile);
				do {
					try {
						sfile.GetBytesFromFile((BYTE*)&token , sizeof(token), __LINE__, __FILE__);
					}
					catch (std::string &error_string) {
						error_string.clear();
						break;
					}
					if (token == TOKEN_TEMPLATE) {
						if (needed_struct_file) {
							needed_struct_file.c_header_file.append("template ");
						}
						if (!this->XFileTemplate(token)) {
							return FALSE;
						}
					}
					else {
						std::wcout << "\nXFileObject.\n";
						if (!this->XFileObject(token)) {
							return FALSE;
						}
						if (token != TOKEN_CBRACE) {
							std::wcout << __LINE__ << ' ' << " error expecting token closing brace.\n";
							return FALSE;
						}
					}
				} while (TRUE);
			}
			catch (const std::string &error_string) {
				f.PrintHistoryLog();
				std::wcout << error_string.c_str() << '\n';
				return FALSE;
			}
			catch (const wchar_t *p_error_string) {
				f.PrintHistoryLog();
				std::wcout << p_error_string << '\n';
				return FALSE;
			}
			catch (const std::length_error &len) {
				std::cout << len.what() << '\n';
				return FALSE;
			}
			catch (const std::bad_alloc &bad) {
				std::cout << bad.what() << '\n';
				return FALSE;
			}
			catch (sErrorMessageException *p_error_message) {
				if (p_error_message) {
					std::cout << p_error_message->error_messages << '\n';
					delete p_error_message;
					p_error_message = nullptr;
				}
				return f.PrintHistoryLog(FALSE);
			}
			std::cout << needed_struct_file.c_header_file << '\n';
			needed_struct_file.c_header_file.clear();
			return TRUE;
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::XFileTemplate(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;
		
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token != TOKEN_NAME) {
			throw(std::to_string(__LINE__) + std::string(" error expecting token name.\n"));
		}
		if (!this->Name(token)) {
			return FALSE;
		}
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token != TOKEN_OBRACE) {
			throw(std::to_string(__LINE__) + std::string(" error expecting opening brace.\n"));
		}
		if (needed_struct_file) {
			needed_struct_file.c_header_file.append(" {\n");
		}
		ret = this->OptionalClassID(token);
		if (ret.has_value()) {
			if (!std::any_cast<BOOL>(ret)) {
				f.PrintHistoryLog();
				return FALSE;
			}
		}
		if (!this->TemplateMembersPart(token)) {
			return f.PrintHistoryLog(FALSE);
		}
		if (token != TOKEN_CBRACE) {
			throw(std::to_string(__LINE__) + std::string(" error expecting closing brace.\n"));
		}
		if (needed_struct_file) {
			needed_struct_file.c_header_file.append("}\n\n");
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::XFileObject(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;
		
		if (token == TOKEN_NAME) {
			if (!this->Name(token)) {
				return FALSE;
			}
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (token == TOKEN_NAME) {
				if (!this->Name(token)) {
					return FALSE;
				}
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			}
		}
		else if (this->PrimitiveType(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (token != TOKEN_NAME) {
				std::wcout << __LINE__ << " error expecting token name read " << token << '\n';
				return FALSE;
			}
			if (!this->Name(token)) {
				return FALSE;
			}
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		}
		else { // needed do not remove
			std::wcout << __LINE__ << " error unexpected token " << token << '\n';
			return FALSE;
		}
		if (token == TOKEN_OBRACE) {
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("{ ");
			}
			ret = this->OptionalClassID(token);
			if (ret.has_value()) {
				if (!std::any_cast<BOOL>(ret)) {
					return FALSE;
				}
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			}
			return this->DataPartsList(token);
		}
		else {
			std::wcout << __LINE__ << ' ' << " unexpected token " << token << " expecting token brace.\n";
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::DataPartsList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->DataPart(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (token == TOKEN_CBRACE) {
				if (needed_struct_file) {
					needed_struct_file.c_header_file.append(" }\n");
				}
				return TRUE;
			}
			return this->FollowingDataPart(token);
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::DataPart(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		unsigned short working_with_current_token = token;

		working_with_current_token = token;
		if (this->NumberList(working_with_current_token)) {
			return TRUE;
		}
		working_with_current_token = token;
		if (this->FloatList(working_with_current_token)) {
			return TRUE;
		}
		if (this->DataReference(working_with_current_token)) {
			return TRUE;
		}
		working_with_current_token = token;
		if (this->XFileObject(working_with_current_token)) {
			return TRUE;
		}
		working_with_current_token = token;
		if (this->StringList(working_with_current_token)) {
			return TRUE;
		}
		working_with_current_token = token;
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::FollowingDataPart(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;
		
		if (this->DataPart(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			this->FollowingDataPart(token);
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::NumberList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		DWORD count = 0, size = 0;
		std::unique_ptr<int[]> integer_list;
		
		if (token == TOKEN_INTEGER_LIST) {
			sfile.GetBytesFromFile((BYTE*)&count, sizeof(count), __LINE__, __FILE__);
			integer_list = std::make_unique<int[]>(count);
			if (!integer_list) {
				throw(std::string("\nCouldn\'t allocate memory for integer list.\n"));
			}
			size = count * sizeof(int);
			sfile.GetBytesFromFile((BYTE*)integer_list.get(), size, __LINE__, __FILE__);
			for (DWORD i = 0; i < count; i++) {
				if (needed_struct_file) {
					needed_struct_file.c_header_file.push_back(' ');
					needed_struct_file.c_header_file.append(std::to_string(integer_list[i]));
					needed_struct_file.c_header_file.push_back(' ');
				}
			}
			return TRUE;
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::FloatList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		DWORD count = 0, size = 0;
		std::unique_ptr<float[]> float_list;
		
		if (token == TOKEN_FLOAT_LIST) {
			sfile.GetBytesFromFile((BYTE*)&count, sizeof(count), __LINE__, __FILE__);
			float_list = std::make_unique<float[]>(count);
			if (!float_list) {
				throw (std::string("\nCouldn\'t allocate memory for float list.\n"));
			}
			size = count * sizeof(float);
			sfile.GetBytesFromFile((BYTE*)float_list.get(), size, __LINE__, __FILE__);
			for (DWORD i = 0; i < count; i++) {
				if (needed_struct_file) {
					needed_struct_file.c_header_file.push_back(' ');
					needed_struct_file.c_header_file.append(std::to_string(float_list[i]));
					needed_struct_file.c_header_file.push_back(' ');
				}
			}
			return TRUE;
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::StringList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::unique_ptr<std::string[]> string_list;
		return FALSE;
	}
	
	DWORD cBinaryXFileParser::ListSeperator(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_COMMA) {
			return 1;
		}
		else if (token == TOKEN_SEMICOLON) {
			return 2;
		}
		f.PrintHistoryLog();
		return 0;
	}
	
	BOOL cBinaryXFileParser::DataReference(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;
		
		if (token == TOKEN_OBRACE) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (token == TOKEN_NAME) {
				if (this->Name(token)) {
					ret = this->OptionalClassID(token);
					if (ret.has_value()) {
						if (std::any_cast<BOOL>(ret)) {
							sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
							if (token == TOKEN_CBRACE) {
								return TRUE;
							}
							else {
								throw(std::string("\nError, unexpected token. ") + std::to_string(__LINE__));
							}
						}
					}
					else {
						return FALSE;
					}
				}
				else {
					throw(std::string("\nFunction Name() returned FALSE. ") + std::to_string(__LINE__));
				}
			}
			throw(std::string("\nUnexpected token. ") + std::to_string(__LINE__));
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::TemplateMembersPart(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;
		
		if (token == TOKEN_OBRACKET) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (this->TemplateOptionInfo(token)) {
				if (token == TOKEN_CBRACKET) {
					return TRUE;
				}
			}
		}
		else {
			if (this->TemplateMembersList(token)) {
				ret = this->FollowingTemplateMembersPart(token);
				return TRUE;
			}
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	std::any cBinaryXFileParser::FollowingTemplateMembersPart(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (token == TOKEN_CBRACE) {
			return TRUE;
		}
		if (token == TOKEN_OBRACKET) {
			return this->TemplateOptionInfo(token);
		}
		return {};
	}
	
	BOOL cBinaryXFileParser::FollowingTemplateMembersList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->TemplateMembers(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			return this->FollowingTemplateMembersList(token);
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::TemplateMembersList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->TemplateMembers(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			return this->FollowingTemplateMembersList(token);
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::TemplateMembers(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		unsigned short working_with_current_token = token;

		if (this->PrimitiveType(working_with_current_token)) {
			token = working_with_current_token;
			if (this->FollowingPrimitiveType(token)) {
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
				if (token == TOKEN_SEMICOLON) {
					if (needed_struct_file) {
						needed_struct_file.c_header_file.append(";\n");
					}
					return TRUE;
				}
				else {
					return f.PrintHistoryLog(FALSE);
				}
			}
			else {
				return f.PrintHistoryLog(FALSE);
			}
		}
		working_with_current_token = token;
		if (working_with_current_token == TOKEN_ARRAY) {
			if (this->Array(working_with_current_token)) {
				token = working_with_current_token;
				return TRUE;
			}
			else {
				return f.PrintHistoryLog(FALSE);
			}
		}
		working_with_current_token = token;
		if (working_with_current_token == TOKEN_NAME) {
			if (this->TemplateReference(working_with_current_token)) {
				token = working_with_current_token;
				return TRUE;
			}
			else {
				return TRUE;
			}
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::Array(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (token != TOKEN_ARRAY) {
			std::wcout << __LINE__ << " error, expecting token array.\n";
			return FALSE;
		}
		if (!this->PrimitiveArrayType(token)) {
			return FALSE;
		}
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token != TOKEN_NAME) {
			std::wcout << __LINE__ << " error, expecting token name. " << token << '\n';
			return FALSE;
		}
		if (!this->Name(token)) {
			return FALSE;
		}
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (!this->DimensionList(token)) {
			std::wcout << __LINE__ << " dimensionlist error.\n";
			return FALSE;
		}
		if (token == TOKEN_SEMICOLON) {
			return TRUE;
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::TemplateReference(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->Name(token)) {
			if (needed_struct_file) {
				needed_struct_file.c_header_file.push_back(' ');
			}
			if (this->TemplateReferenceName(token)) {
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
				if (token == TOKEN_SEMICOLON) {
					if (needed_struct_file) {
						needed_struct_file.c_header_file.append(";\n");
					}
					return TRUE;
				}
			}
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::TemplateReferenceName(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif

		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_NAME) {
			return this->Name(token);
		}
		else if (token == TOKEN_SEMICOLON) {
			if (needed_struct_file) {
				needed_struct_file.c_header_file.push_back('[');
			}
			return TRUE;
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::FollowingPrimitiveType(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_NAME) {
			if (this->Name(token)) {
				return TRUE;
			}
		}
		else if (token == TOKEN_SEMICOLON) {
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append(";\n");
			}
			return TRUE;
		}
		else {
		}
		return f.PrintHistoryLog();
	}
	
	BOOL cBinaryXFileParser::PrimitiveType(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		switch (token) {
		case TOKEN_WORD:
			return TRUE;
		case TOKEN_DWORD:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("DWORD ");
			}
			return TRUE;
		case TOKEN_FLOAT:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("FLOAT ");
			}
			return TRUE;
		case TOKEN_DOUBLE:
			return TRUE;
		case TOKEN_CHAR:
			return TRUE;
		case TOKEN_UCHAR:
			return TRUE;
		case TOKEN_SWORD:
			return TRUE;
		case TOKEN_SDWORD:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("SDWORD ");
			}
			return TRUE;
		case TOKEN_LPSTR:
			return TRUE;
		case TOKEN_UNICODE:
			return TRUE;
		case TOKEN_CSTRING:
			return TRUE;
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::PrimitiveArrayType(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		switch (token) {
		case TOKEN_WORD:
			return TRUE;
        case TOKEN_DWORD:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("DWORD ");
			}
			return TRUE;
        case TOKEN_FLOAT:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("FLOAT ");
			}
			return TRUE;
        case TOKEN_DOUBLE:
			return TRUE;
        case TOKEN_CHAR:
			return TRUE;
        case TOKEN_UCHAR:
			return TRUE;
        case TOKEN_SWORD:
			return TRUE;
        case TOKEN_SDWORD:
			if (needed_struct_file) {
				needed_struct_file.c_header_file.append("SDWORD ");
			}
			return TRUE;
        case TOKEN_LPSTR:
			return TRUE;
        case TOKEN_UNICODE:
			return TRUE;
        case TOKEN_CSTRING:
			return TRUE;
		case TOKEN_NAME:
			return this->Name(token);
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::Integer(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		int value = 0;
		
		sfile.GetBytesFromFile((BYTE*)&value, sizeof(value), __LINE__, __FILE__);
		if (needed_struct_file) {
			needed_struct_file.c_header_file.append(std::to_string(value));
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::DimensionSize(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif

		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_INTEGER) {
			if (this->Integer(token)) {
				return TRUE;
			}
		}
		else if (token == TOKEN_NAME) {
			return this->Name(token);
		}
		std::wcout << __LINE__ << " error, unexpected token. " << token << '\n';
		f.PrintHistoryLog();
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::Dimension(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (token == TOKEN_OBRACKET) {
			if (needed_struct_file) {
				needed_struct_file.c_header_file.push_back('[');
			}
			if (this->DimensionSize(token)) {
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
				if (token != TOKEN_CBRACKET) {
					std::wcout << __LINE__ << " error expecting closing bracket " << token << '\n';
					return FALSE;
				}
				if (needed_struct_file) {
					needed_struct_file.c_header_file.push_back(']');
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::FollowingDimensionList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->Dimension(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			if (token == TOKEN_OBRACKET) {
				return this->FollowingDimensionList(token);
			}
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::DimensionList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (this->Dimension(token)) {
			sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
			this->FollowingDimensionList(token);
			if (token == TOKEN_SEMICOLON) {
				if (needed_struct_file) {
					needed_struct_file.c_header_file.append(";\n");
				}
				return TRUE;
			}
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	std::any cBinaryXFileParser::TemplateOptionList(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (token == TOKEN_NAME) {
			if (this->Name(token)) {
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
				return this->TemplateOptionList(token);
			}
			else {
				return FALSE;
			}
		}
		return {};
	}
	
	BOOL cBinaryXFileParser::TemplateOptionInfo(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		std::any ret;

		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_DOT) {
			if (this->Ellipsis(token)) {
				sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
				return TRUE;
			}
		}
		else {
			ret = this->TemplateOptionList(token);
			if (ret.has_value()) {
				return std::any_cast<BOOL>(ret);
			}
			else {
				return TRUE;
			}
		}
		return FALSE;
	}
	
	BOOL cBinaryXFileParser::Ellipsis(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif

		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token != TOKEN_DOT) {
			return FALSE;
		}
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token != TOKEN_DOT) {
			return FALSE;
		}
		return TRUE;
	}
	
	BOOL cBinaryXFileParser::Name(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		DWORD count = 0, i = 0;
		char ch = 0;
		DWORD bytes_to_read = static_cast<DWORD>(sizeof(ch));
		
		if (token != TOKEN_NAME) {
			return FALSE;
		}
		sfile.GetBytesFromFile((BYTE*)&count, sizeof(count), __LINE__, __FILE__);
		while (TRUE) {
			if (i == (BUFFER_LENGTH - 1)) {
				throw(L"Error name length too long.");
			}
			if (i == count) {
				if (needed_struct_file) {
					needed_struct_file.c_header_file.append(std::string(buffer));
				}
				return TRUE;
			}
			sfile.GetBytesFromFile((BYTE*)&ch, bytes_to_read, __LINE__, __FILE__);
			if (i > 0) {
				if (std::isdigit(ch) || std::isalpha(ch) || ch == '_') {
					buffer[i++] = ch;
					buffer[i] = '\0';
				}
				else {
					throw(std::string("Error, unknown character in string.\n"));
				}
			}
			else {
				if (std::isalpha(static_cast<int>(ch)) || ch == '_') {
					buffer[i++] = ch;
					buffer[i] = '\0';
				}
				else {
					throw(std::string("Error, string must start with alpha character.\n"));
				}
			}
		}
		f.PrintHistoryLog();
		return FALSE;
	}
	
	void cBinaryXFileParser::ClassID(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		DWORD data1;
		WORD data2, data3;
		BYTE data4[8];
		
		sfile.GetBytesFromFile((BYTE*)&data1, sizeof(data1), __LINE__, __FILE__);
		sfile.GetBytesFromFile((BYTE*)&data2, sizeof(data2), __LINE__, __FILE__);
		sfile.GetBytesFromFile((BYTE*)&data3, sizeof(data3), __LINE__, __FILE__);
		sfile.GetBytesFromFile((BYTE*)data4, static_cast<DWORD>(_ARRAYSIZE(data4) * sizeof(BYTE)), __LINE__, __FILE__);
	}

	std::any cBinaryXFileParser::OptionalClassID(unsigned short &token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		sfile.GetBytesFromFile((BYTE*)&token, sizeof(token), __LINE__, __FILE__);
		if (token == TOKEN_GUID) {
			this->ClassID(token);
			return TRUE;
		}
		return {};
	}
}
