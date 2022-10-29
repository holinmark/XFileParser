#pragma once

#ifndef		CBINARYXFILEPARSER_H
#define		CBINARYXFILEPARSER_H

#define TOKEN_NAME				1
#define TOKEN_STRING			2
#define TOKEN_INTEGER			3
#define TOKEN_GUID				5
#define TOKEN_INTEGER_LIST		6
#define TOKEN_FLOAT_LIST		7

#define TOKEN_OBRACE			10
#define TOKEN_CBRACE			11
#define TOKEN_OPAREN			12
#define TOKEN_CPAREN			13
#define TOKEN_OBRACKET			14
#define TOKEN_CBRACKET			15
#define TOKEN_OANGLE			16
#define TOKEN_CANGLE			17
#define TOKEN_DOT				18
#define TOKEN_COMMA				19
#define TOKEN_SEMICOLON			20
#define TOKEN_TEMPLATE			31
#define TOKEN_WORD				40
#define TOKEN_DWORD				41
#define TOKEN_FLOAT				42
#define TOKEN_DOUBLE			43
#define TOKEN_CHAR				44
#define TOKEN_UCHAR				45
#define TOKEN_SWORD				46
#define TOKEN_SDWORD			47
#define TOKEN_VOID				48
#define TOKEN_LPSTR				49
#define TOKEN_UNICODE			50
#define TOKEN_CSTRING			51
#define TOKEN_ARRAY				52

#define BUFFER_LENGTH			1024

#include <Windows.h>
#include "sFileIO.h"
#include "sFunctionCallHistory.h"
#include "sErrorMessageException.h"
#include <string>
#include <any>
#include <memory>
#include <stdexcept>

#ifdef FUNCTIONCALLSTACK
#include "Output.h"
#endif

namespace ns_HoLin
{
	struct sBinaryMeshHeaderFile
	{
		BOOL output_header_to_file;
		std::string c_header_file;
		
		sBinaryMeshHeaderFile() { output_header_to_file = FALSE; }
		~sBinaryMeshHeaderFile() { output_header_to_file = FALSE; }
		operator bool() { return (bool)output_header_to_file; }
		bool operator !() { return (bool)!output_header_to_file; }
	};
	
	class cBinaryXFileParser
	{
		char buffer[BUFFER_LENGTH];
		
	public:
		ns_HoLin::cFileInput sfile;
		ns_HoLin::sBinaryMeshHeaderFile needed_struct_file;
		
		cBinaryXFileParser();
		~cBinaryXFileParser();
		BOOL ParseFile(PHANDLE);
		
	protected:
		BOOL XFileTemplate(unsigned short&);
		BOOL XFileObject(unsigned short&);
		std::any DataPartsList(unsigned short&);
		BOOL DataPart(unsigned short&);
		std::any FollowingDataPart(unsigned short&);
		BOOL NumberList(unsigned short&);
		BOOL FloatList(unsigned short&);
		BOOL StringList(unsigned short&);
		DWORD ListSeperator(unsigned short&);
		BOOL DataReference(unsigned short&);
		BOOL TemplateMembersPart(unsigned short&);
		std::any FollowingTemplateMembersPart(unsigned short&);
		std::any FollowingTemplateMembersList(unsigned short&);
		BOOL TemplateMembersList(unsigned short&);
		BOOL TemplateMembers(unsigned short&);
		BOOL Array(unsigned short&);
		BOOL TemplateReference(unsigned short&);
		BOOL TemplateReferenceName(unsigned short&);
		BOOL FollowingPrimitiveType(unsigned short&);
		BOOL PrimitiveType(unsigned short&);
		BOOL PrimitiveArrayType(unsigned short&);
		void Integer(unsigned short&);
		BOOL DimensionSize(unsigned short&);
		BOOL Dimension(unsigned short&);
		BOOL FollowingDimensionList(unsigned short&);
		BOOL DimensionList(unsigned short&);
		std::any TemplateOptionList(unsigned short&);
		BOOL TemplateOptionInfo(unsigned short&);
		BOOL Ellipsis(unsigned short&);
		BOOL Name(unsigned short&);
		void ClassID(unsigned short&);
		std::any OptionalClassID(unsigned short&);
	};
}

#endif
