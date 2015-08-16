// ConsolePrinter.cpp: implementation of the CConsolePrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConsolePrinter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConsolePrinter* CConsolePrinter::_instance = 0;   
  
CConsolePrinter::CConsolePrinter(bool in)   
{   
    // create a new console to the process   
    AllocConsole();
	pFile = NULL;
	if (in == true)
	{
		hConsoleMode = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);   
		pFile  = _fdopen(hConsoleMode, "r");   
		
		// use default stream buffer   
		setvbuf(pFile, NULL, _IONBF, 0);   
		*stdin = *pFile;
	}
	else
	{
		hConsoleMode = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);   
		pFile  = _fdopen(hConsoleMode, "w");   
		
		// use default stream buffer   
		setvbuf(pFile, NULL, _IONBF, 0);   
		*stdout = *pFile;
	}
}   

CConsolePrinter::~CConsolePrinter()   
{   
    FreeConsole();
	if (pFile != NULL)
	{
		fclose(pFile);
	}
}   

CConsolePrinter* CConsolePrinter::Instance(bool in)   
{   
    if (_instance == 0)   
    {   
        _instance = new CConsolePrinter(in);   
    }   
    return _instance;
}   
  
void CConsolePrinter::Destroy()   
{   
    if (_instance)   
    {   
        delete _instance;   
    }   
    _instance = 0;   
}