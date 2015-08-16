#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "Tokenizer.h"
using namespace std;

Tokenizer::Tokenizer()
{
	m_bGood = false;
	m_pFile = 0;
	m_nLineNum = 0;
	m_strFileName = "";
}

Tokenizer::~Tokenizer()
{
	if (m_pFile != 0)
	{
		printf("ERROR: Tokenizer::~Tokenizer()- Closing file '%s'\n", m_strFileName);
		fclose((FILE*)m_pFile);
	}
	m_bGood = false;
}

// open the skel file
bool Tokenizer::Open(const string fname)
{
	m_pFile = (FILE*)fopen(fname.c_str(), "r");
	if (m_pFile == 0)
	{
		cout << "ERROR: crTokenzier::Open()- Can't open file " << fname << endl;
		return false;
	}
	m_nLineNum = 1;
	m_strFileName = fname;
	m_bGood = true;
	return true;
}

bool Tokenizer::Close()
{
	if (m_pFile != 0)
		fclose((FILE*)m_pFile);
	else
		return false;

	m_pFile = 0;
	return true;
}

bool Tokenizer::Abort(string error)
{
	cout << "ERROR: " << m_strFileName << ", line " << m_nLineNum << ", " << error << endl;;
	Close();
	m_bGood = false;
	return false;
}

bool Tokenizer::IsEOF(void)
{
	return feof((FILE*)m_pFile);
}

char Tokenizer::GetChar()
{
	char c = char(getc((FILE*)m_pFile));
	if (c == '\n')
		m_nLineNum++;
	return c;
}

char Tokenizer::CheckChar()
{
	int c = getc((FILE*)m_pFile);
	ungetc(c, (FILE*)m_pFile);
	return char(c);
}

int Tokenizer::GetInt()
{
	SkipWhitespace();
	int pos=0;
	char temp[256];

	// Get first character ('-' or digit)
	char c = CheckChar();
	if (c == '-')
	{
		temp[pos++] = GetChar();
		c = CheckChar();
	}
	if (!isdigit(c))
	{
		cout << "ERROR: Tokenizer::GetInt()- Expecting int on line " << m_nLineNum << " of " << m_strFileName << endl;
		m_bGood = false;
		return 0;
	}
	temp[pos++]=GetChar();

	// Get integer potion
	while(isdigit(c = CheckChar()))
		temp[pos++]=GetChar();

	// Finish
	temp[pos++]='\0';
	return atoi(temp);
}

// BUG: can't parse ".2", "f", or "F"
// Uses: [-]I[.[I]][(e|E)[+|-]I]
// Should use: [+|-](I|I.|.I|I.I)[(e|E)[+|-]I][f|F]
double Tokenizer::GetDouble()
{
	SkipWhitespace();
	int pos=0;
	char temp[256];

	// get first character ('-' or digit)
	char c=CheckChar();
	if (c == '-')
	{
		temp[pos++]=GetChar();
		c=CheckChar();
	}
	if (!isdigit(c))
	{
		cout << "ERROR: Tokenizer::GetDouble()- Expecting double on line " << m_nLineNum << " of " << m_strFileName << " " << c <<endl;
		m_bGood = false;
		return 0.0f;
	}
	temp[pos++]=GetChar();

	// get integer potion of mantissa
	while (isdigit(c = CheckChar()))
		temp[pos++] = GetChar();

	// get fraction component
	if (c == '.')
	{
		temp[pos++]=GetChar();
		while(isdigit(c=CheckChar())) temp[pos++]=GetChar();
	}

	// Get exponent
	if (c=='e' || c=='E')
	{
		temp[pos++]=GetChar();
		c=CheckChar();
		if (c=='+' || c=='-')
		{
			temp[pos++]=GetChar();
			c=CheckChar();
		}
		if (!isdigit(c))
		{
			cout << "ERROR: Tokenizer::GetDouble()- Poorly formatted double exponent on line " << m_nLineNum << " of " << m_strFileName << endl;
			m_bGood = false;
			return 0.0f;
		}
		while (isdigit(c=CheckChar())) 
		{
			temp[pos++]=GetChar();
		}
	}

	// Finish
	temp[pos++]='\0';
	return double(atof(temp));
}

string Tokenizer::GetToken(void)
{
	long orgpos = ftell((FILE*)m_pFile);
	SkipWhitespace();

	char buf[256];
	int pos=0;
	char c = CheckChar();
	while (c!=' ' && c!='\n' && c!='\t' && !feof((FILE*)m_pFile))
	{
		buf[pos++] = GetChar();
		c = CheckChar();
	}
	buf[pos]='\0';
	return string(buf);
}

string Tokenizer::CheckToken(void)
{
	long orgpos = ftell((FILE*)m_pFile);
	int nLineNum = m_nLineNum;
	SkipWhitespace();

	char buf[256];
	int pos=0;
	char c = CheckChar();
	while (c!=' ' && c!='\n' && c!='\t' && !feof((FILE*)m_pFile))
	{
		buf[pos++] = GetChar();
		c = CheckChar();
	}
	buf[pos]='\0';
	
	long length = ftell((FILE*)m_pFile) - orgpos;
	fseek((FILE*)m_pFile, -1 * length, SEEK_CUR);
	m_nLineNum = nLineNum;
	return string(buf);
}

bool Tokenizer::FindToken(const string tok)
{
	SkipWhitespace();

	int pos = 0;
	const char *tokbuf = tok.c_str();
	while(tokbuf[pos] != '\0')
	{
		if (feof((FILE*)m_pFile))
			return false;
		char c = GetChar();
		if (c == tokbuf[pos])
			pos++;
		else
			pos=0;
	}
	return true;
}

bool Tokenizer::SkipWhitespace()
{
	char c = CheckChar();
	bool white = false;
	while (isspace(c))
	{
		GetChar();
		c = CheckChar();
		white = true;
	}
	return white;
}

bool Tokenizer::SkipLine()
{
	char c = GetChar();
	while (c != '\n')
	{
		if (feof((FILE*)m_pFile))
			return false;
		c = GetChar();
	}
	return true;
}

bool Tokenizer::Reset()
{
	// reset to the beginning of the file
	if (fseek((FILE*)m_pFile, 0, SEEK_SET))
		return false;
	return true;
}
