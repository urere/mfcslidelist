// DataFile.cpp: implementation of the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SlideList.h"
#include "DataFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataFile::CDataFile()
{
	m_IndentationLevel = 0;
	m_TabSize = 4;
}

CDataFile::~CDataFile()
{

}

//////////////////////////////////////////////////////////////////////
// Public operations
//////////////////////////////////////////////////////////////////////

void CDataFile::StartHashVariable( LPCTSTR pName, LPCTSTR pComment /* = NULL */ )
{
	// Indentation level should be zero before we start
	ASSERT(m_IndentationLevel == 0);

	// Name must be provided
	ASSERT(pName);

	// A blank line to start
	// then Comment if present
	if ( pComment )
	{
		CString csCommentLine = "\n# ";
		csCommentLine += pComment;
		WriteString( csCommentLine );
	}
	WriteString( "\n" );

	CString csCodeLine = "%";
	csCodeLine += pName;
	WriteString( csCodeLine );

	WriteString( " = (\n" );

	// Set indendation level, the named variable always starts at 1
	m_IndentationLevel = 1;
}

void CDataFile::EndHashVariable( LPCTSTR pComment /* = NULL */ )
{
	// Must be at indentation level 1 to end
	ASSERT(m_IndentationLevel == 1);

	// Close the hash
	WriteString( ");\n" );

	// then Comment if present
	if ( pComment )
	{
		CString csCommentLine = "\n# ";
		csCommentLine += pComment;
		WriteString( csCommentLine );
		WriteString( "\n" );
	}

	// Reset the indentation level
	m_IndentationLevel = 0;
}


void CDataFile::StartHash( LPCTSTR pKeyName, LPCTSTR pComment /* = NULL */ )
{
	// Indentation level must be 1 or greater
	ASSERT(m_IndentationLevel >= 1);

	// Name must be supplied
	ASSERT(pKeyName);

	// Write out the comment if supplied
	if ( pComment )
	{
		CString csCommentLine = "# ";
		csCommentLine += pComment;
		WriteIndent();
		WriteString( csCommentLine );
		WriteString( "\n" );
	}

	// Write out the start of the hash
	WriteIndent();
	WriteQuotedString( pKeyName );
	WriteString( " => {\n" );

	// Increment the indent level
	m_IndentationLevel += 1;
}

void CDataFile::EndHash(  LPCTSTR pComment /* = NULL */ )
{
	// Indentation level must be 2 or greater
	ASSERT(m_IndentationLevel >= 2);

	// Decrement the indent level
	m_IndentationLevel -= 1;

	// Close the hash
	WriteIndent();
	WriteString( "},\n" );
}


void CDataFile::AddAttribute( LPCTSTR pKeyName, LPCTSTR pValue )
{
	// key and value must be valid
	ASSERT(pKeyName);
	ASSERT(pValue);

	// Indentation level must be 1 or greater
	ASSERT(m_IndentationLevel >= 1);

	// Start with the indent
	WriteIndent();
	WriteQuotedString( pKeyName );
	WriteString( " => " );
	WriteQuotedString( pValue );
	WriteString( ",\n" );
}

void CDataFile::AddAttribute( LPCTSTR pKeyName, LONG lValue )
{
	// key and value must be valid
	ASSERT(pKeyName);

	// Indentation level must be 1 or greater
	ASSERT(m_IndentationLevel >= 1);

	CString csValue;
	csValue.Format( "%ld", lValue );

	// Start with the indent
	WriteIndent();
	WriteQuotedString( pKeyName );
	WriteString( " => " );
	WriteQuotedString( csValue );
	WriteString( ",\n" );
}

void CDataFile::AddAttribute( LPCTSTR pKeyName, ULONG ulValue )
{
	// key and value must be valid
	ASSERT(pKeyName);

	// Indentation level must be 1 or greater
	ASSERT(m_IndentationLevel >= 1);

	CString csValue;
	csValue.Format( "%lu", ulValue );

	// Start with the indent
	WriteIndent();
	WriteQuotedString( pKeyName );
	WriteString( " => " );
	WriteQuotedString( csValue );
	WriteString( ",\n" );
}


void CDataFile::WriteIndent( void )
{
	// Output the number of spaces that correspond to the indent level and tab size
	if ( m_IndentationLevel > 0 )
	{
		CString csIndent( ' ', m_IndentationLevel * m_TabSize );
		WriteString( csIndent );
	}
}

void CDataFile::WriteQuotedString( LPCTSTR pString )
{
	ASSERT( pString );

	WriteString( "\"" );
	WriteString( pString );
	WriteString( "\"" );
}
