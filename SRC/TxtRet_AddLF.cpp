// TxtRet_AddLF.cpp : Defines the entry point for the console application.
//
// PETSCII Lower/Upper or Upper/Graphic to ASCII/Unicode16(LE)/UTF-8

#include "stdafx.h"
#include "TxtRet_AddLF.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// globals
enum TxtEncode outCode = BYTE_OUT; //any 8-bit code (anything but Unicode)
enum TxtTarget target = ASCII_T; //ASCII
enum PETfont inFont = LOWER_UPPER;
enum TxtNewLine outLine = SUF_LF;//for DOS/WIN
enum NoTxtTrans noTrans = CONTROL_ONLY; 
bool addBOM = false;

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}
	bool syntax=false;//assume no syntax error
	bool verbose=false;
	CString test;
	if (argc==4)
	{//check for output type
		test=LPCTSTR(argv[3]);
		if (test[0]=='-' || test[0]=='/') 
		{
			int pos=1;
			char parm = test[pos++];
			do {
				if (parm == 'v')
					verbose = true;
				else if (parm == 'B')
					addBOM = true;
				else if (parm == 'b')
					outCode = BYTE_OUT;
				else if (parm == 'u')
					outCode = UTF16LE;
				else if (parm == 'U')
					outCode = UTF16BE;
				else if (parm == '8')
					outCode = UTF8;
				else if (parm == 'a')
					target = ASCII_T;
				else if (parm == 'i')
					target = ISO_8859;
				else if (parm == 'I')
					target = UNICODE_T;
				else if (parm == 'w')
					target = WIN_1252;
				else if (parm == 'l')
					inFont = LOWER_UPPER;
				else if (parm == 'g')
					inFont = UPPER_GRAPHIC;
				else if (parm == 'c')
					outLine = PASS_CR;// CP/M
				else if (parm == 'p')
					outLine = CR_2_LF;// POSIX / UNIX
				else if (parm == 'm')
					outLine = PRE_LF;// MAC / AMIGA
				else if (parm == 'd')
					outLine = SUF_LF;// DOS / WINDOWS
				else if (parm == '0')
					noTrans = CONTROL_ONLY;
				else if (parm == '1')
					noTrans = NO_POUND;
				else if (parm == '2')
					noTrans = NO_ASSIGN;
				else if (parm == '3')
					noTrans = NO_POUND_ASSIGN;
				else if (parm == '4')
					noTrans = NO_PI;
				else if (parm == '5')
					noTrans = NO_POUND_PI;
				else if (parm == '6')
					noTrans = NO_ASSIGN_PI;
				else if (parm == '7')
					noTrans = NO_POUND_ASSIGN_PI;
				else
					syntax = true;
			} while ((parm = test[pos++]) && !syntax);

		} else
			syntax=true;
	}
	if (argc==2 && (test=LPCTSTR(argv[1])).Left(6).CompareNoCase("option")==0)
	{//list options
		cout << _T("The available (case-sensitive) options are:") << endl;
		cout << _T("v -- Verbose (list files processed).") << endl;
		cout << _T("l -- lower/upper PETSCII input [default].") << endl;
		cout << _T("g -- graphic/upper PETSCII input.") << endl;
		cout << _T("B -- add BOM to start of 16-bit encoded output.") << endl;
		cout << _T("a -- ASCII set output [default].") << endl;
		cout << _T("i -- ISO-8859 set output.") << endl;
		cout << _T("I -- Unicode set output.") << endl;
		cout << _T("w -- Win-1252 set output.") << endl;
		cout << _T("b -- Byte encoded output [default]. Unicode set not allowed!") << endl;
		cout << _T("u -- (little endian) 16-bit encoded output.") << endl;
		cout << _T("U -- (big endian) 16-bit encoded output.") << endl;
		cout << _T("8 -- UTF-8 encoded output.") << endl;
		cout << _T("c -- CP/M new line output (CR).") << endl;
		cout << _T("d -- DOS/Win new line output (CR+LF) [default].") << endl;
		cout << _T("m -- Mac/Amiga new line output (LF+CR).") << endl;
		cout << _T("p -- POSIX/UNIX new line output (LF).") << endl;
		cout << _T("0 -- Don't translate control codes [default].") << endl;
		cout << _T("1 -- Also no translate British pound (0x5c).") << endl;
		cout << _T("2 -- Also no translate left-arrow (0x5e).") << endl;
		cout << _T("3 -- No translate all of above (control, 0x5c, 0x5e).") << endl;
		cout << _T("4 -- Also no translate pi (0xff).") << endl;
		cout << _T("5 -- No translate pi or pound (control, 0x5c, 0xff).") << endl;
		cout << _T("6 -- No translate pi or underscore (control, 0x5e, 0xff).") << endl;
		cout << _T("7 -- No translate all of above (control, 0x5c, 0x5e, 0xff).") << endl;
		return 0;//no error, info only
	} else if (argc==2 && test.Left(7).CompareNoCase("example")==0)
	{//list examples
		cout << _T("Example 1, lower/upper PETSCII to byte-encoded ASCII:") << endl;
		cout << _T("\tpet2asc input.txt output.txt") << endl;
		cout << _T("Example 2, upper/graphic PETSCII to byte-encoded ASCII:") << endl;
		cout << _T("\tpet2asc input.txt output.txt -g") << endl;
		cout << _T("Example 3, lower/upper PETSCII to byte-encoded Win-1252:") << endl;
		cout << _T("\tpet2asc input.txt output.txt -w") << endl;
		cout << _T("Example 4, lower/upper PETSCII to UTF-16LE-encoded Unicode:") << endl;
		cout << _T("\tpet2asc input.txt output.txt -uI") << endl;
		cout << _T("Example 5, lower/upper PETSCII to UTF-16BE-encoded Unicode:") << endl;
		cout << _T("\tpet2asc input.txt output.txt -UI") << endl;
		cout << _T("Example 6, lower/upper PETSCII to UTF-8-encoded Unicode:") << endl;
		cout << _T("\tpet2asc input.txt output.txt -8I") << endl;
		cout << _T("All the above will generate DOS/Windows line endings (CR+LF).") << endl;
		cout << _T("Example 7, same as Example 6 but with LINUX/POSIX/UNIX line endings (LF):") << endl;
		cout << _T("\tpet2asc input.txt output.txt -8Ip") << endl;
		return 0;//no error, info only
	} else if (argc==2 && test.Left(6).CompareNoCase("detail")==0)
	{//list examples
		cout << _T("Most PETSCII low control codes (0~31) will pass-though (no translate).") << endl;
		cout << _T("PETSCII CR (13) will be translated in 1 of 4 ways:") << endl;
		cout << _T("\t1.CR (CP/M);  2.LF (UNIX);  3.LF+CR (Mac);  4.CR+LF (Win) [see OPTIONS]") << endl;
		cout << _T("PETSCII high control codes (128~159) will be translated as follows:") << endl;
		cout << _T("\toutput set ISO-8859-1/Unicode = pass-though (no translate).") << endl;
		cout << _T("\toutput set ASCII/Win-1252 = code 6 (ASCII 'ACK' [unused by CBM]).") << endl;
		cout << _T("If output set is *not* Unicode, only a few PETSCII 'graphics' will translate:") << endl;
		cout << _T("\t92 = 163 (British pound) if output is not ASCII (ISO-8850 or Win-1252).") << endl;
		cout << _T("\t125 (Sh-) = 124 (vertical bar).") << endl;
		cout << _T("\t163 (C=T) = 175 (macron) if output is not ASCII (ISO-8850 or Win-1252).") << endl;
		cout << _T("\t164 (C=@) = 95 (underscore).") << endl;
		cout << _T("\t171 (C=Q) = 125 \"}\".") << endl;
		cout << _T("\t179 (C=W) = 123 \"{\".") << endl;
		cout << _T("\t209 (ShQ) = 153 (bullet) if output not ASCII & input = graphic PETSCII.") << endl;
		cout << _T("Any other PETSCII code that can *not* be translated will be encoded as:") << endl;
		cout << _T("\tE0xx (priv.use) if Unicode output & input shared by both PETSCII sets") << endl;
		cout << _T("\tE0xx (priv.use) if Unicode output & input is upper/graphic PETSCII set") << endl;
		cout << _T("\tE1xx (priv.use) if Unicode output & input is lower/upper PETSCII set") << endl;
		cout << _T("\t127 (ASCII DEL) if the output set is *not* Unicode (ASCII/ISO-8850-1/Win-1252)") << endl;
		return 0;//no error, info only
	} else if (argc<3 || argc>4 || syntax)
	{//syntax error
		cout << _T("Syntax error!") << endl;
		cout << _T("Translate PETSCII to ASCII/ISO-8859-1/Unicode/Windwos-1252.") << endl;
		cout << _T("args: ORIG DEST [<-|/><option...>]") << endl;
		cout << _T("ORIG = Source filename or directory") << endl;
		cout << _T("DEST = Destination filename or directory") << endl;
		cout << _T("Notes: 1) If ORIG is a directory, DEST must be a directory.") << endl;
		cout << _T("       2) DEST directory name(s) must not exist as a file(s).") << endl;
		cout << _T("       3) DEST directory(s) will be created if not present.")<< endl;
		cout << _T("Enter 'pet2asc examples' for examples...") << endl;
		cout << _T("Enter 'pet2asc options' for list of options...") << endl;
		cout << _T("Enter 'pet2asc details' for more info...") << endl;
		return 1;//no file specified
	} else if (target == UNICODE_T && outCode == BYTE_OUT)
	{//logic error!
		cout << _T("Error!  Output set is Uniscode (16-bit) but encoding is 'byte' (8-bit)!!!") << endl;
		cout << _T("Try non-Unicode (8-bit) output set: ASCII/ISO-8859-1/Win-1252,") << endl;
		cout << _T("Or...") << endl;
		cout << _T("Try larger encoding (UTF-8, UTF-16LE, or UTF-16BE).") << endl;
		return 2;//error -- incompatible inputs
	}
	DWORD fAtrib=GetFileAttributes(LPCTSTR(argv[1]));
	if (fAtrib==-1)
	{
		cout << _T("Could not access: ") << LPCTSTR(argv[1]) << endl;
		return 2;
	}
	if (delRetFromFldr(LPCTSTR(argv[1]),LPCTSTR(argv[2]),verbose))//process initial folder
		return 0;
	return 2;//error processing
}

bool delRetFromFldr(LPCTSTR source, LPCTSTR dest, bool verbose)
{
	DWORD fAtrib=GetFileAttributes(source);
	if (fAtrib==-1)
		return false;//couldn't access source (the caller should've checked already)
	if (!(fAtrib&FILE_ATTRIBUTE_DIRECTORY))
		return delRetFromFile(source, dest);//just process a single file
/*	//extract the name of current folder from path
	CString fldrName=source;
	int pos=fldrName.ReverseFind('\\');
	if (pos>=0)
		fldrName=fldrName.Mid(pos+1);//kill the path (keep the name)*/
	if (verbose)
		cout << source << endl;
	CString searchPath=source;
	searchPath+=_T("\\*.*");
	WIN32_FIND_DATA searchData;
	HANDLE searchHandle;
	if ((searchHandle=FindFirstFile(searchPath,&searchData))==INVALID_HANDLE_VALUE)
		return true;//no files meet criteria
	_mkdir(dest);
/*	CString copyDir=dest;
	pos=copyDir.ReverseFind('\\');//find path seperator, if any
	if (pos>=0)
		copyDir=copyDir.Mid(pos+1);//keep the name (kill the path)*/
	bool good=true;//assume function succeeds
	CString origDir=source;//save origin folder path
	origDir+='\\';//this just speeds up the loop since we won't have to add \ each time
	do
	{//check each item in this folder
		//create path to item
		CString temp=origDir;
		temp+=searchData.cFileName;
		//check item attributes
		if ((fAtrib=GetFileAttributes(temp))==-1)
		{//couldn't access item
			cout << _T("\r\nCouldn't get attributes of ") << LPCTSTR(temp) << endl;
			good=false;//note failure;
			break;//exit loop
		}
		if (fAtrib&FILE_ATTRIBUTE_DIRECTORY)
		{//subdirectory found
			if (strcmp(searchData.cFileName,_T("."))==0 || strcmp(searchData.cFileName,_T(".."))==0)
				continue;//ignore parent and this folder
			//form path to subfolder
			CString temp2=dest;
			temp2+='\\';
			temp2+=searchData.cFileName;
			//process subfolder
			if (!delRetFromFldr(temp,temp2,verbose))//call this function recursively!
			{
				good=false;//note failure;
				break;//exit loop
			}
		}
		else
		{//found file
			if (!delRetFromFile(temp,dest))//process file
			{//function failed
				good=false;//note failure;
				break;//exit loop
			}
		}
	}
	while(FindNextFile(searchHandle,&searchData));
	FindClose(searchHandle);//free system resource
	return good;//return success / failure status
}

bool delRetFromFile(LPCTSTR source, LPCTSTR dest)
{
	CFile orig;
	CFileException ferror;
	if (!orig.Open(source,CFile::modeRead|CFile::shareDenyWrite,&ferror))
	{
		cout << _T("\r\nCouldn't open: ") << source << endl;
		return false;
	}
	CString copyName=dest;
	DWORD fAtrib=GetFileAttributes(copyName);
	if (fAtrib!=-1 && GetFileAttributes(copyName)&FILE_ATTRIBUTE_DIRECTORY)
	{//destination is a directory: use source name
		//prepare to extract souce filename from source path
		CString temp=source;
		int pos=temp.ReverseFind('\\');
		//create destination filename
		copyName+='\\';//destination folder name...
		copyName+=temp.Mid(pos+1);//plus source filename = dest path name
	}
#define	BUF_SIZE 2048
	char buffer[BUF_SIZE];
	char outBuf[3*BUF_SIZE];
	CFile copyFile;
	if (!copyFile.Open(LPCTSTR(copyName),CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
	{
		orig.Close();
		cout << _T("\r\nCouldn't create: ") << copyName << endl;
		return false;
	}

	if (addBOM && (outCode==UTF16LE || outCode==UTF16BE))
	{//add Byte-Order-Marker to output
		if (outCode==UTF16LE) {
			buffer[0] = 0xff;
			buffer[1] = 0xfe;
		} else {
			buffer[0] = 0xfe;
			buffer[1] = 0xff;
		}
		copyFile.Write(LPVOID(buffer),2);
	}

    unsigned int remain=orig.GetLength();
	while (orig.GetPosition()<remain)
	{
		int bufsiz=orig.Read(LPVOID(buffer),BUF_SIZE);
		int i=0;
		while (i<bufsiz && buffer[i]!='\r'/* && buffer[i]!='\n'*/) ++i;
		int j;//input position
		int k;//output position
		int lenC;//output bytes of encoded char
#ifdef _DEBUG
		for (j=0; j<3*BUF_SIZE; ++j)
			outBuf[j] = 0;//null
#endif
		bool found = i<bufsiz;
		for (j=k=0; j < i; ++j)
		{//each input character, translate...
			unsigned char inC;//input char
			DWORD rawC;//output char (raw, not encoded)
			DWORD encC;//output char (encoded, bloated)

			inC = buffer[j];

			//first translate to unicode
			if (inC < 0x41)
			{//control code, number or common punctuation (no translate)
				rawC = inC;
			} else if (inC < 0x5b)
			{// alphabetic
				rawC = inC; //assume input is PETSCII upper/graphic
				if (inFont == LOWER_UPPER)
					rawC ^= 0x20;//make ASCII/ISO/WIN lowercase (0x61~7a)
			} else if (inC < 0x60)
			{// rare punctuation 1
				rawC = inC; //assume no translation
				if (inC == 0x5c && noTrans != NO_POUND
					&& noTrans != NO_POUND_ASSIGN 
					&& noTrans != NO_POUND_PI)
				{//translate British pound)
					rawC = 0xa3;
				}
				if (inC == 0x5e && noTrans != NO_ASSIGN 
					&& noTrans != NO_POUND_ASSIGN 
					&& noTrans != NO_POUND_ASSIGN_PI)
				{//translate left arrow [assignment operator])
					rawC = 0x2190; //alternate 0x2b05 ?
				}
			} else if (inC < 0x80)
			{//translate non-standard PETSCII to standard PETSCII
				inC ^= 0xa0; //0x60...7f -> 0xc0...df
			} else if (inC >= 0xe0)
			{//translate non-standard PETSCII to standard PETSCII
				if (inC < 0xff)
					inC ^= 0x40; //0xe0...fe -> 0xa0...be
				else if (noTrans == NO_PI 
					|| noTrans == NO_POUND_PI 
					|| noTrans == NO_ASSIGN_PI 
					|| noTrans == NO_POUND_ASSIGN_PI)
					rawC = inC; //0xff ... no translate
				else if (inFont == LOWER_UPPER)
					rawC = 0xe1ff; //reversed diaganol cross-hatch (alt private use!)
				else
					rawC = 0x3c0; //greek lower-case pi
			}

			if (inC < 0x80)
				;//do nothing
			else if (inC < 0xa0)
			{//high control codes
				if (target == ASCII_T || target == WIN_1252)
				{//ASCII or Windows output
					rawC = 0x06; //ASCII ACK (unused CBM low control code)
				} else
				{//ISO 8859 or Unicode output
					rawC = inC; //no translation
				}
			} else if (inC < 0xc0)
				switch (inC)
			{//translate primary graphics (common to both PETSCII fonts)
				case 0xa0:
					rawC = inC; //no tranlation (non-breaking space)
					break;
				case 0xa1:
					rawC = 0x258c; //right half-block
					break;
				case 0xa2:
					rawC = 0x2584; //bottom half-block
					break;
				case 0xa3:
					if (target == ISO_8859 || target == WIN_1252)
						rawC = 0xaf ;//macron (not combining)
					else
						rawC = 0x2594; //top 1/8 block
					break;
				case 0xa4:
					if (target == ISO_8859 || target == WIN_1252)
						rawC = 0x5f ;//underscore
					else
						rawC = 0x2581; //bottom  1/8 block
					break;
				case 0xa5:
					rawC = 0x258f; //left 1/8 block (2/8 on TED and VIC-II)
					break;
				case 0xa6:
					rawC = 0x25a9; //diaganol cross-hatch
					break;
				case 0xa7:
					rawC = 0x2595; //right 1/8 block (2/8 on TED and VIC-II)
					break;
				case 0xa8:
					rawC = 0xe0a8; //bottom half cross-hatch (main private use!)
					break;
				case 0xa9:
					if (inFont == LOWER_UPPER)
						rawC = 0xe1a9; //lower-left to upper-right diaganol fill (alt private use!)
					else
						rawC = 0x25e4; //upper-left triangle
					break;
				case 0xaa:
					rawC = 0xe0aa; //right 2/8 block (main private use!)
					break;
				case 0xab:
					if (target == ISO_8859 || target == WIN_1252)
						rawC = 0x7d ;// }
					else
						rawC = 0x251c; //east-pointing T
					break;
				case 0xac:
					rawC = 0x2597; //bottom-right block
					break;
				case 0xad:
					rawC = 0x2514; //corner up and right
					break;
				case 0xae:
					rawC = 0x2510; //corner down and left
					break;
				case 0xaf:
					rawC = 0x2582; //bottom 2/8 block
					break;
				case 0xb0:
					rawC = 0x250c; //corner down and right
					break;
				case 0xb1:
					rawC = 0x2534; //north-pointing T
					break;
				case 0xb2:
					rawC = 0x253c; //south-pointing T
					break;
				case 0xb3:
					if (target == ISO_8859 || target == WIN_1252)
						rawC = 0x7b ;// {
					else
						rawC = 0x2524; //west-pointing T
					break;
				case 0xb4:
					rawC = 0x258e; //left 2/8 block
					break;
				case 0xb5:
					rawC = 0x258d; //left 3/8 block
					break;
				case 0xb6:
					rawC = 0xe0b6; //right 3/8 block (main private use!)
					break;
				case 0xb7:
					rawC = 0xe0b7; //top 2/8 block (main private use!)
					break;
				case 0xb8:
					rawC = 0xe0b8; //top 3/8 block (main private use!)
					break;
				case 0xb9:
					rawC = 0x2583; //bottom 3/8 block
					break;
				case 0xba:
					if (inFont == LOWER_UPPER)
						rawC = 0x2713; //check mark ... possibly Unicode square root 0x221a
					else
						rawC = 0x259f; //similar to L-miiror-x
					break;
				case 0xbb:
					rawC = 0x2596; //lower-left block
					break;
				case 0xbc:
					rawC = 0x259d; //upper-right block
					break;
				case 0xbd:
					rawC = 0x2518; //corner up and left
					break;
				case 0xbe:
					rawC = 0x2598; //upper-left block
					break;
				case 0xbf:
					rawC = 0x259a; //upper-left and lower-right blocks
					break;
			} else if (inC < 0xe0 && inFont == LOWER_UPPER)
			{//shifted lower/upper PETSCII
				if (inC == 0xc0)
					rawC = 0x2500; //horizontal bar at 5/8 (thick centered bar for TED / VIC-II)
				else if (inC < 0xdb)
					rawC = inC ^ 0x80; //make lower-case ASCII (0xc1 -> 0x61 ... 0xda -> 0x7a)
				else switch (inC) {
				case 0xdb:
					rawC = 0x253c; //cross... similar to +
					break;
				case 0xdc:
					rawC = 0xe0dc; //diaganol cross-hatch (left-half... main private use!)
					break;
				case 0xdd:
					if (target == ISO_8859 || target == WIN_1252)
						rawC = 0x7c ;// |
					else
						rawC = 0x2502; //vertical bar at 5/8 (centered thick bar for TED / VIC-II)
					break;
				case 0xde:
					rawC = 0xe1de; //reversed cross-hatch (alt private use!)
					break;
				case 0xdf:
					rawC = 0x25a7; //upper-left to lower-right diaganol fill
				}
			} else if (inC < 0xe0) 
				switch (inC)
			{//shifted upper/graphic PETSCII
				case 0xc0:
					rawC = 0x2500; //horizontal bar at 5/8 (center thick bar for TED / VIC-II)
					break;
				case 0xc1:
					rawC = 0x2660; //spade suit
					break;
				case 0xc2:
					rawC = 0xe0c2; //vertical bar at 4/8 (main private use! centered thick bar for TED / VIC-II)
					break;
				case 0xc3:
					rawC = 0xe0c3; //horizontal bar at 4/8 (main private use! centered thick bar for TED / VIC-II)
					break;
				case 0xc4:
					rawC = 0xe0c4; //horizontal bar at 3/8 (main private use!)
					break;
				case 0xc5:
					rawC = 0xe0c5; //horizontal bar at 2/8 (main private use!)
					break;
				case 0xc6:
					rawC = 0xe0c6; //horizontal bar at 6/8 (main private use!)
					break;
				case 0xc7:
					rawC = 0xe0c7; //vertical bar at 3/8 (main private use!)
					break;
				case 0xc8:
					rawC = 0xe0c8; //vertical bar at 6/8 (main private use!)
					break;
				case 0xc9:
					rawC = 0x256e; //arc bottom to left
					break;
				case 0xca:
					rawC = 0x2570; //arc top to right
					break;
				case 0xcb:
					rawC = 0x266f; //arc top to left
					break;
				case 0xcc:
					rawC = 0x2599; //bottom-left corner (similar to L)
					break;
				case 0xcd:
					rawC = 0x2572; //diaganol top-left to bottom right (similar to \)
					break;
				case 0xce:
					rawC = 0x2571; //diaganol cross (similar to X)
					break;
				case 0xcf:
					rawC = 0x259b; //top-left corner (similar to L mirror-Y)
					break;
				case 0xd0:
					rawC = 0x259c; //top-right corner (similar to 7)
					break;
				case 0xd1: //possibly 0x2022
					rawC = 0x2219; //solid circle (bullet)
					break;
				case 0xd2:
					rawC = 0xe0d2; //horizontal bar at 7/8 (main private use!)
					break;
				case 0xd3:
					rawC = 0x2665; //Heart suit (black)
					break;
				case 0xd4:
					rawC = 0xe0d4; //horizontal bar at 2/8 (main private use!)
					break;
				case 0xd5:
					rawC = 0x256d; //arc bottom to right
					break;
				case 0xd6:
					rawC = 0x2573; //diaganol cross (similar to X)
					break;
				case 0xd7:
					rawC = 0x2218; //hollow circle (bullet)
					break;
				case 0xd8:
					rawC = 0x2663; //Spade suit
					break;
				case 0xd9:
					rawC = 0xe0d9; //vertical bar at 7/8 (main private use!)
					break;
				case 0xda:
					rawC = 0x2666; //Diamond suit (black)
					break;
				case 0xdb:
					rawC = 0x253c; //cross (similar to +)
					break;
				case 0xdc:
					rawC = 0xe0dc; //diaganol cross hatch (left-half... main private use!)
					break;
				case 0xdd: //posibly ASCII +124
					rawC = 0x2502; //vertical bar at 5/8 (centered thick bar for TED / VIC-II)
					break;
				case 0xde:
					rawC = 0x3c0; //greek lower-case pi
					break;
				case 0xdf:
					rawC = 0x25e5; //upper-right triangle
					break;
			}

			//now have unicode char 'rawC'
			//if output char-set is not unicode, remove invalid code...
			if (target == ASCII_T)
			{//ASCII output
				if (rawC > 0x7f) {
					rawC = 0x7f; //invalid
				}
			} else if (target == ISO_8859)
			{//ISO output
				if (rawC > 0xff) {
					rawC = 0x7f; //invalid
				}
			} else if (target == WIN_1252)
			{//WIN output
				if (rawC == 0x2022 || rawC == 0x2219)
					rawC = 0x95; //bullet (solid circle)
				else if (rawC > 0xff || rawC >= 0x80 && rawC < 0xa0) {
					rawC = 0x7f; //invalid
				}
			}// else Unicode output

			//next, encode...
			lenC = 0;
			if (outCode == BYTE_OUT)
			{//simple 8-bit
				encC = rawC;
				if (encC >= 0x100)
				{//oops!
					ASSERT(false);
					encC = 0x7f; //invalid
				}
				lenC = 1;// 1-byte
			} else if (outCode == UTF16LE)
			{//simple 16-bit
				encC = rawC;
				lenC = 2;// 2-byte
			} else if (outCode == UTF16BE)
			{//reversed 16-bit
				encC = ((rawC & 0xff) << 8) | (rawC >> 8);
				lenC = 2;// 2-byte
			} else
			{//UTF-8 ... multibyte / complex!
				if (rawC < 0x80)
				{//1-byte sequence
					encC = rawC;
					lenC = 1;
				} else if (rawC < 0x800)
				{//2-byte sequence
					encC = ((rawC & 0x3f | 0x80) << 8) | ((rawC & 0x7c0) << 2) | 0xc0;
					lenC = 2;
				} else if (rawC < 0x10000)
				{//3-byte sequence
					encC = ((rawC & 0x3f | 0x80) << 16) | (( ((rawC & 0xfc0) >> 6) | 0x80) << 8) | ((rawC & 0xf000) >> 12) | 0xe0;
					lenC = 3;
				} else
				{//oops!
					ASSERT(false);
					encC = 0x7f; //invalid
					lenC = 1;
				}
			}

			//finally, store byte(s) in output buffer
			for (int o=0; o<lenC; ++o) {
				outBuf[k+o] = encC & 0xff;
				encC >>= 8;
			}
			k += lenC;
			ASSERT(k <= 3*(BUF_SIZE-found));
		} //each input char

        if (found)
        {//found return character...
			switch (outLine) 
			{//write 'new line' to output buffer
			default: //oops!
				ASSERT(false);
			case PASS_CR:
				if (outCode == UTF16LE) {
					outBuf[k] = 0x0d; //CR
					outBuf[k+1] = 0x00;
					lenC = 2;
				} else if (outCode == UTF16BE) {
					outBuf[k] = 0x00;
					outBuf[k+1] = 0x0d; //CR
					lenC = 2;
				} else {
					outBuf[k] = 0x0d; //CR
					lenC = 1;
				}
				break;
			case CR_2_LF:
				if (outCode == UTF16LE) {
					outBuf[k] = 0x0a; //LF
					outBuf[k+1] = 0x00;
					lenC = 2;
				} else if (outCode == UTF16BE) {
					outBuf[k] = 0x00;
					outBuf[k+1] = 0x0a; //LF
					lenC = 2;
				} else {
					outBuf[k] = 0x0a; //LF
					lenC = 1;
				}
				break;
			case PRE_LF:
				if (outCode == UTF16LE) {
					outBuf[k] = 0x0a; //LF
					outBuf[k+1] = 0x00;
					outBuf[k+2] = 0x0d; //CR
					outBuf[k+3] = 0x00;
					lenC = 4;
				} else if (outCode == UTF16BE) {
					outBuf[k] = 0x00;
					outBuf[k+1] = 0x0a; //LF
					outBuf[k+2] = 0x00;
					outBuf[k+3] = 0x0d; //CR
					lenC = 4;
				} else {
					outBuf[k] = 0x0a; //LF
					outBuf[k+1] = 0x0d; //CR
					lenC = 2;
				}
				break;
			case SUF_LF:
				if (outCode == UTF16LE) {
					outBuf[k] = 0x0d; //CR
					outBuf[k+1] = 0x00;
					outBuf[k+2] = 0x0a; //LF
					outBuf[k+3] = 0x00;
					lenC = 4;
				} else if (outCode == UTF16BE) {
					outBuf[k] = 0x00;
					outBuf[k+1] = 0x0d; //CR
					outBuf[k+2] = 0x00;
					outBuf[k+3] = 0x0a; //LF
					lenC = 4;
				} else {
					outBuf[k] = 0x0d; //CR
					outBuf[k+1] = 0x0a; //LF
					lenC = 2;
				}
				break;
			}
			k += lenC;
		}

		//write buffer to file
		copyFile.Write(LPVOID(outBuf),k);

		//backup to character after input-CR
		if (found)
			orig.Seek(i-bufsiz+1,CFile::current);
	}
	orig.Close();
	copyFile.Close();
	return true;
}
