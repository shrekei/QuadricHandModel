#pragma once
#include <string>

////////////////////////////////////////////////////////////////////////////////
//****************************** token.h *************************************//
// The Tokenizer class for reading simple ascii data files. The GetToken function
// just grabs tokens separated by whitespace, but the GetInt and Getdouble functions
// specifically parse integers and floating point numbers. SkipLine will skip to
// the next carraige return. FindToken searches for a specific token and returns
// true if it found it.
////////////////////////////////////////////////////////////////////////////////
class Tokenizer  
{
public:
	Tokenizer(void);
	virtual ~Tokenizer();	
	bool	Open(const std::string file);
	bool	Close(void);	
	bool	Abort(std::string error);	// Prints error & closes file, and always returns false
	
public:		// tokenization
	char		GetChar(void);
	char		CheckChar(void);
	int			GetInt(void);
	double		GetDouble(void);
	std::string	GetToken(void);
	std::string	CheckToken(void);
	bool		FindToken(const std::string tok);
	bool		SkipWhitespace(void);
	bool		SkipLine(void);
	bool		Reset(void);
	bool		IsEOF(void);
	bool		IsGood(void) { return m_bGood; }
	
public:		// access functions	
	std::string	GetFileName(void) { return m_strFileName; }
	int			GetLineNum(void) { return m_nLineNum; }
	
private:
	void*		m_pFile;
	std::string	m_strFileName;
	int			m_nLineNum;
	bool		m_bGood;
};

