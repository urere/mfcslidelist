// PrintingSupport.cpp: implementation of the CPrintingSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SlideList.h"
#include "PrintingSupport.h"

#include "SlideListDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FORMFEED				0x0c
#define REPORT_CONFIG_START		"<START_REPORT_CONFIG>"
#define REPORT_CONFIG_END		"<END_REPORT_CONFIG>"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrintingSupport::CPrintingSupport( CSlideListDoc *pDocument, LPCTSTR pReportFileName )
{
	m_pDocument = pDocument;

	ASSERT(m_pDocument);

	// Store the name of the file containing the report to be printed
	m_csReportFileName = pReportFileName;

	// Set the size of the cache array to twice the number of films
	// If this is exceeded grow the array 10 entries at a time
	m_PageFilePosition.SetSize( 0, 1 );

	// Ensure all of the cached pages are 0xffffffff
	int NumPages = m_PageFilePosition.GetSize();
	
	for ( int Page = 0; Page < NumPages; Page++ )
	{
		m_PageFilePosition[Page] = 0xffffffff;
	}

	// Default page layout
	m_pDC = NULL;
	m_pInfo = NULL;
	m_CurrentYPos = 0;
	m_TopMargin = 0;
	m_BottomMargin = 0;
	m_LeftMargin = 0;
	m_RightMargin = 0;

	m_CurrentRowHeight = 0;

}

CPrintingSupport::~CPrintingSupport()
{
	// Delete all of the fonts from the cache
	DeleteFontCache();
}

BOOL CPrintingSupport::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Scan through the report file and count the number of pages
	// Also record the position of each page so that it can be
	// quickly located later
	// Configuration information delimited by 
	//		<START_REPORT_CONFIG> and
	//		<END_REPORT_CONFIG>
	// may appear at the start of the file. These delimiters will
	// be ignored after report data as been read

	BOOL bFileScanned = FALSE;

	CStdioFile ReportFile;

	if ( ReportFile.Open( m_csReportFileName, CFile::modeRead | CFile::typeText ) )
	{
		CString csLine;
		DWORD FilePosition = 0;
		DWORD CurrentPage = 0;
		BOOL bReadingConfig = FALSE;
		BOOL bNewPage = TRUE;

		while ( ReportFile.ReadString( csLine ) )
		{
			// Are we reading configuration information at the moment
			if ( bReadingConfig )
			{
				// Process this line as configuration information
				if ( csLine == REPORT_CONFIG_END )
				{
					bReadingConfig = FALSE;
				}
			}
			else
			{
				// Start of config ?
				if ( (CurrentPage == 0) && (csLine == REPORT_CONFIG_START) )
				{
					bReadingConfig = TRUE;
				}
				else
				{
					// This is a report data line

					// First line of new page page ?
					if ( bNewPage )
					{
						CurrentPage += 1;
						m_PageFilePosition.SetAtGrow( CurrentPage-1, FilePosition );
						bNewPage = FALSE;
					}
					else
					{
						// Start of a new page ?
						if ( (csLine.GetLength() > 0) && (csLine[0] == FORMFEED) )
						{
							// Rest of this line is one a new page
							if ( csLine.GetLength() > 1 )
							{
								CurrentPage += 1;
								// Add 1 to the file position to skip over the FF
								m_PageFilePosition.SetAtGrow( CurrentPage-1, FilePosition+1 );
							}
							else
							{
								// Next line starts the new page
								bNewPage = TRUE;
							}
						}
					}
				}
			}

			// Remember the current file position
			FilePosition = ReportFile.GetPosition();
		}

		ReportFile.Close();

		// Store the number of pages in the report
		pInfo->SetMinPage( 1 );
		pInfo->SetMaxPage( CurrentPage );

		bFileScanned = TRUE;
	}

	return ( bFileScanned );
}

void CPrintingSupport::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pDC->SetMapMode( MM_TWIPS );

	AddFont( pDC, "DEFAULT", "Courier New", 12, FALSE, FALSE, FALSE );
}

void CPrintingSupport::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SetMapMode( MM_TWIPS );
}				

void CPrintingSupport::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// Print the requested page

	// Does this page already exist in the page case ?
	int PageCacheIndex = (int) pInfo->m_nCurPage - 1;

	// Is there room in the current page cache ?
	if ( PageCacheIndex <= m_PageFilePosition.GetUpperBound() )
	{
		// Print the page starting at this file position
		CStdioFile ReportFile;

		if ( ReportFile.Open( m_csReportFileName, CFile::modeRead | CFile::typeText ) )
		{
			CString csLine;

			// Set file position to the start of the requested page
			ReportFile.Seek( m_PageFilePosition[PageCacheIndex], CFile::begin );

			// Output the page
			StartPage( pDC, pInfo );

			while ( ReportFile.ReadString( csLine ) )
			{
				// End of page yet ?
				if ( csLine.GetLength() > 0 )
				{
					if ( csLine[0] != FORMFEED )
					{
						// Output this line
						OutputText( "DEFAULT", csLine );
						NextRow();
					}
					else
					{
						// End of page reached
						break;
					}
				}
				else
				{
					// Blank line
					OutputText( "DEFAULT", " " );
					NextRow();
				}
			}

			ReportFile.Close();
		}
	}
}

void CPrintingSupport::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// Delete all of the fonts from the cache
	DeleteFontCache();
}


BOOL CPrintingSupport::AddFont( CDC *pDC, LPCTSTR pLocalName, LPCTSTR pFaceName, int PointSize, BOOL bBold /* = FALSE */, BOOL bItalic /* = FALSE */, BOOL bUnderLine /* = FALSE */ )
{
	// Creates a font matching the requested specification and stores in the cache using the local name
	// supplied

	BOOL bCreated = FALSE;

	// If it already exists, assume its already been created
	if ( GetFont( pLocalName ) == NULL )
	{

		LOGFONT PointFont;
		
		// Common font attributes
		memset( &PointFont, 0, sizeof(PointFont) );
		PointFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
		PointFont.lfQuality = PROOF_QUALITY;
		PointFont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;

		// Point size
		PointFont.lfHeight = 10 * PointSize;

		// Bold, italic, underline
		if ( bBold )
			PointFont.lfWeight = FW_BOLD;
		else
			PointFont.lfWeight = FW_NORMAL;
		PointFont.lfItalic = bItalic;
		PointFont.lfUnderline = bUnderLine;

		// Face name
		strcpy( PointFont.lfFaceName, pFaceName );

		// Create it
		CFont *pNewFont = new CFont();
		if ( pNewFont )
		{
			if ( pNewFont->CreatePointFontIndirect( &PointFont, pDC ) )
			{
				// Add it to the cache
				m_FontCache.SetAt( pLocalName, (CObject *) pNewFont );

				// Add a TEXTMETICS structure to the cache
				// First need to select the font
				CFont *pOldFont =  pDC->SelectObject( pNewFont );

				// Then store the info
				TEXTMETRIC *pFontInfo = new TEXTMETRIC;
				pDC->GetTextMetrics( pFontInfo );
				m_FontInfoCache.SetAt( pLocalName, (void *) pFontInfo );

				pDC->SelectObject( pOldFont );

				bCreated = TRUE;
			}
			else
			{
				delete pNewFont;
			}
		}
	}
	else
	{
		// It already exists
		bCreated = TRUE;
	}

	return ( bCreated );
}


CFont *CPrintingSupport::GetFont( LPCTSTR pLocalName )
{
	// Return the requested font, NULL if not found
	CFont *pFont = NULL;

	if ( !m_FontCache.Lookup( pLocalName, (CObject *&) pFont ) )
	{
		// Failed to find the font
		pFont = NULL;
	}

	return( pFont );
}

TEXTMETRIC *CPrintingSupport::GetFontInfo( LPCTSTR pLocalName )
{
	// Return the requested TEXTMETRIC, NULL if not found
	TEXTMETRIC *pFontInfo = NULL;

	if ( !m_FontInfoCache.Lookup( pLocalName, (void *&) pFontInfo ) )
	{
		// Failed to find the font
		pFontInfo = NULL;
	}

	return( pFontInfo );
}

void CPrintingSupport::DeleteFontCache()
{
	// Delete all of the fonts in the cache
	POSITION Pos;

	// The fonts
	Pos = m_FontCache.GetStartPosition();
	while ( Pos )
	{
		CString csLocalName;
		CFont *pFont = NULL;

		// Get the next font
		m_FontCache.GetNextAssoc( Pos, csLocalName, (CObject *&) pFont );

		ASSERT(pFont);
		delete pFont;
	}

	// Remove all of the keys
	m_FontCache.RemoveAll();

	// The TEXTMETRIC
	Pos = m_FontInfoCache.GetStartPosition();
	while ( Pos )
	{
		CString csLocalName;
		TEXTMETRIC *pFontInfo = NULL;

		// Get the next font
		m_FontInfoCache.GetNextAssoc( Pos, csLocalName, (void *&) pFontInfo );

		ASSERT(pFontInfo);
		delete (TEXTMETRIC *) pFontInfo;
	}

	// Remove all of the keys
	m_FontInfoCache.RemoveAll();
}

// Page Layout function
void CPrintingSupport::DefinePageLayout( int TopMargin, int BottomMargin, int LeftMargin, int RightMargin )
{
	m_TopMargin = TopMargin;
	m_BottomMargin = BottomMargin;
	m_LeftMargin = LeftMargin;
	m_RightMargin = RightMargin;
}

void CPrintingSupport::StartPage( CDC *pDC, CPrintInfo* pInfo )
{
	// This signals the start of a new page, clear all of the structures used to track
	// current position on the page

	m_CurrentYPos = 0;
	m_CurrentYPos -= m_TopMargin;

	m_CurrentRowHeight = 0;

	// Store the DC and printer information for later use
	m_pDC = pDC;
	m_pInfo = pInfo;

}

BOOL CPrintingSupport::OutputText( LPCTSTR pLocalFontName, LPCTSTR pText )
{
	// Output a single line of text
	// The YPos is the current YPos

	BOOL bOutput = FALSE;

	ASSERT(m_pDC);
	ASSERT(m_pInfo);
	
	// Change to the requested font
	CFont *pOldFont = m_pDC->SelectObject( GetFont( pLocalFontName ) );

	m_pDC->TextOut( m_LeftMargin, m_CurrentYPos, pText );

	// Use the font height to set of height for this row IF it is greater than any thing
	// already output at this position
	TEXTMETRIC *pFontInfo = GetFontInfo( pLocalFontName );
	ASSERT(pFontInfo);

	int TextHeight = pFontInfo->tmHeight + pFontInfo->tmExternalLeading;
	if ( TextHeight > m_CurrentRowHeight )
	{
		m_CurrentRowHeight = TextHeight;
	}

	m_pDC->SelectObject( pOldFont );

	return ( bOutput );
}

BOOL CPrintingSupport::DrawLine( int Width, int XStart /* = -1 */, int XStop /* = -1 */ )
{
	BOOL bOutput = FALSE;

	ASSERT(m_pDC);
	ASSERT(m_pInfo);

	// Complete current Row
	if ( m_CurrentRowHeight > 0 )
	{
		NextRow();
	}

	m_CurrentYPos -= 10+Width;


	m_pDC->MoveTo( m_LeftMargin, m_CurrentYPos );
	m_pDC->LineTo( m_pInfo->m_rectDraw.Width() - m_RightMargin, m_CurrentYPos );

	// set the new Y position down by twice the width of the line
	m_CurrentYPos -= 10+Width;

	return( bOutput );
}

void CPrintingSupport::NextRow()
{
	// Move the Y position down by the current row height and reset this to zero
	m_CurrentYPos -= m_CurrentRowHeight;

	m_CurrentRowHeight = 0;
}