// DataFile.h: interface for the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILE_H__FDD0E2C0_CE30_434B_B597_051F8F578A13__INCLUDED_)
#define AFX_DATAFILE_H__FDD0E2C0_CE30_434B_B597_051F8F578A13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataFile : public CStdioFile  
{

// Construction/Destruction
public:
	CDataFile();
	virtual ~CDataFile();

// Public operations
public:
	void StartHashVariable( LPCTSTR pName, LPCTSTR pComment = NULL );
	void EndHashVariable( LPCTSTR pComment = NULL );

	void StartHash( LPCTSTR pKeyName, LPCTSTR pComment = NULL );
	void EndHash(  LPCTSTR pComment = NULL );

	void AddAttribute( LPCTSTR pKeyName, LPCTSTR pValue );
	void AddAttribute( LPCTSTR pKeyName, LONG lValue );
	void AddAttribute( LPCTSTR pKeyName, ULONG ulValue );

// Private helper functions
private:
	void WriteIndent( void );
	void WriteQuotedString( LPCTSTR pString );

// Private attributes
private:
	int		m_TabSize;
	int		m_IndentationLevel;
};

#endif // !defined(AFX_DATAFILE_H__FDD0E2C0_CE30_434B_B597_051F8F578A13__INCLUDED_)
