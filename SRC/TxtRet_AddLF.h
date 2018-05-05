
#if !defined(AFX_TXTRET_ADDLF_H__A8B6310D_CBA1_4750_A321_AE437A948B8B__INCLUDED_)
#define AFX_TXTRET_ADDLF_H__A8B6310D_CBA1_4750_A321_AE437A948B8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

enum TxtEncode {
	BYTE_OUT, //Only ASCII, ISO 8850-1, and WIN-1252
	UTF16LE,
	UTF16BE,
	UTF8
};

enum TxtTarget {
	ASCII_T,
	ISO_8859,
	WIN_1252,
	UNICODE_T
};

enum PETfont {
	LOWER_UPPER,
	UPPER_GRAPHIC
};

enum TxtNewLine {
	PASS_CR, //NO TRANSL (CP/M)
	CR_2_LF, //UNIX
	PRE_LF, //AMIGA/MAC
	SUF_LF //DOS/WIN
};

enum NoTxtTrans {
	CONTROL_ONLY,
	NO_POUND,
	NO_ASSIGN,
	NO_POUND_ASSIGN,
	NO_PI,
	NO_POUND_PI,
	NO_ASSIGN_PI,
	NO_POUND_ASSIGN_PI
};

#include <iostream>
#include <string>

typedef std::string* LPCTSTR;

bool delRetFromFldr(LPCTSTR source, LPCTSTR dest, bool verbose);
bool delRetFromFile(LPCTSTR source, LPCTSTR dest);

#endif // !defined(AFX_TXTRET_ADDLF_H__A8B6310D_CBA1_4750_A321_AE437A948B8B__INCLUDED_)
