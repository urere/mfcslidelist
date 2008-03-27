// DetailView.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "SlideListDoc.h"
#include "DetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailView

IMPLEMENT_DYNCREATE(CDetailView, CScrollView)

CDetailView::CDetailView()
{
	m_TopMargin = 10;
	m_BottomMargin = 10;
	m_LeftMargin = 10;
	m_RightMargin = 10;

	m_CurrentYPos = 0;
	m_CurrentRowHeight = 0;
}

CDetailView::~CDetailView()
{
	DeleteFontCache();
}


BEGIN_MESSAGE_MAP(CDetailView, CScrollView)
	//{{AFX_MSG_MAP(CDetailView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
CSlideListDoc* CDetailView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSlideListDoc)));
	return (CSlideListDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDetailView drawing

void CDetailView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDetailView::OnDraw(CDC* pDC)
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ( FontsInCache() == 0 )
	{
		// Add required fonts
		AddFont( pDC, "Main Title", "Arial", 16, TRUE, FALSE, TRUE );
		AddFont( pDC, "Title", "Arial", 12, TRUE, FALSE, FALSE );
		AddFont( pDC, "Details", "Arial", 12, FALSE, FALSE, FALSE );
		AddFont( pDC, "Small Details", "Arial", 10, FALSE, FALSE, FALSE );
	}

	// Clear the line position details before we start drawing
	m_CurrentYPos = m_TopMargin;
	m_CurrentRowHeight = 0;

	int XPos = 0;
	int MaxWidth = 0;

	// Are there any film details ?
	CFilmDetails *pFilm = pDoc->GetSelectedFilm();
	if ( pFilm )
	{

		MaxWidth = DrawFilmDetails( pDC, pFilm );

        // Draw a blank line and a line across to divide the page up
		NewLine( "Title" );

        CRect ClientRect;
	    GetClientRect( &ClientRect );

        pDC->MoveTo( 0, m_CurrentYPos );
        pDC->LineTo( ClientRect.Width(), m_CurrentYPos );

		// Display photo details if available
		CPhotoDetails *pPhoto = pDoc->GetSelectedPhoto();
		if ( pPhoto )
		{
			int PhotoMaxWidth = 0;

			// Blank line before the photo details
			NewLine( "Title" );

			// Must match the film
			ASSERT(pFilm->GetFilmNumber() == pPhoto->GetFilmNumber());
			ASSERT(pFilm->GetFilmYear() == pPhoto->GetFilmYear());

			// Output the photo details
			PhotoMaxWidth = DrawPhotoDetails( pDC, pPhoto );

			if ( PhotoMaxWidth > MaxWidth )
			{
				MaxWidth = PhotoMaxWidth;
			}
		}
        else
        {
			int PhotoListMaxWidth = 0;

			// Blank line before the photo details
			NewLine( "Title" );

            // Otherwise just draw a list of photos
            DrawFilmPhotoList( pDC, pFilm );

			if ( PhotoListMaxWidth > MaxWidth )
			{
				MaxWidth = PhotoListMaxWidth;
			}
        }

		// Set the scroll information based on the height and width
		CSize ScrollSize;

		ScrollSize.cx = m_LeftMargin + MaxWidth + m_RightMargin;
		ScrollSize.cy = m_CurrentYPos + m_BottomMargin;
		SetScrollSizes( MM_TEXT, ScrollSize );

	}
}

int CDetailView::DrawFilmDetails(CDC* pDC, CFilmDetails *pFilm )
{
	// Output film details, returns the maximum width used, This is based on the length
	// of the description

	int CurrentWidth = 0;
	int MaxWidth = 0;
	int ValueXPos = 0;
	int Column2LabelPos = 0;
	int Column2ValuePos = 0;
	CString csFormatted;

	CRect ClientRect;
	GetClientRect( &ClientRect );

    // Draw a shaded background for the title
 	TEXTMETRIC *pFontInfo = GetFontInfo( "Main Title" );
	ASSERT(pFontInfo);
	int TextHeight = pFontInfo->tmHeight + pFontInfo->tmExternalLeading;
    
    CRect ShadedRect = ClientRect;
    GetFontInfo( "Main Title" );
    ShadedRect.bottom = TextHeight + m_TopMargin;
    ShadedRect.left = GetDeviceScrollPosition().x;
    ShadedRect.right = ShadedRect.left + max(GetTotalSize().cx, ClientRect.right);
    COLORREF CurrentBk = pDC->GetBkColor();
    pDC->FillSolidRect( &ShadedRect, 0x00c0c0c0 );
    ShadedRect.top = ShadedRect.bottom;
    ShadedRect.bottom += 4;
    pDC->FillSolidRect( &ShadedRect, 0x00808080 );
    pDC->SetBkColor( 0x00c0c0c0 );

	// Draw the film ID
	OutputText( pDC, "Main Title", pFilm->GetFilmReferenceNumber() );
	NewLine();
    pDC->SetBkColor( CurrentBk );
    m_CurrentYPos += 6;

	// Description, this sets the width for everything else
	ValueXPos = OutputText( pDC, "Title", "Description: " );
	MaxWidth = OutputText( pDC, "Details", pFilm->GetDescription(), ValueXPos );
	NewLine();

	// if the returned position is less than the returned description width use the client
	if ( MaxWidth < ClientRect.Width() )
	{
		MaxWidth = ClientRect.Width();
	}

	// Output the film year and number
	csFormatted.Format( "%u", pFilm->GetFilmNumber() );
	OutputText( pDC, "Title", "Number: " );
	OutputText( pDC, "Details", csFormatted, ValueXPos );

	CurrentWidth = 3 * ValueXPos;
	Column2LabelPos = CurrentWidth;
	csFormatted.Format( "20%2.2u", pFilm->GetFilmYear() );
	CurrentWidth = OutputText( pDC, "Title", "Year: ", Column2LabelPos );
	Column2ValuePos = CurrentWidth+30;
	CurrentWidth = OutputText( pDC, "Details", csFormatted, Column2ValuePos );
	NewLine();
	if ( CurrentWidth > MaxWidth )
	{
		MaxWidth = CurrentWidth;
	}

	// Output the type and make
	CString csType;
	switch ( pFilm->GetType() )
	{
		case CFilmDetails::FILM_COLOUR_PRINT:
			csType = "Colour Print";
			break;
		case CFilmDetails::FILM_COLOUR_SLIDE:
			csType = "Colour Slide";
			break;
		case CFilmDetails::FILM_BW_PRINT:
			csType = "BW Print";
			break;
		case CFilmDetails::FILM_BW_SLIDE:
			csType = "BW Slide";
			break;
		case CFilmDetails::FILM_OTHER:
		default:
			csType = "Other";
			break;
	}

	OutputText( pDC, "Title", "Type: " );
	OutputText( pDC, "Details", csType, ValueXPos );

	OutputText( pDC, "Title", "Make: ", Column2LabelPos );
	CurrentWidth = OutputText( pDC, "Details", pFilm->GetFilmMake(), Column2ValuePos );
	NewLine();
	if ( CurrentWidth > MaxWidth )
	{
		MaxWidth = CurrentWidth;
	}

	// Output any notes
	OutputText( pDC, "Title", "Notes: " );
	OutputParagraph( pDC, "Details", pFilm->GetNotes(), ValueXPos, MaxWidth-m_RightMargin );
	NewLine();

	return ( MaxWidth );
}

int CDetailView::DrawFilmPhotoList(CDC* pDC, CFilmDetails *pFilm )
{
	// Output film photo list, returns the maximum width used, This is based on the length
	// of the description

	int CurrentWidth = 0;
	int MaxWidth = 0;
	int ValueXPos = 0;
    POSITION Pos;
    CPhotoDetails *pPhoto;

    Pos = pFilm->GetPhotoListHead();
    while ( Pos )
    {
        pPhoto = pFilm->GetNextPhoto( Pos );
        ASSERT(pPhoto);

        CurrentWidth = OutputText( pDC, "Small Details", pPhoto->GetPhotoSummary() );
        NewLine();
        if ( CurrentWidth > MaxWidth )
        {
            MaxWidth = CurrentWidth;
        }
    }

	return ( MaxWidth );
}

int CDetailView::DrawPhotoDetails(CDC* pDC, CPhotoDetails *pPhoto )
{
	// Output film details, returns the maximum width used, This is based on the length
	// of the description

	int CurrentWidth = 0;
	int MaxWidth = 0;
	int ValueXPos = 0;
	int Column2LabelPos = 0;
	int Column2ValuePos = 0;
	CString csFormatted;

	// Draw the Photo ID
	OutputText( pDC, "Main Title", pPhoto->GetPhotoReferenceNumber() );
	NewLine();

	// Description, this sets the width for everything else
	ValueXPos = OutputText( pDC, "Title", "Description: " );
	MaxWidth = OutputText( pDC, "Details", pPhoto->GetDescription(), ValueXPos );
	NewLine();

	// Location,
	OutputText( pDC, "Title", "Location: " );
	MaxWidth = OutputText( pDC, "Details", pPhoto->GetLocation(), ValueXPos );
	NewLine();

	// if the returned position is less than the returned description width use the client
	CRect ClientRect;
	GetClientRect( &ClientRect );
	if ( MaxWidth < ClientRect.Width() )
	{
		MaxWidth = ClientRect.Width();
	}

	// Output the photo rating
	csFormatted.Format( "%u", pPhoto->GetRating() );
	OutputText( pDC, "Title", "Rating: " );
	OutputText( pDC, "Details", csFormatted, ValueXPos );
	NewLine();
	if ( CurrentWidth > MaxWidth )
	{
		MaxWidth = CurrentWidth;
	}

	// Output any Keywords
	OutputText( pDC, "Title", "Keywords: " );
	OutputParagraph( pDC, "Details", pPhoto->GetKeywords(), ValueXPos, MaxWidth-m_RightMargin );
	NewLine();

	// Output any notes
	OutputText( pDC, "Title", "Notes: " );
	OutputParagraph( pDC, "Details", pPhoto->GetNotes(), ValueXPos, MaxWidth-m_RightMargin );
	NewLine();

	return ( MaxWidth );
}

int CDetailView::OutputText( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos /* = 0 */ )
{
	// Output a single line of text
	// The YPos is the current YPos
	// Returns the new Xpos relative to the left margin

	int FinalXPos = 0;

	// Change to the requested font
	CFont *pOldFont = pDC->SelectObject( GetFont( pLocalFontName ) );

	pDC->TextOut( m_LeftMargin + XPos, m_CurrentYPos, pText );

	// Calculate the new x position
	FinalXPos = XPos + pDC->GetTextExtent( pText ).cx;

	// Use the font height to set of height for this row IF it is greater than any thing
	// already output at this position
	TEXTMETRIC *pFontInfo = GetFontInfo( pLocalFontName );
	ASSERT(pFontInfo);

	int TextHeight = pFontInfo->tmHeight + pFontInfo->tmExternalLeading;
	if ( TextHeight > m_CurrentRowHeight )
	{
		m_CurrentRowHeight = TextHeight;
	}

	pDC->SelectObject( pOldFont );

	return ( FinalXPos );
}

int CDetailView::OutputParagraph( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos /* = 0 */, int MaxWidth /* = 0 */  )
{
	// Outputs text, possibly spanning multiple lines
	// No line will exceed the width supplied relative to the Xmargin and XPos
	// The YPos is the current YPos
	// Returns the height of the text output

	int FinalXPos = 0;
	CRect ParagraphRect;

	// Change to the requested font
	CFont *pOldFont = pDC->SelectObject( GetFont( pLocalFontName ) );

	// Determine the bounding rectangle
	ParagraphRect.left = m_LeftMargin + XPos;
	if ( MaxWidth )
	{
		// Use supplied width
		ParagraphRect.right = MaxWidth;
	}
	else
	{
		// Use all of remaining space
		CRect ClientRect;
		GetClientRect( &ClientRect );
		ParagraphRect.right = ClientRect.right - m_RightMargin;
	}

	ParagraphRect.top = m_CurrentYPos;
	ParagraphRect.bottom = m_CurrentYPos;

	// Let draw text calculate the height
	int TextHeight = pDC->DrawText( pText, &ParagraphRect, DT_WORDBREAK | DT_LEFT | DT_NOPREFIX | DT_EXTERNALLEADING | DT_CALCRECT );
	ParagraphRect.bottom += TextHeight;
	pDC->DrawText( pText, &ParagraphRect, DT_WORDBREAK | DT_LEFT | DT_NOPREFIX | DT_EXTERNALLEADING );
	if ( TextHeight > m_CurrentRowHeight )
	{
		m_CurrentRowHeight = TextHeight;
	}

	// Use the font height to set of height for this row IF it is greater than any thing
	// already output at this position
	TEXTMETRIC *pFontInfo = GetFontInfo( pLocalFontName );
	ASSERT(pFontInfo);


	pDC->SelectObject( pOldFont );

	return ( TextHeight );
}

void CDetailView::NewLine ( LPCTSTR pLocalFontName /* = NULL */ )
{
	// Use the height of the supplied font name if supplied, other wise the current row height
	if ( pLocalFontName )
	{
		TEXTMETRIC *pFontInfo = GetFontInfo( pLocalFontName );
		ASSERT(pFontInfo);

		int TextHeight = pFontInfo->tmHeight + pFontInfo->tmExternalLeading;
		if ( TextHeight > m_CurrentRowHeight )
		{
			m_CurrentRowHeight = TextHeight;
		}
	}

	m_CurrentYPos += m_CurrentRowHeight;
	m_CurrentRowHeight = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDetailView diagnostics

#ifdef _DEBUG
void CDetailView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDetailView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDetailView message handlers


/////////////////////////////////////////////////////////////////////////////
// CDetailView FONT support

BOOL CDetailView::AddFont( CDC *pDC, LPCTSTR pLocalName, LPCTSTR pFaceName, int PointSize, BOOL bBold /* = FALSE */, BOOL bItalic /* = FALSE */, BOOL bUnderLine /* = FALSE */ )
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


CFont *CDetailView::GetFont( LPCTSTR pLocalName )
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

TEXTMETRIC *CDetailView::GetFontInfo( LPCTSTR pLocalName )
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

void CDetailView::DeleteFontCache()
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
