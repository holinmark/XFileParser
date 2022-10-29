#include "stdafx.h"

namespace ns_HoLin
{
	sXFileData::~sXFileData()
	{
		Cleanup();
	}

	void sXFileData::Cleanup(HANDLE hfile)
	{
		smateriallist.clear();
		smeshlist.Cleanup();
		sframeslist.Cleanup();
		sanimationsetlist.Cleanup();
	}
	
	cTextXFileParser::cTextXFileParser()
	{
		linenumber = 1;
		trackoutput = FALSE;
	}

	cTextXFileParser::~cTextXFileParser()
	{
		linenumber = 1;
		trackoutput = FALSE;
		functioncalls.history.clear();
	}

	BOOL cTextXFileParser::ParseFile(PHANDLE hfile, BOOL btrack, BOOL accumulate_data)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		const std::size_t blen = 1024;
		char buff[blen];
		
		if (accumulate_data) {
			xfiledata.Cleanup();
			sfile.Cleanup();
		}
		trackoutput = btrack;
		sfile.SetFileHandle(hfile);
		try {
			while (TRUE) {
				try {
					if (!GetNextChar()) {
						break;
					}
				}
				catch (const std::wstring &error_str) {
					return TRUE;
				}
				if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess()))
					continue;
				else if (sfile.GetCurrentCharToProcess() == '/') {
					try {
						if (!GetComment()) {
							break;
						}
					}
					catch (const std::wstring &error_str) {
						return TRUE;
					}
					continue;
				}
				else if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
					buff[0] = sfile.GetCurrentCharToProcess();
					buff[1] = '\0';
					if (GetReservedWord(&buff[1], blen - 1, '{')) {
						if (!ExtractTemplates(buff, blen)) {
							break;
						}
					}
					else {
						return PrintOffendingLine("\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
							"Error extracting string", buff, "Mesh line number :", linenumber, __LINE__);
					}
				}
				else {
					return PrintOffendingLine("\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
						"Unknown token", sfile.GetCurrentCharToProcess(), "Mesh line number :", linenumber, __LINE__);
				}
			}
		}
		catch (const std::wstring &error_string) {
			MessageBox(nullptr, error_string.c_str(), L"Error!", MB_OK);
		}
		catch (const std::exception &e) {
			MessageBoxA(nullptr, e.what(), "Error!", MB_OK);
		}
		return FALSE;
	}

	cTextXFileParser::operator bool() const
	{
		if (sfile)
			return true;
		return false;
	}

	bool cTextXFileParser::operator !() const
	{
		if (sfile)
			return false;
		return true;
	}

	BOOL cTextXFileParser::GetNextChar()
	{
		if (sfile.MoveBufferIndex(trackoutput)) {
			if (sfile.GetCurrentCharToProcess() == '\r') {
				if (!sfile.MoveBufferIndex(trackoutput)) {
					return FALSE;
				}
			}
			if (sfile.GetCurrentCharToProcess() == '\n') {
				linenumber++;
			}
			if (trackoutput) {
#ifdef FUNCTIONCALLSTACK
				std::clog << sfile.GetCurrentCharToProcess();
#endif
			}
			return TRUE;
		}
		return FALSE;
	}

	BOOL cTextXFileParser::GetTemplateName(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		int ch = 0;

		buff[0] = '\0';
		if (sfile.GetCurrentCharToProcess() == (int)'{') {
			return TRUE;
		}
		if (!this->GetNextInput(IsValidCharOpening)) {
			return FALSE;
		}
		ch = sfile.GetCurrentCharToProcess();
		if (ch == (int)'{') {
			return TRUE;
		}
		if (ch == (int)'_' || std::isalpha(ch) || ch == (int)'.' || ch == (int)'-') {
			std::size_t limit = blen - 2, i = 1;
			
			buff[0] = static_cast<char>(ch);
			buff[1] = '\0';
			while(TRUE) {
				if (i == limit) {
					return PrintOffendingLine(
							"\r\n%s %zu\r\n%u\r\n",
							"Error maximum string length reached.\r\nMesh line number:",
							linenumber,
							__LINE__);
				}
				if (!GetNextChar()) {
					return FALSE;
				}
				ch = static_cast<int>(sfile.GetCurrentCharToProcess());
				if (ch == (int)' ' || ch == (int)'\n') {
					if (GetNextToken('{')) {
						return TRUE;
					}
					else {
						break;
					}
				}
				else if (ch == (int)'\r') {
					return this->GetCarriageReturn();
				}
				else if (std::isalpha(ch) || std::isdigit(ch) || ch == (int)'.' || ch == (int)'_' || ch == (int)'\'' || ch == (int)'-') {
					buff[i++] = static_cast<char>(ch);
					buff[i] = '\0';
				}
				else if (ch == (int)'{') {
					return TRUE;
				}
				else {
					
					return PrintOffendingLine(
						"\r\n%s \'%c\' %s \'{\'\r\n%s %zu\r\n%u\r\n",
						"Error unexpected token",
						static_cast<char>(ch),
						"expecting",
						"Mesh line number :",
						linenumber,
						__LINE__);
						
				}
			}
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error buffer overload.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		else {
			return this->PrintOffendingLine(
							"\r\n%s.\r\n%s %zu\r\n%u\r\n",
							"Error unexpected character",
							"Mesh line number:",
							linenumber,
							__LINE__);
		}
		return this->PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetDigit(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		int ch = 0;
		
		while (TRUE) {
			if (GetNextChar()) {
				ch = sfile.GetCurrentCharToProcess();
				if (isdigit(ch)) {
					buff[0] = static_cast<char>(ch);
					buff[1] = '\0';
					return TRUE;
				}
				else if (ch == (int)'/') {
					if (!GetComment())
						break;
				}
				else if (ch == (int)' ' || ch == (int)'\n' || ch == (int)'\t')
					continue;
				else if (ch == (int)'\r') {
					if (!GetCarriageReturn())
						return FALSE;
					continue;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		return PrintOffendingLine(
				"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
				"Unexpected token",
				static_cast<char>(ch),
				"Mesh line number :",
				linenumber,
				__LINE__);
	}

	BOOL cTextXFileParser::GetSignedDigit(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		while (TRUE) {
			if (GetNextChar()) {
				if (isdigit(sfile.GetCurrentCharToProcess()) || sfile.GetCurrentCharToProcess() == '-') {
					buff[0] = sfile.GetCurrentCharToProcess();
					buff[1] = '\0';
					return TRUE;
				}
				else if (sfile.GetCurrentCharToProcess() == '/') {
					if (GetComment())
						continue;
					else
						break;
				}
				else if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess()))
					continue;
				else if (sfile.GetCurrentCharToProcess() == '\r') {
					if (!GetCarriageReturn())
						return FALSE;
					continue;
				}
				else {
					return PrintOffendingLine(
						"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
						"Unexpected token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
				}
			}
			else {
				return PrintOffendingLine(
					"\r\n%s %zu\r\n%u\r\n",
					"Unexpected end of file. Mesh line number :",
					linenumber,
					__LINE__);
			}
		}
		return FALSE;
	}

	BOOL cTextXFileParser::GetMatrixBody(char *buff, std::size_t blen, void *plist, DWORD row, DWORD col)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetFloat(buff, blen))
			return FALSE;
		((DirectX::XMFLOAT4X4*)plist)->m[row][col] = (float)atof(buff);
		return TRUE;
	}

	BOOL cTextXFileParser::GetMatrix(char *buff, std::size_t blen, DirectX::XMFLOAT4X4 &matrix)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{')) {
				this->PrintOffendingLine(
					"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Error expecting \'{\' got",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number",
					linenumber,
					__LINE__);
				return FALSE;
			}
		}
		if (Get2DArray(buff, blen, (void*)&matrix, 4, 4, &cTextXFileParser::GetMatrixBody)) {
			if (GetNextToken('}')) {
				return TRUE;
			}
		}
		return PrintOffendingLine(
			"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
			"Unexpected token",
			sfile.GetCurrentCharToProcess(),
			"Mesh line number :",
			linenumber,
			__LINE__);
	}

	BOOL cTextXFileParser::GetFrameTransformMatrix(char *buff, std::size_t blen, DirectX::XMFLOAT4X4 &matrix)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{')) {
				return this->PrintOffendingLine(
					"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Error expecting \'{\' got",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number:",
					linenumber,
					__LINE__);
			}
		}
		if (Get2DArray(buff, blen, (void*)&matrix, 4, 4, &cTextXFileParser::GetMatrixBody)) {
			if (GetNextToken(';')) {
				if (GetNextToken('}')) {
					return TRUE;
				}
			}
		}
		return PrintOffendingLine(
						"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
						"Unexpected token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
	}

	BOOL cTextXFileParser::GetReservedWord(char *buff, std::size_t blen, char token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2;
		int ch = 0;
		
		for (std::size_t index = 0; index < limit; ++index) {
			if (GetNextChar()) {
				ch = static_cast<int>(sfile.GetCurrentCharToProcess());
				if (std::isalpha(ch) || isdigit(ch) || ch == (int)'.' || ch == (int)'_') {
					buff[index] = sfile.GetCurrentCharToProcess();
					buff[index + 1] = '\0';
				}
				else if (IsWhiteSpace(this, ch) || static_cast<char>(ch) == token) {
					return TRUE;
				}
				else if (ch == (int)'\r') {
					return GetCarriageReturn();
				}
				else if (ch == (int)'/') {
					return GetComment();
				}
				else {
					return PrintOffendingLine("\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
						"Unknown token", static_cast<char>(ch), "Mesh line number :", linenumber, __LINE__);
				}
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n",
					"Unexpected end of file. Mesh line number :", linenumber, __LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n",
			"Error buffer overload. Mesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetName(char *buff, std::size_t blen, char sep)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2;
		int ch = 0;
		
		while (TRUE) {
			if (!GetNextChar()) {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error. Mesh line number:", linenumber, __LINE__);
			}
			ch = static_cast<int>(sfile.GetCurrentCharToProcess());
			if (std::isalnum(ch) || ch == (int)'_' || ch == (int)'-' || ch == (int)'.') {
				break;
			}
			else if (IsWhiteSpace(this, ch)) {
				continue;
			}
			else if (ch == (int)'}') {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n",
					"Error no string input. Mesh line number :", linenumber, __LINE__);
			}
			else {
				return PrintOffendingLine("\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Error unknown token", static_cast<char>(ch), "Mesh line number :", linenumber, __LINE__);
			}
		}
		buff[0] = static_cast<char>(ch);
		buff[1] = '\0';
		for (std::size_t i = 1; i < limit; ++i) {
			if (GetNextChar()) {
				ch = sfile.GetCurrentCharToProcess();
				if (std::isalnum(ch) || ch == (int)'.' || ch == (int)'_' || ch == (int)'\'' || ch == (int)'-') {
					buff[i] = static_cast<char>(ch);
					buff[i + 1] = '\0';
				}
				else if (static_cast<char>(ch) == sep) {
					return TRUE;
				}
				else if (IsWhiteSpace(this, ch)) {
					if (!GetNextToken(sep))
						break;
					return TRUE;
				}
				else if (ch == (int)'\r') {
					if (!GetCarriageReturn())
						break;
					if (!GetNextToken('{'))
						break;
					return TRUE;
				}
				else if (ch == (int)'/') {
					if (!GetComment())
						break;
					if (!GetNextToken('{'))
						break;
					else
						return TRUE;
				}
				else {
					return PrintOffendingLine("\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
						"Unknown token", static_cast<char>(ch), "Mesh line number :", linenumber, __LINE__);
				}
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n",
					"Unexpected end of file. Mesh line number :", linenumber, __LINE__);
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetString(char *buff, std::size_t blen, char sep)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2, i = 0;

		while (TRUE) {
			if (!GetNextChar())
				return FALSE;
			if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess()))
				continue;
			else if (sfile.GetCurrentCharToProcess() == '\"')
				break;
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n",
					"Error expecting a string. Mesh line number :", linenumber, __LINE__);
			}
		}
		while (TRUE) {
			if (i == limit) {
				return PrintOffendingLine(
							"\r\n%s %zu\r\n%u\r\n",
							"Error maximum string length reached. Mesh line number",
							linenumber,
							__LINE__);
			}
			if (!GetNextChar())
				return FALSE;
			if (sfile.GetCurrentCharToProcess() == '\"') {
				if (i == 0) {
					return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error string length zero declared. Mesh line number :",
						linenumber,
						__LINE__);
				}
				if (!GetNextToken(sep)) {
					return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Error expecting token",
							sep,
							"Mesh line number :",
							linenumber,
							__LINE__);
				}
				return TRUE;
			}
			else if (std::isalnum((int)sfile.GetCurrentCharToProcess()) || std::ispunct((int)sfile.GetCurrentCharToProcess())) {
				buff[i++] = sfile.GetCurrentCharToProcess();
				buff[i] = '\0';
			}
			else {
				if (sfile.GetCurrentCharToProcess() == '\n' || sfile.GetCurrentCharToProcess() == '\r') {
					return PrintOffendingLine(
							"\r\n%s %zu\r\n%u\r\n",
							"Error unexpected carriage return. Mesh line number:",
							linenumber,
							__LINE__);
				}
				return PrintOffendingLine("\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Error extracting string. Unknown character",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error : buffer overload. Mesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetColorRGBA(char *buff, std::size_t blen, DirectX::XMFLOAT4 &c)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.x = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.y = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.z = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.w = (float)atof(buff);
		return TRUE;
	}

	BOOL cTextXFileParser::GetColorRGB(char *buff, std::size_t blen, DirectX::XMFLOAT3 &c)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.x = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.y = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		c.z = (float)atof(buff);
		return TRUE;
	}

	BOOL cTextXFileParser::GetMaterialAttributes(char *buff, std::size_t blen, ns_HoLin::sMaterial *p_material)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!VerifyToken('{'))
			return FALSE;
		if (strcmp(buff, "TextureFilename") == 0) {
			if (!GetString(buff, blen, ';'))
				return FALSE;
			p_material->filename = buff;
			if (!GetNextToken('}'))
				return FALSE;
			return TRUE;
		}
		return PrintOffendingLine(
					"\r\n%s \'%s\'\r\n%s %zu\r\n%u\r\n",
					"Unknown identifier",
					buff,
					"Mesh line number:",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetMaterialBody(char *buff, std::size_t blen, ns_HoLin::sMaterial *p_material)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetColorRGBA(buff, blen, p_material->facecolor))
			return FALSE;
		if (VerifyToken(';')) {
			if (!GetNextToken(';')) {
				return FALSE;
			}
		}
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		p_material->power = (float)atof(buff);
		if (!GetColorRGB(buff, blen, p_material->specularcolor))
			return FALSE;
		if (VerifyToken(';')) {
			if (!GetNextToken(';'))
				return FALSE;
		}
		if (!GetColorRGB(buff, blen, p_material->emissivecolor))
			return FALSE;
		if (VerifyToken(';')) {
			if (!GetNextToken(';'))
				return FALSE;
		}
		while (TRUE) {
			if (!GetNextInput(IsValidEntry))
				break;
			if (sfile.GetCurrentCharToProcess() == '}')
				return TRUE;
			if (sfile.GetCurrentCharToProcess() == '{') {
				if (!GetName(buff, blen, '}'))
					return FALSE;
				continue;
			}
			else if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
				buff[0] = sfile.GetCurrentCharToProcess();
				buff[1] = '\0';
				if (!GetReservedWord(&buff[1], blen - 1, '{'))
					return FALSE;
				if (!GetMaterialAttributes(buff, blen, p_material))
					return FALSE;
			}
			else {
				return PrintOffendingLine(
					"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number:",
					linenumber,
					__LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error GetMaterialBody. Mesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetUnsignedInteger(char *buff, std::size_t blen, void *plist)
	{
		// This function is shared by GetSkinWeightsBody
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;

		DWORD d = (DWORD)atoi(buff);

		((std::vector<DWORD>*)plist)->emplace_back(d);
		return TRUE;
	}

	BOOL cTextXFileParser::GetMeshMaterialListAttributes(char *buff, std::size_t blen, ns_HoLin::sMesh *pmesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() == '{') {
			if (!GetName(buff, blen, '}'))
				return FALSE;
			for (std::size_t i=0; i<pmesh->p_extra->smeshmateriallist.size(); ++i) {
				if (pmesh->p_extra->smeshmateriallist[i].name == std::string(buff))
					return TRUE;
			}
			return PrintOffendingLine(
						"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
						"Error could not find material named",
						buff,
						"Mesh line number :",
						linenumber,
						__LINE__);
		}
		else if (sfile.GetCurrentCharToProcess() == 'M') {
			buff[0] = sfile.GetCurrentCharToProcess();
			buff[1] = '\0';
			if (!GetReservedWord(&buff[1], blen - 1, '{'))
				return FALSE;
			if (strcmp((const char*)"Material", (const char*)buff) == 0) {
				ns_HoLin::sMaterial material;

				if (!GetMaterial(buff, blen, &material))
					return FALSE;
				pmesh->p_extra->smeshmateriallist.emplace_back(std::move(material));
				return TRUE;
			}
		}
		return PrintOffendingLine(
			"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
			"Error unexpected token",
			sfile.GetCurrentCharToProcess(),
			"Mesh line number :",
			linenumber,
			__LINE__);
	}

	BOOL cTextXFileParser::GetMeshMaterialListBody(char *buff, std::size_t blen, ns_HoLin::sMesh *pmesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD nmaterials = 0, number_of_face_indices = 0;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		nmaterials = (DWORD)atoi(buff);
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_face_indices = (DWORD)atoi(buff);
		if (!GetArray(buff, blen, (void*)&pmesh->p_extra->material_indices, number_of_face_indices, &cTextXFileParser::GetUnsignedInteger, FALSE))
			return FALSE;
		if (nmaterials == 0) {
			if (!GetNextToken('}'))
				return FALSE;
		}
		for (DWORD i = 0; i < nmaterials; ++i) {
			if (!GetNextInput(IsValidEntry))
				return FALSE;
			if (sfile.GetCurrentCharToProcess() == '}')
				break;
			if (!GetMeshMaterialListAttributes(buff, blen, pmesh))
				return FALSE;
		}
		if (nmaterials != pmesh->p_extra->smeshmateriallist.size()) {
			return PrintOffendingLine(
							"\r\n%s %zu %s %u %s %zu\r\n%u\r\n",
							"Error expecting",
							pmesh->p_extra->smeshmateriallist.size(),
							"materials got",
							nmaterials,
							"materials.\r\nMesh line number :",
							linenumber,
							__LINE__);
		}
		return GetNextToken('}');
	}

	BOOL cTextXFileParser::GetMeshMaterialList(char *buff, std::size_t blen, ns_HoLin::sMesh *pmesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{')) {
				return FALSE;
			}
		}
		if (GetMeshMaterialListBody(buff, blen, pmesh))
			return TRUE;
		return PrintOffendingLine(
			"%s %zu\r\n%u\r\n",
			"In GetMeshMaterialList. GetMeshMaterialListBody returned FALSE.\r\nMesh line number :",
			linenumber,
			__LINE__
		);
	}

	BOOL cTextXFileParser::GetMaterial(char *buff, std::size_t blen, ns_HoLin::sMaterial *p_material)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		memset((void*)buff, 0, blen);
		buff[0] = '\0';
		if (!GetTemplateName(buff, blen))
			return FALSE;
		if (strlen(buff) > 0)
			p_material->name = buff;
		else {
			return PrintOffendingLine(
				"\r\n%s %zu\r\n%u\r\n",
				"Material template not named.\r\nMesh line number :",
				linenumber,
				__LINE__);
		}
		return GetMaterialBody(buff, blen, p_material);
	}

	BOOL cTextXFileParser::GetUnsignedInteger(char *buff, std::size_t blen, BOOL getnexttoken)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2;
		
		if (!GetDigit(buff, blen))
			return FALSE;
		buff[0] = sfile.GetCurrentCharToProcess();
		buff[1] = '\0';
		for (std::size_t i = 1; i < blen; ++i) {
			if (GetNextChar()) {
				if (isdigit(sfile.GetCurrentCharToProcess())) {
					buff[i] = sfile.GetCurrentCharToProcess();
					buff[i + 1] = '\0';
				}
				else if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
					return TRUE;
				}
				else if (sfile.GetCurrentCharToProcess() == '\r') {
					if (!GetCarriageReturn())
						break;
					return TRUE;
				}
				if (sfile.GetCurrentCharToProcess() == ',' || sfile.GetCurrentCharToProcess() == ';') {
					if (getnexttoken) {
						if (!GetNextInput(IsValidSeperator)) {
							return PrintOffendingLine(NULL);
						}
					}
					return TRUE;
				}
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unexpected end of file.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Buffer overflow.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetExponent(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetSignedDigit(buff, blen)) {
			if (sfile.GetCurrentCharToProcess() == '-' || isdigit(sfile.GetCurrentCharToProcess())) {
				if (GetInteger(&buff[1], blen - 1)) {
					return TRUE;
				}
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetFraction(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2;

		for (std::size_t i = 0; i < limit; ++i) {
			if (!GetNextChar())
				return FALSE;
			if (isdigit(sfile.GetCurrentCharToProcess())) {
				buff[i] = sfile.GetCurrentCharToProcess();
				buff[i + 1] = '\0';
				continue;
			}
			else if (sfile.GetCurrentCharToProcess() == 'e' || sfile.GetCurrentCharToProcess() == 'E') {
				buff[i] = sfile.GetCurrentCharToProcess();
				buff[i + 1] = '\0';
				if (GetExponent(&buff[i + 1], (blen - i + 1)))
					return TRUE;
			}
			else if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess()))
				return TRUE;
			else if (sfile.GetCurrentCharToProcess() == '\r') {
				if (!GetNextChar())
					return FALSE;
				return TRUE;
			}
			else {
				return TRUE;
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error buffer overflow.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetInteger(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::size_t limit = blen - 2;

		for (std::size_t i = 0; i < limit; ) {
			if (!GetNextChar())
				return FALSE;
			if (isdigit(sfile.GetCurrentCharToProcess())) {
				buff[i] = sfile.GetCurrentCharToProcess();
				buff[++i] = '\0';
				continue;
			}
			else if (sfile.GetCurrentCharToProcess() == '.') {
				buff[i] = sfile.GetCurrentCharToProcess();
				buff[i + 1] = '\0';
				if (!GetFraction(&buff[i + 1], (blen - i - 1)))
					return FALSE;
				break;
			}
			else if (sfile.GetCurrentCharToProcess() == 'e' || sfile.GetCurrentCharToProcess() == 'E') {
				buff[i] = sfile.GetCurrentCharToProcess();
				buff[i + 1] = '\0';
				if (!GetExponent(&buff[i + 1], (blen - i - 1)))
					return FALSE;
				break;
			}
			else if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
				break;
			}
			else if (sfile.GetCurrentCharToProcess() == '\r') {
				if (!GetNextChar())
					return FALSE;
			}
			else if (sfile.GetCurrentCharToProcess() == '/') {
				if (!GetComment())
					return FALSE;
				break;
			}
			else
				break;
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetFloat(char *buff, std::size_t blen, BOOL getnexttoken)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetSignedDigit(buff, blen)) {
			if (GetInteger(&buff[1], blen - 1)) {
				if (getnexttoken) {
					if (!GetNextInput(IsValidSeperator))
						return FALSE;
				}
				return TRUE;
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetFloat(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		float value = 0.0;

		if (!GetFloat(buff, blen))
			return FALSE;
		value = (float)atof(buff);
		((std::vector<float>*)plist)->emplace_back(value);
		return TRUE;
	}
	
	BOOL cTextXFileParser::GetVectorBody(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DirectX::XMFLOAT3 f;

		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		f.x = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		f.y = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		f.z = (float)atof(buff);
		((std::vector<DirectX::XMFLOAT3>*)plist)->emplace_back(f);
		return TRUE;
	}
	
	BOOL cTextXFileParser::GetVector(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		if (!GetNextToken('{')) {
			return PrintOffendingLine("\r\n%s %zu %u\r\n", "Error mesh line number:", linenumber, __LINE__);
		}
		if (!GetVectorBody(buff, blen, plist)) {
			return PrintOffendingLine("\r\n%s %zu %u\r\n", "Error mesh line number:", linenumber, __LINE__);
		}
		if (!GetNextToken('}')) {
			return PrintOffendingLine("\r\n%s %zu %u\r\n", "Error mesh line number:", linenumber, __LINE__);
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetPolygon(char *buff, std::size_t blen, void *polygon)
	{
		// This function is shared by GetSkinWeightsBody and GetPolygons
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		((std::vector<DWORD>*)polygon)->emplace_back((DWORD)atoi(buff));
		return TRUE;
	}

	BOOL cTextXFileParser::GetPolygons(char *buff, std::size_t blen, void *polygons)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_vertices_in_polygon = 0;
		std::vector<DWORD> polygon_indices;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_vertices_in_polygon = (DWORD)atoi(buff);
		if (!GetArray(buff, blen, (void*)&polygon_indices, number_of_vertices_in_polygon, &cTextXFileParser::GetPolygon, FALSE))
			return FALSE;
		((std::vector<std::vector<DWORD>>*)polygons)->emplace_back(std::move(polygon_indices));
		if (sfile.GetCurrentCharToProcess() == ';' || sfile.GetCurrentCharToProcess() == ',') {
			return TRUE;
		}
		else {
			if (!GetNextInput(IsValidSeperator))
				return FALSE;
			else if (sfile.GetCurrentCharToProcess() == ';' || sfile.GetCurrentCharToProcess() == ',') {
				return TRUE;
			}
		}
		return PrintOffendingLine(
			"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
			"Error unexpecting token",
			sfile.GetCurrentCharToProcess(),
			"Mesh line nuber:",
			linenumber,
			__LINE__);
	}

	BOOL cTextXFileParser::GetFaces(char *buff, std::size_t blen, ns_HoLin::sMeshFaces &meshfaces)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_faces = 0;
		std::size_t number_of_indices = 0;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		number_of_faces = (DWORD)atoi(buff);
		if (!VerifyToken(';'))
			return FALSE;
		meshfaces.facevertexindices.reserve((std::size_t)number_of_faces);
		if (!GetArray(buff, blen, (void*)&meshfaces.facevertexindices, number_of_faces, &cTextXFileParser::GetPolygons))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() == ';') {
			for (auto f : meshfaces.facevertexindices) {
				if (f.size() > 0)
					number_of_indices += f.size();
			}
			meshfaces.number_of_indices = (DWORD)number_of_indices;
			return TRUE;
		}
		return PrintOffendingLine(
			"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
			"Error expecting \';\', got",
			sfile.GetCurrentCharToProcess(),
			"Mesh line number :",
			linenumber,
			__LINE__);
	}

	BOOL cTextXFileParser::GetArray(char *buff, std::size_t blen, void *plist, DWORD number_of_entries, BOOL(cTextXFileParser::*pCallBackFunction)(char*, std::size_t, void*), BOOL getnextinput)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		for (DWORD i = 0; i < number_of_entries; ++i) {
			if (!(this->*pCallBackFunction)(buff, blen, plist)) {
				return FALSE;
			}
			if (sfile.GetCurrentCharToProcess() == ',')
				continue;
			else if (sfile.GetCurrentCharToProcess() == ';') {
				if (!getnextinput) {
					if ((i + 1) == number_of_entries)
						break;
					else {

						return PrintOffendingLine(
							"\r\n%s %u %s %u\r\n%s %zu\r\n%u\r\n",
							"Error expecting",
							number_of_entries,
							"got",
							(i + 1),
							"Mesh line number :",
							linenumber,
							__LINE__
						);

					}
				}
				if (!GetNextInput(IsValidSeperator))
					return FALSE;
				if (sfile.GetCurrentCharToProcess() == ',')
					continue;
				else if (sfile.GetCurrentCharToProcess() == ';') {
					if ((i + 1) == number_of_entries)
						break;
					else {
						return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
					}
				}
			}
		}
		if (sfile.GetCurrentCharToProcess() == ';' || sfile.GetCurrentCharToProcess() == ',')
			return TRUE;
		
		return PrintOffendingLine(
					"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::Get2DArray(char *buff, std::size_t blen, void *plist, DWORD rows, DWORD cols, BOOL(cTextXFileParser::*pCallBackFunction)(char*, std::size_t, void*, DWORD, DWORD))
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		for (DWORD i = 0; i < rows; ++i) {
			for (DWORD j = 0; j < cols; ++j) {
				if (!(this->*pCallBackFunction)(buff, blen, plist, i, j))
					return FALSE;
				if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
					if (!GetNextInput(IsValidSeperator)) {
						return PrintOffendingLine(
							"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
							"Unknown token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
					}
				}
				if (sfile.GetCurrentCharToProcess() == ',')
					continue;
				if (sfile.GetCurrentCharToProcess() == ';') {
					if (i != j) {
						return PrintOffendingLine(
							"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
							"Unexpected token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
					}
					else
						continue;
				}
				else {
					return PrintOffendingLine(
						"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
						"Error unknown token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
				}
			}
		}
		if (sfile.GetCurrentCharToProcess() == ';' || sfile.GetCurrentCharToProcess() == ',') {
			return TRUE;
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetVertices(char *buff, std::size_t blen, std::vector<DirectX::XMFLOAT3> &vertices, DWORD numofvertices)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		buff[0] = '\0';
		if (GetArray(buff, blen, (void*)&vertices, numofvertices, &cTextXFileParser::GetVectorBody)) {
			return TRUE;
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetNormalFaceIndices(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_indices_in_polygon = 0;
		std::vector<DWORD> entries;
		
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_indices_in_polygon = (DWORD)atoi(buff);
		entries.reserve((std::size_t)number_of_indices_in_polygon);
		if (!GetArray(buff, blen, (void*)&entries, number_of_indices_in_polygon, &cTextXFileParser::GetPolygon, FALSE))
			return FALSE;
		((std::vector<std::vector<DWORD>>*)plist)->emplace_back(std::move(entries));
		return TRUE;
	}

	BOOL cTextXFileParser::GetMeshNormalsBody(char *buff, std::size_t blen, ns_HoLin::sMeshNormals *p_meshnormals)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;

		DWORD number_of_normals = (DWORD)atoi(buff);
		DWORD number_of_face_indices = 0;

		p_meshnormals->normals.reserve((std::size_t)number_of_normals);
		if (!GetArray(buff, blen, (void*)&p_meshnormals->normals, number_of_normals, &cTextXFileParser::GetVectorBody))
			return FALSE;
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_face_indices = (DWORD)atoi(buff);
		p_meshnormals->facenormals.reserve((std::size_t)number_of_face_indices);
		return GetArray(buff, blen, (void*)&p_meshnormals->facenormals, number_of_face_indices, &cTextXFileParser::GetNormalFaceIndices);
	}

	BOOL cTextXFileParser::GetMeshNormals(char *buff, std::size_t blen, ns_HoLin::sMeshNormals *p_meshnormals)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextInput(IsValidCharOpening)) {
				return FALSE;
			}
			if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
				return PrintOffendingLine(
						"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
						"Unknown token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
			}
		}
		if (sfile.GetCurrentCharToProcess() == '{') {
			if (GetMeshNormalsBody(buff, blen, p_meshnormals)) {
				if (sfile.GetCurrentCharToProcess() == '}')
					return TRUE;
				else {
					if (GetNextInput(IsValidCharClosing)) {
						if (sfile.GetCurrentCharToProcess() == '}')
							return TRUE;
						else {
							return PrintOffendingLine(
								"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
								"Error expecting \'}\' got",
								sfile.GetCurrentCharToProcess(),
								"Mesh line number :",
								linenumber,
								__LINE__);
						}
					}
				}
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error mesh line number: ", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetCoord2D(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DirectX::XMFLOAT2 f;

		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		f.x = (float)atof(buff);
		if (!GetFloat(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		f.y = (float)atof(buff);
		((std::vector<DirectX::XMFLOAT2>*)plist)->emplace_back(f);
		return TRUE;
	}

	BOOL cTextXFileParser::GetMeshTextureCoordBody(char *buff, std::size_t blen, std::vector<DirectX::XMFLOAT2> &t)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_texture_coord = 0;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_texture_coord = (DWORD)atoi(buff);
		t.reserve((std::size_t)number_of_texture_coord);
		return GetArray(buff, blen, (void*)&t, number_of_texture_coord, &cTextXFileParser::GetCoord2D);
	}

	BOOL cTextXFileParser::GetMeshTextureCoord(char *buff, std::size_t blen, std::vector<DirectX::XMFLOAT2> &t)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{'))
				return FALSE;
		}
		if (GetMeshTextureCoordBody(buff, blen, t)) {
			if (GetNextToken('}'))
				return TRUE;
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetSkinWeightsBody(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		ns_HoLin::sSkinWeights skinning;

		if (!GetString(buff, blen, ';'))
			return FALSE;
		skinning.transformNodeName = buff;
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		skinning.nWeights = (DWORD)atoi(buff);
		if (skinning.nWeights > 0) {
			if (!GetArray(buff, blen, (void*)&skinning.p_vertexIndices, skinning.nWeights, &cTextXFileParser::GetUnsignedInteger, FALSE))
				return FALSE;
			if (!VerifyToken(';'))
				return FALSE;
			if (!GetArray(buff, blen, (void*)&skinning.p_weights, skinning.nWeights, &cTextXFileParser::GetFloat, FALSE))
				return FALSE;
			if (!VerifyToken(';'))
				return FALSE;
		}
		if (!Get2DArray(buff, blen, (void*)&skinning.matrixOffset, (DWORD)4, (DWORD)4, &cTextXFileParser::GetMatrixBody))
			return FALSE;
		p_mesh->p_extra->p_skininfo->p_skin_weights.emplace_back(std::move(skinning));
		if (!GetNextToken(';'))
			return FALSE;
		if (!GetNextToken('}'))
			return PrintOffendingLine(NULL);
		return TRUE;
	}

	BOOL cTextXFileParser::GetSkinWeights(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetTemplateName(buff, blen)) {
			if (GetSkinWeightsBody(buff, blen, p_mesh)) {
				return TRUE;
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetXSkinMeshHeaderBody(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		if (!GetNextToken('}'))
			return FALSE;
		return TRUE;
	}

	BOOL cTextXFileParser::GetXSkinMeshHeader(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetNextInput(IsValidEntry)) {
			if (sfile.GetCurrentCharToProcess() == '{') {
				if (GetXSkinMeshHeaderBody(buff, blen, p_mesh))
					return TRUE;
			}
		}
		return PrintOffendingLine(
					"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Error unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetVertexDuplicationIndicesBody(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (!GetUnsignedInteger(buff, blen)) {
			return this->PrintOffendingLine(
					"\r\n%s %zu\r\n%u\r\n",
					"Error GetUnsignedInteger returned FALSE.\r\nMesh line number:",
					linenumber,
					__LINE__);
		}
		p_mesh->p_extra->sduplicates.nIndices = (DWORD)std::atoi(buff);
		if (sfile.GetCurrentCharToProcess() != ';') {
			if (!GetNextToken(';')) {
				return FALSE;
			}
		}
		if (!GetUnsignedInteger(buff, blen)) {
			return this->PrintOffendingLine(
					"\r\n%s %zu\r\n%u\r\n",
					"Error GetUnsignedInteger returned FALSE.\r\nMesh line number:",
					linenumber,
					__LINE__);
		}
		if (sfile.GetCurrentCharToProcess() != ';') {
			if (!GetNextToken(';'))
				return FALSE;
		}
		p_mesh->p_extra->sduplicates.nOriginalVertices = (DWORD)std::atoi(buff);
		if (!GetArray(buff, blen, (void*)&p_mesh->p_extra->sduplicates.Indices, (DWORD)p_mesh->p_extra->sduplicates.nIndices, &cTextXFileParser::GetUnsignedInteger, FALSE))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() == ';') {
			if (sfile.GetCurrentCharToProcess() != '}') {
				if (!GetNextToken('}')) {
					return FALSE;
				}
			}
		}
		else {
			return PrintOffendingLine(
				"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
				"Error expecting token \';\' got",
				sfile.GetCurrentCharToProcess(),
				"Mesh line number :",
				linenumber,
				__LINE__);
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetVertexDuplicationIndices(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (p_mesh) {
			if (sfile.GetCurrentCharToProcess() != '{') {
				if (!GetNextToken('{'))
					return FALSE;
			}
			if (!GetVertexDuplicationIndicesBody(buff, blen, p_mesh))
				return FALSE;
		}
		else {
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error, mesh memory not allocated.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetIndexColor(char *buff, std::size_t blen, void *plist)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		ns_HoLin::sIndexedColor c;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
			if (!GetNextToken(';'))
				return FALSE;
		}
		if (sfile.GetCurrentCharToProcess() == ';') {
			c.index = (DWORD)std::atoi(buff);
			if (GetColorRGBA(buff, blen, c.indexColor)) {
				((std::vector<ns_HoLin::sIndexedColor>*)plist)->emplace_back(c);
				return TRUE;
			}
		}
		return PrintOffendingLine(
					"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetMeshVertexColorsBody(char *buff, std::size_t blen, ns_HoLin::sMesh *pmesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD nVertexColors; // should equal number of polygons in mesh

		if (!pmesh)
			return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error no mesh declared.\r\nMesh line number :", linenumber, __LINE__);
		if (!GetUnsignedInteger(buff, blen, ';'))
			return FALSE;
		nVertexColors = (DWORD)std::atoi(buff);
		if (pmesh->meshfaces.number_of_indices == nVertexColors) {
			if (GetArray(buff, blen, (void*)&pmesh->p_extra->scolors.vertexColors, nVertexColors, &cTextXFileParser::GetIndexColor, FALSE)) {
				return TRUE;
			}
			return FALSE;
		}
		return PrintOffendingLine(
					"\r\n%s %zu\r\n%u\r\n",
					"Error number of colors and number of polygons do not match.\r\nMesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetMeshVertexColors(char *buff, std::size_t blen, ns_HoLin::sMesh *pmesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{'))
				return FALSE;
		}
		if (!GetMeshVertexColorsBody(buff, blen, pmesh))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() != '}') {
			if (!GetNextToken('}'))
				return FALSE;
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetMeshAttributes(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
			buff[0] = sfile.GetCurrentCharToProcess();
			buff[1] = '\0';
			if (GetReservedWord(&buff[1], blen - 2, '{')) {
				if (p_mesh->p_extra == nullptr) {
					p_mesh->p_extra = new ns_HoLin::sMeshExtraAttributes;
					if (p_mesh->p_extra == NULL) {
						return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
					}
				}
				if (strcmp(buff, "MeshMaterialList") == 0) {
					if (GetMeshMaterialList(buff, blen, p_mesh))
						return TRUE;
					return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
				}
				else if (strcmp(buff, "MeshNormals") == 0) {
					return GetMeshNormals(buff, blen, &p_mesh->p_extra->meshnormals);
				}
				else if (strcmp(buff, "Matrix4x4") == 0) {
					DirectX::XMFLOAT4X4 matrix;

					if (GetMatrix(buff, blen, matrix)) {
						p_mesh->p_extra->list_of_matrices.emplace_back(matrix);
						return TRUE;
					}
					return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
				}
				else if (strcmp(buff, "MeshTextureCoords") == 0) {
					return GetMeshTextureCoord(buff, blen, p_mesh->p_extra->texturecoord);
				}
				else if (strcmp(buff, "MeshVertexColors") == 0) {
					return GetMeshVertexColors(buff, blen, p_mesh);
				}
				else if (strcmp(buff, "VertexDuplicationIndices") == 0) {
					return GetVertexDuplicationIndices(buff, blen, p_mesh);
				}
				else if (strcmp(buff, "SkinWeights") == 0) {
					if (p_mesh->p_extra->p_skininfo == nullptr) {
						p_mesh->p_extra->p_skininfo = new ns_HoLin::sSkinInfo;
						if (p_mesh->p_extra->p_skininfo == nullptr) {
							return PrintOffendingLine(
										"\r\n%s %zu\r\n%u\r\n",
										"Error unable to allocate memory.\r\nMesh line number :",
										linenumber,
										__LINE__);
						}
					}
					return GetSkinWeights(buff, blen, p_mesh);
				}
				else if (strcmp(buff, "XSkinMeshHeader") == 0) {
					if (p_mesh->p_extra->p_skininfo) {
						if (p_mesh->p_extra->p_skininfo->skin_header.nBones > 0) {
							return PrintOffendingLine(
										"\r\n%s %zu\r\n%u\r\n",
										"Template XSkinMeshHeader already declared.\r\nMesh line number :",
										linenumber,
										__LINE__);
						}
					}
					else {
						p_mesh->p_extra->p_skininfo = new ns_HoLin::sSkinInfo;
						if (p_mesh->p_extra->p_skininfo == nullptr) {
							return PrintOffendingLine(
										"\r\n%s %zu\r\n%u\r\n",
										"Error unable to allocate memory.\r\nMesh line number :",
										linenumber,
										__LINE__);
						}
					}
					return GetXSkinMeshHeader(buff, blen, p_mesh);
				}
				else if (strcmp(buff, "Vector") == 0) {
					return this->GetVector(buff, blen, (void*)&p_mesh->p_extra->list_of_vectors);
				}
				else {
					return PrintOffendingLine(
								"\r\n%s \'%s\'\r\n%s %zu\r\n%u\r\n",
								"Unknown word",
								buff,
								"Mesh line number :",
								linenumber,
								__LINE__);
				}
			}
		}
		return PrintOffendingLine(
					"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
					"Unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetMeshBody(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_vertices = 0;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
				return FALSE;
		number_of_vertices = (DWORD)atoi(buff);
		p_mesh->vertices.reserve((std::size_t)(number_of_vertices * sizeof(DirectX::XMFLOAT3)));
		if (!GetVertices(buff, blen, p_mesh->vertices, number_of_vertices))
			return FALSE;
		if (!GetFaces(buff, blen, p_mesh->meshfaces))
			return FALSE;
		while (TRUE) {
			if (!GetNextInput(IsValidEntry)) {
				if (sfile.GetEndOfFileStatus()) {
					return PrintOffendingLine(
								"\r\n%s %zu\r\n%u\r\n",
								"Unexpected end of file. Possibly missing \'}\'.\r\nMesh line number :",
								linenumber,
								__LINE__);
				}
				return FALSE;
			}
			if (sfile.GetCurrentCharToProcess() == '}')
				break;
			else if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
				if (!GetMeshAttributes(buff, blen, p_mesh))
					return FALSE;
			}
			else if (sfile.GetCurrentCharToProcess() == '{') {
				if (!GetName(buff, blen, '}'))
					return FALSE;
				for (std::size_t i = 0; i < xfiledata.smateriallist.size(); ++i) {
					if (xfiledata.smateriallist[i].name == std::string(buff))
						return TRUE;
				}
				return PrintOffendingLine(
							"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
							"Error could not find material",
							buff,
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
			else {
				return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Error, unexpected token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetMesh(char *buff, std::size_t blen, ns_HoLin::sMesh *p_mesh)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		memset((void*)buff, 0, blen);
		buff[0] = '\0';
		if (!GetTemplateName(buff, blen))
			return FALSE;
		if (strlen(buff) > 0)
			p_mesh->name = buff;
		else {
			std::string name("Mesh");
			
			CreateName(buff, blen);
			sprintf_s(buff, blen, "%s_%zu", buff, xfiledata.smeshlist.number_of_meshes);
			name += buff;
			p_mesh->name = std::move(name);
		}
		return GetMeshBody(buff, blen, p_mesh);
	}

	BOOL cTextXFileParser::GetSubFrame(char *buff, std::size_t blen, ns_HoLin::sFrame *p_parentframe)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::string name;

		memset((void*)buff, 0, blen);
		buff[0] = '\0';
		if (!GetTemplateName(buff, blen))
			return FALSE;

		ns_HoLin::sSequenceOfFrames *pseq = xfiledata.sframeslist.GetLastSequence();
		ns_HoLin::sFrame *p_frame = pseq->CreateNewFrame();

		if (p_frame) {
			if (strlen(buff) == 0) {
				CreateName(buff, blen);
				sprintf_s(buff, blen, "%s_%zu", buff, p_frame->number_of_frames_created);
			}
			name = buff;
			p_frame->name = name;
			p_frame->parent_name = p_parentframe->name;
			if (name.length() > 0)
				pseq->framenames.emplace_back(name);
			if (pseq->parent_children.find(p_frame->parent_name) != pseq->parent_children.end()) {
				pseq->parent_children[p_frame->parent_name].emplace_back(p_frame->name);
			}
			else {
				pseq->parent_children.insert({p_parentframe->name, std::vector<std::string>({p_frame->name})});
			}
			if (!GetFrameBody(buff, blen, p_frame))
				return PrintOffendingLine(NULL);
		}
		else {
			return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetFrameAttributes(char *buff, std::size_t blen, ns_HoLin::sFrame *p_parentframe)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (strcmp(buff, "Frame") == 0) {
			if (GetSubFrame(buff, blen, p_parentframe))
				return TRUE;
		}
		else if (strcmp(buff, "FrameTransformMatrix") == 0) {
			buff[0] = '\0';
			if (GetFrameTransformMatrix(buff, blen, xfiledata.sframeslist.GetLastSequence()->plastframe->matrix))
				return TRUE;
		}
		else if (strcmp(buff, "Vector") == 0) {
			if (GetVector(buff, blen, (void*)&xfiledata.sframeslist.GetLastSequence()->plastframe->vectors)) {
				return TRUE;
			}
		}
		else if (strcmp(buff, "Mesh") == 0) {
			ns_HoLin::sMesh *p_mesh = xfiledata.smeshlist.CreateNewMesh();

			if (p_mesh) {
				buff[0] = '\0';
				if (GetMesh(buff, blen, p_mesh)) {
					p_parentframe->mesh.insert(p_parentframe->mesh.end(), p_mesh->name);
					return TRUE;
				}
				else
					return FALSE;
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		else {
			return PrintOffendingLine("\r\n%s \'%s\'\r\n%s %zu\r\n%u\r\n", "Unknown word", buff, "Mesh line number :", linenumber, __LINE__);
		}
		return FALSE;
	}

	BOOL cTextXFileParser::GetFrameBody(char *buff, std::size_t blen, ns_HoLin::sFrame *pframe)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		ns_HoLin::sSequenceOfFrames *pseq = nullptr;

		while (TRUE) {
			if (GetNextInput(IsValidEntry)) {
				if (sfile.GetCurrentCharToProcess() == '{') {
					if (!GetName(buff, blen, '}'))
						return FALSE;
					pseq = xfiledata.sframeslist.SearchSequence(std::string(buff));
					if (pseq == nullptr) {
						if (!xfiledata.smeshlist.Find(std::string(buff))) {
							return PrintOffendingLine(
										"\r\n%s \'%s\'\r\n%s %zu\r\n%u\r\n",
										"Error could not find object reference",
										buff,
										"Mesh line number :",
										linenumber,
										__LINE__);
						}
						pframe->mesh.emplace_back(std::string(buff));
					}
					else {
						if (pseq->pfirstframe) {
							pseq->pfirstframe->parent_name = xfiledata.sframeslist.GetLastSequence()->pfirstframe->name;
						}
					}
					continue;
				}
				else if (sfile.GetCurrentCharToProcess() == '}') {
					return TRUE;
				}
				else if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
					buff[0] = sfile.GetCurrentCharToProcess();
					buff[1] = '\0';
					if (!GetReservedWord(&buff[1], blen - 1, '{'))
						return FALSE;
					if (!GetFrameAttributes(buff, blen, pframe))
						return FALSE;
					continue;
				}
				else {
					return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Encountered error. Unknown token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
				}
			}
			else {
				if (sfile.GetEndOfFileStatus()) {
					return PrintOffendingLine(
								"\r\n%s %zu\r\n%u\r\n",
								"Unexpected end of file.\r\nMesh line number :",
								linenumber,
								__LINE__);
				}
				break;
			}
		}
		return PrintOffendingLine(
					"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Error unknown token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetFrame(char *buff, std::size_t blen, ns_HoLin::sSequenceOfFrames *pseq)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::string name;
		ns_HoLin::sFrame *pframe = nullptr;
		
		buff[0] = '\0';
		if (!GetTemplateName(buff, blen))
			return FALSE;
		pframe = pseq->CreateNewFrame();
		if (pframe != nullptr) {
			if (strlen(buff) > 0)
				name = buff;
			else {
				CreateName(buff, blen);
				sprintf_s(buff, blen, "%s_%zu", buff, pframe->number_of_frames_created);
				name = buff;
			}
			pframe->name = name;
			return GetFrameBody(buff, blen, pframe);
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetFloatKeysBody(char *buff, std::size_t blen, void *v)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_entries = 0;
		float value = 0.0f;

		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_entries = (DWORD)atoi(buff);
		for (DWORD i = 0; i < number_of_entries; ++i) {
			if (!GetFloat(buff, blen))
				return FALSE;
			if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
				if (!GetNextInput(IsValidSeperator))
					return FALSE;
			}
			value = static_cast<float>(atof(buff));
			((ns_HoLin::sTimedFloatKeys*)v)->tfkeys.emplace_back(value);
			if (sfile.GetCurrentCharToProcess() == ',')
				continue;
			else if (sfile.GetCurrentCharToProcess() == ';') {
				if ((i + 1) == number_of_entries)
					continue;
				else {
					return PrintOffendingLine(
								"\r\n%s %u %s %u.\r\n%s %zu\r\n%u\r\n",
								"Error expecting",
								number_of_entries,
								"timed float keys, got",
								i,
								"Mesh line number :",
								linenumber,
								__LINE__);
				}
			}
			else {
				return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Unexpected token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
		}
		if (sfile.GetCurrentCharToProcess() == ';')
			return TRUE;
		return PrintOffendingLine(
						"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
						"Unexpected token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
	}

	BOOL cTextXFileParser::GetTimedFloatKeys(char *buff, std::size_t blen, void *v)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		ns_HoLin::sTimedFloatKeys time_slot;
		
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		time_slot.time = (DWORD)atoi(buff);
		if (!GetFloatKeysBody(buff, blen, (void*)&time_slot))
			return FALSE;
		if (!GetNextToken(';'))
			return FALSE;
		((std::vector<ns_HoLin::sTimedFloatKeys>*)v)->emplace_back(std::move(time_slot));
		return TRUE;
	}

	BOOL cTextXFileParser::GetAnimationKeyBody(char *buff, std::size_t blen, sAnimation_Data *p_anim_data)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		DWORD number_of_keys = 0;
		std::vector<ns_HoLin::sTimedFloatKeys> transform_data;
		
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		p_anim_data->type_of_transform = (DWORD)atoi(buff);
		if (p_anim_data->type_of_transform >= 3) {
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error transform type must be 0, 1, or 2.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (!VerifyToken(';'))
			return FALSE;
		number_of_keys = (DWORD)atoi(buff);
		if (!GetArray(buff, blen, (void*)&p_anim_data->transformation_data, number_of_keys, &cTextXFileParser::GetTimedFloatKeys))
			return FALSE;
		//p_anim_data->transformation_data = transform_data;
		return TRUE;
	}

	BOOL cTextXFileParser::GetAnimationKey(char *buff, std::size_t blen, sAnimation_Data *p_anim_data)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{'))
				return FALSE;
		}
		if (!GetAnimationKeyBody(buff, blen, p_anim_data))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() != '}') {
			if (!GetNextToken('}')) {
				return FALSE;
			}
		}
		return TRUE;
	}

	BOOL cTextXFileParser::GetAnimationOptionsBody(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() != ';') {
			if (!GetNextToken(';'))
				return FALSE;
		}
		DWORD openclosed = (DWORD)std::atoi(buff);

		if (openclosed > 1) {
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error AnimationOptions, is either open or closed.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		if (sfile.GetCurrentCharToProcess() != ';') {
			if (!GetNextToken(';'))
				return FALSE;
		}
		DWORD positionquality = (DWORD)std::atoi(buff);
		if (positionquality > 1) {
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Error AnimationOptions, position quality is either linear or spline.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		if (sfile.GetCurrentCharToProcess() != '}') {
			if (!GetNextToken('}'))
				return FALSE;
		}
		return TRUE;
	}
	
	BOOL cTextXFileParser::GetAnimationOptions(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		
		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{'))
				return FALSE;
		}
		return GetAnimationOptionsBody(buff, blen);
	}

	BOOL cTextXFileParser::GetAnimationBody(char *buff, std::size_t blen, ns_HoLin::sAnimation *p_animation)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		
		while (TRUE) {
			if (!GetNextInput(IsValidEntry))
				break;
			if (sfile.GetCurrentCharToProcess() == '{') {
				if (!GetName(buff, blen, '}'))
					break;
				if (strlen(buff) == 0) {
					return PrintOffendingLine(
								"\r\n%s %zu\r\n%u\r\n",
								"Error no name entry.\r\nMesh line number :",
								linenumber,
								__LINE__);
				}
				std::string s(buff);
				
				if (!xfiledata.sframeslist.FindFrames(s)) {
					if (!xfiledata.smeshlist.Find(s)) {
						return PrintOffendingLine(
										"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
										"Could not find object reference",
										buff,
										"Mesh line number :",
										linenumber,
										__LINE__);
					}
				}
				p_animation->list_of_objects.emplace_back(s);
			}
			else if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
				buff[0] = sfile.GetCurrentCharToProcess();
				buff[1] = '\0';
				if (!GetReservedWord(&buff[1], blen - 1, '{'))
					return FALSE;
				if (strcmp(buff, "AnimationKey") == 0) {
					ns_HoLin::sAnimation_Data *p_anim_data = p_animation->CreateAnimation_Data();

					if (p_anim_data == nullptr) {
						return PrintOffendingLine(
									"\r\n%s %zu\r\n%u\r\n",
									"Unable to allocate memory.\r\nMesh line number :",
									linenumber,
									__LINE__);
					}
					if (!GetAnimationKey(buff, blen, p_anim_data))
						return FALSE;
				}
				else if (strcmp(buff, "AnimationOptions") == 0) {
					if (!GetAnimationOptions(buff, blen))
						return FALSE;
				}
				else {
					return PrintOffendingLine(
								"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
								"Unknown token",
								buff,
								"Mesh line number :",
								linenumber,
								__LINE__);
				}
			}
			else if (sfile.GetCurrentCharToProcess() == '}') {
				return TRUE;
			}
			else {
				return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Unknown token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetAnimation(char *buff, std::size_t blen, ns_HoLin::sAnimationSet *pset)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::string name;
		ns_HoLin::sAnimation *p_animation = nullptr;

		buff[0] = '\0';
		memset((void*)buff, 0, blen);
		if (!GetTemplateName(buff, blen)) {
			return FALSE;
		}
		if (strlen(buff) == 0) {
			CreateName(buff, blen);
			strcat_s(buff, blen, "_Animation");
		}
		name = buff;
		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{')) {
				return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Error unexpected token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
		}
		if (pset) {
			p_animation = pset->CreateAnimation();
		}
		else {
			p_animation = xfiledata.sanimationsetlist.CreateAnimation();
		}
		if (p_animation == nullptr) {
			return PrintOffendingLine(
						"\r\n%s %zu\r\n%u\r\n",
						"Unable to allocate memory.\r\nMesh line number :",
						linenumber,
						__LINE__);
		}
		p_animation->name = std::move(name);
		if (GetAnimationBody(buff, blen, p_animation))
			return TRUE;
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number : ", linenumber, __LINE__);
	}
	
	BOOL cTextXFileParser::GetAnimationSetBody(char *buff, std::size_t blen, sAnimationSet *pset)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		while (TRUE) {
			if (!GetNextInput(IsValidEntry))
				break;
			if (std::isalpha((int)sfile.GetCurrentCharToProcess())) {
				buff[0] = sfile.GetCurrentCharToProcess();
				buff[1] = '\0';
				if (!GetReservedWord(&buff[1], blen - 1, '{'))
					return FALSE;
				if (strcmp(buff, "Animation") == 0) {
					if (!GetAnimation(buff, blen, pset))
						break;
				}
				else {
					return PrintOffendingLine(
								"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
								"Error expecting \'Animation\', but got",
								buff,
								"Mesh line number :",
								linenumber,
								__LINE__);
				}
			}
			else if (sfile.GetCurrentCharToProcess() == '}')
				return TRUE;
			else {
				return PrintOffendingLine(
							"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
							"Unknown token",
							sfile.GetCurrentCharToProcess(),
							"Mesh line number :",
							linenumber,
							__LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number : ", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetAnimationSet(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		std::string name;

		buff[0] = '\0';
		memset((void*)buff, 0, blen);
		if (!GetTemplateName(buff, blen))
			return FALSE;
		if (strlen(buff) == 0) {
			CreateName(buff, blen);
			strcat_s(buff, blen, "_AnimationSet");
		}
		name = buff;

		ns_HoLin::sAnimationSet *pset = xfiledata.sanimationsetlist.CreateAnimationSet();

		if (pset) {
			pset->name = std::move(name);
			return GetAnimationSetBody(buff, blen, pset);
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error allocating memory.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetAnimTicksPerSecond(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != '{') {
			if (!GetNextToken('{'))
				return FALSE;
		}
		if (!GetUnsignedInteger(buff, blen))
			return FALSE;
		xfiledata.sanimationsetlist.animtickspersecond = (DWORD)atoi(buff);
		if (!VerifyToken(';'))
			return FALSE;
		return GetNextToken('}');
	}

	BOOL cTextXFileParser::ExtractTemplates(char *buff, std::size_t blen)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (strcmp(buff, "Mesh") == 0) {
			ns_HoLin::sMesh *p_mesh = xfiledata.smeshlist.CreateNewMesh();
	
			if (p_mesh) {
				return GetMesh(buff, blen, p_mesh);
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		else if (strcmp(buff, "Frame") == 0) {
			ns_HoLin::sSequenceOfFrames *pseq = xfiledata.sframeslist.CreateNewSequence();

			if (pseq) {
				return GetFrame(buff, blen, pseq);
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unable to allocate memory.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		else if (strcmp(buff, "Material") == 0) {
			ns_HoLin::sMaterial material;

			if (GetMaterial(buff, blen, &material)) {
				xfiledata.smateriallist.emplace_back(std::move(material));
				return TRUE;
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "GetMaterial failure.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		else if (strcmp(buff, "AnimationSet") == 0) {
			return GetAnimationSet(buff, blen);
		}
		else if (strcmp(buff, "AnimTicksPerSecond") == 0) {
			return GetAnimTicksPerSecond(buff, blen);
		}
		else if (strcmp(buff, "Animation") == 0) {
			return GetAnimation(buff, blen);
		}
		return PrintOffendingLine(
					"\r\n%s \'%s\'.\r\n%s %zu\r\n%u\r\n",
					"Unknown tempate",
					buff,
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::VerifyToken(char token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (sfile.GetCurrentCharToProcess() != token) {
			if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess())) {
				if (GetNextToken(token)) {
					return TRUE;
				}
			}
		}
		if (sfile.GetCurrentCharToProcess() == token)
			return TRUE;
		return PrintOffendingLine(
					"\r\n%s \'%c\' %s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Error unexpected token",
					sfile.GetCurrentCharToProcess(),
					"expecting",
					token,
					"Mesh line number :",
					linenumber,
					__LINE__);
	}

	BOOL cTextXFileParser::GetCarriageReturn()
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetNextChar()) {
			if (sfile.GetCurrentCharToProcess() != '\n') {
				return PrintOffendingLine(
								"\r\n%s %zu\r\n%u\r\n",
								"Unexpected carriage return.\r\nMesh line number :",
								linenumber,
								__LINE__);
			}
		}
		else {
			return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unexpected end of file.\r\nMesh line number :", linenumber, __LINE__);
		}
		return TRUE;
	}

	BOOL cTextXFileParser::ReadCommentChar()
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		while (TRUE) {
			if (GetNextChar()) {
				if (sfile.GetCurrentCharToProcess() == '\r') {
					if (GetCarriageReturn())
						return TRUE;
					else
						break;
				}
				else if (sfile.GetCurrentCharToProcess() == '\n')
					return TRUE;
			}
			else {
				if (sfile.GetEndOfFileStatus())
					return TRUE;
				break;
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetComment()
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		if (GetNextChar()) {
			if (sfile.GetCurrentCharToProcess() == '/') {
				if (ReadCommentChar())
					return TRUE;
			}
			else {
				return PrintOffendingLine(
					"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
					"Unexpected token",
					sfile.GetCurrentCharToProcess(),
					"Mesh line number :",
					linenumber,
					__LINE__);
			}
		}
		return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Unexpected end of file.\r\nMesh line number :", linenumber, __LINE__);
	}

	BOOL cTextXFileParser::GetNextInput(std::function<BOOL(int)> func)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif
		int ch = 0;
		
		while (TRUE) {
			if (GetNextChar()) {
				ch = sfile.GetCurrentCharToProcess();
				if (IsWhiteSpace(this, ch)) {
					continue;
				}
				if (func(ch)) {
					return TRUE;
				}
				if (sfile.GetCurrentCharToProcess() == '/') {
					if (GetComment())
						continue;
					else {
						break;
					}
				}
				else if (sfile.GetCurrentCharToProcess() == '\r') {
					if (!GetCarriageReturn()) {
						break;
					}
					continue;
				}
				else {
					return PrintOffendingLine(
						"\r\n%s \'%c\'.\r\n%s %zu\r\n%u\r\n",
						"Unexpected token",
						sfile.GetCurrentCharToProcess(),
						"Mesh line number :",
						linenumber,
						__LINE__);
				}
			}
			else {
				if (sfile.GetEndOfFileStatus()) {
					return PrintOffendingLine(
							"\r\n%s %zu\r\n%u\r\n",
							"Unexpected end of file error.\r\nMesh line number :",
							linenumber,
							__LINE__);
				}
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		return PrintOffendingLine(NULL);
	}

	BOOL cTextXFileParser::GetNextToken(const char token)
	{
#ifdef FUNCTIONCALLSTACK
		ns_HoLin::sFunctionCallHistory currentfunction(__func__);
#endif

		while (TRUE) {
			if (GetNextChar()) {
				if (sfile.GetCurrentCharToProcess() == token)
					return TRUE;
				if (IsWhiteSpace(this, (int)sfile.GetCurrentCharToProcess()))
					continue;
				if (sfile.GetCurrentCharToProcess() == '/') {
					if (GetComment())
						continue;
					else {
						return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error.\r\nMesh line number :", linenumber, __LINE__);
					}
				}
				else {
					return PrintOffendingLine(
							"\r\n%s \'%c\' %s \'%c\'\r\n%s %zu\r\n%u\r\n",
							"Error unexpected token",
							sfile.GetCurrentCharToProcess(),
							"expecting",
							token,
							"Mesh line number :",
							linenumber,
							__LINE__);
				}
			}
			else {
				return PrintOffendingLine("\r\n%s %zu\r\n%u\r\n", "Error unexpected end of file.\r\nMesh line number :", linenumber, __LINE__);
			}
		}
		
		return PrintOffendingLine(
			"\r\n%s \'%c\'\r\n%s %zu\r\n%u\r\n",
			"Error line number expecting",
			sfile.GetCurrentCharToProcess(),
			"Mesh line number :",
			linenumber,
			__LINE__);
			
	}
	/*
	void cTextXFileParser::GetOffendingLine(std::string &before, std::string &after)
	{
		DWORD index = 0, count = 0;
		
		for (index = sfile.index_of_next_char_to_read; index < sfile.GetPageSize(); ++index) {
			if (sfile.file_buffer[index] == '\n')
				break;
			else {
				after.append(1, sfile.file_buffer[index]);
			}
		}
		for (index = sfile.index_of_next_char_to_read; index < sfile.GetPageSize(); --index) {
			if (index == 0) {
				before.insert(std::begin(before), sfile.file_buffer[index]);
				for (count = (DWORD)(sfile.prevpage.size() - 1); count < sfile.GetPageSize(); --count) {
					if (sfile.prevpage[count] == '\n')
						break;
					before.insert(std::begin(before), sfile.prevpage[count]);
				}
				break;
			}
			before.insert(std::begin(before), sfile.file_buffer[index]);
			if (sfile.file_buffer[index] == '\n')
				break;
		}
	}
*/
	BOOL cTextXFileParser::PrintOffendingLine(const char *p_format, ...)
	{
#ifdef FUNCTIONCALLSTACK
		if (functioncalls.history.size() == 0)
			return FALSE;
#endif
		if (serrormessages.b_has_error_msg_outputed)
			return FALSE;
		
		std::unique_ptr<char[]> pbuff(new char[sfile.GetPageSize()]);
		va_list arg;
		std::ofstream fout("Errorlog.txt");
		DWORD bytes = 0;
		auto clogbuf = std::clog.rdbuf();
		std::string before, after;
		
		if (fout) {
			std::clog.rdbuf(fout.rdbuf());
		}
		sfile.GetOffendingLine(before, after);
		if (p_format ) {
			va_start(arg, p_format);
			if (pbuff) {
				bytes = _vsprintf_p(pbuff.get(), sfile.GetPageSize(), p_format, arg);
				std::clog << pbuff.get();
				MessageBoxA(nullptr, pbuff.get(), "Error", MB_OK);
				std::clog << '\n' << before << " Error here " << after << '\n';
			}
			va_end(arg);
		}
		functioncalls.PrintHistoryLog();
		std::clog.rdbuf(clogbuf);
		fout.close();
#ifdef _WINDOWS
		MessageBox(nullptr, L"Error, see Errorlog.txt file for more information.", L"Error", MB_OK);
#else
		std::wcout << L"See file \'Errorlog.txt\' for more information.\n";
#endif
		serrormessages.b_has_error_msg_outputed = TRUE;
		return FALSE;
	}
	
	BOOL IsWhiteSpace(ns_HoLin::cTextXFileParser *p, int ch)
	{
		if (ch == (int)' ' || ch == (int)'\t' || ch == (int)'\n')
			return TRUE;
		else if (ch == (int)'\r') {
			if (p->GetNextChar()) {
				if (p->sfile.GetCurrentCharToProcess() == '\n')
					return TRUE;
			}
		}
		return FALSE;
	}

	BOOL IsValidEntry(int ch)
	{
		if (std::isalpha((int)ch) || ch == (int)'}' || ch == (int)'{')
			return TRUE;
		return FALSE;
	}

	BOOL IsValidSeperator(int ch)
	{
		if (ch == (int)',' || ch == (int)';')
			return TRUE;
		return FALSE;
	}

	BOOL IsValidCharOpening(int ch)
	{
		if (std::isalpha(ch) || ch == (int)'{' || ch == (int)'_' || ch == (int)'.' || ch == (int)'-')
			return TRUE;
		return FALSE;
	}
	
	BOOL IsValidCharClosing(int ch)
	{
		if (std::isalpha(ch) || ch == (int)'}')
			return TRUE;
		return FALSE;
	}

	BOOL CreateName(char *buff, std::size_t blen)
	{
		char name[L_tmpnam_s];
		errno_t err = 0;
		std::size_t len = 0, temppathlength = 0, index = 0;

		GetTempPathA((DWORD)blen, buff);
		temppathlength = strlen(buff);
		err = tmpnam_s(name, L_tmpnam_s);
		if (err == 0) {
			err = strcpy_s((char*)buff, blen, (const char*)&name[temppathlength]);
			if (err == 0) {
				for (DWORD i = 0; i < strlen(buff); ++i) {
					if (buff[i] == '.')
						buff[i] = '_';
				}
				return TRUE;
			}
		}
		return FALSE;
	}
}
