// ConsolePrinter.h: interface for the CConsolePrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLEPRINTER_H__B6D47581_FFDA_430F_812C_63265C193B9D__INCLUDED_)
#define AFX_CONSOLEPRINTER_H__B6D47581_FFDA_430F_812C_63265C193B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <io.h>
#include <fcntl.h>

class CConsolePrinter  
{
public:    
    static void Destroy();   
    static CConsolePrinter* Instance(bool in);   	
    virtual ~CConsolePrinter();
  
protected:   
    CConsolePrinter(bool in);   
  
private:   
    static CConsolePrinter* _instance;
    FILE *pFile;
	int hConsoleMode;
};

#endif // !defined(AFX_CONSOLEPRINTER_H__B6D47581_FFDA_430F_812C_63265C193B9D__INCLUDED_)
