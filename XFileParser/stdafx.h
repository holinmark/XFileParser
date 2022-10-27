#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define FUNCTIONCALLSTACK

// Windows Header Files
#include <Windows.h>
#include <DirectXMath.h>

// C RunTime Header Files
#include <stdlib.h>
#include <Shobjidl.h>
#include <WRL\client.h>
#include <tchar.h>

#include <cstdio>
#include <cstdarg>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <any>
#include <vector>
#include <tuple>
#include <limits>
#include <functional>
#include <new>
#include <cctype>
#include <stdexcept>
#include <exception>

#include "Resource.h"
#include "Output.h"
#include "cBaseWindow.h"
#include "cApplication.h"
#include "cBinaryXFileParser.h"
#include "cTextXFileParser.h"
#include "cXFile.h"
#include "sAnimationSet.h"
#include "sErrorMessageException.h"
#include "sFileIO.h"
#include "sFrameList.h"
#include "sFunctionCallHistory.h"
#include "SkinInfo.h"
#include "sMaterialList.h"
#include "sMeshList.h"
