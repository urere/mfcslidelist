// SelectedPhotoView.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "SlideListDoc.h"
#include "SlideListView.h"
#include "SelectedPhotoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectedPhotoView

IMPLEMENT_DYNCREATE(CSelectedPhotoView, CScrollView)

CSelectedPhotoView::CSelectedPhotoView()
{
	m_TopMargin = 10;
	m_BottomMargin = 10;
	m_LeftMargin = 10;
	m_RightMargin = 10;

	m_CurrentYPos = 0;
	m_CurrentRowHeight = 0;

    m_FirstPhotoPosition = 0;
    m_LastPhotoPosition = 0;
    m_PhotoHeight = 0;
}

CSelectedPhotoView::~CSelectedPhotoView()
{
    DeleteFontCache();
}


BEGIN_MESSAGE_MAP(CSelectedPhotoView, CScrollView)
	//{{AFX_MSG_MAP(CSelectedPhotoView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
CSlideListDoc* CSelectedPhotoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSlideListDoc)));
	return (CSlideListDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSelectedPhotoView drawing

void CSelectedPhotoView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSelectedPhotoView::OnDraw(CDC* pDC)
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

    unsigned int SelectedPhotoCount = pDoc->GetSelectedPhotoCount(); 

	MaxWidth = OutputText( pDC, "Main Title", "Selected Photographs" );
	NewLine();
    pDC->SetBkColor( CurrentBk );
    m_CurrentYPos += 6;

    // Clear location of first photo
    m_FirstPhotoPosition = 0;
    m_LastPhotoPosition = 0;
    m_PhotoHeight = 0;

	// Clear the array of selected photos and set its new size
	m_PhotoIdArray.RemoveAll();
	m_PhotoIdArray.SetSize( SelectedPhotoCount );

    // Scan through all of the films and photos looking for selected photos
	// Dont, bother if there arn't any selected photos
	if ( SelectedPhotoCount > 0 )
	{
		POSITION FilmPos;
		CFilmDetails *pFilm;
		POSITION PhotoPos;
		CPhotoDetails *pPhoto;
		int SummaryWidth;

        // Remember position of first selected photo and height of photo line
        // This us used when the mousr button is pressed
        m_FirstPhotoPosition = m_CurrentYPos;
	    TEXTMETRIC *pFontInfo = GetFontInfo( "Small Details" );
	    ASSERT(pFontInfo);
	    m_PhotoHeight = pFontInfo->tmHeight + pFontInfo->tmExternalLeading;

		FilmPos = pDoc->GetFilmListHead();
		unsigned int PhotoIndex = 0;
		while ( FilmPos )
		{
			pFilm = pDoc->GetNextFilm( FilmPos );
			if ( pFilm )
			{
				// Scan through the photos in this film
				PhotoPos = pFilm->GetPhotoListHead();
				while ( PhotoPos )
				{
					pPhoto = pFilm->GetNextPhoto( PhotoPos );
					if ( pPhoto )
					{
						// Is this photo selected ?
						if ( pPhoto->IsSelected() )
						{
							SummaryWidth = OutputText( pDC, "Small Details", pPhoto->GetPhotoSummary() );
							NewLine();
							if ( SummaryWidth > MaxWidth )
							{
								MaxWidth = SummaryWidth;
							}

							// Store the ID of this photo
							m_PhotoIdArray[PhotoIndex] = pPhoto->GetPhotoReferenceNumber();
							PhotoIndex += 1;
						}
					}
				}
			}
		}

        m_LastPhotoPosition = m_CurrentYPos;
	}

	// Set the scroll information based on the height and width
	CSize ScrollSize;

	ScrollSize.cx = m_LeftMargin + MaxWidth + m_RightMargin;
	ScrollSize.cy = m_CurrentYPos + m_BottomMargin;
	SetScrollSizes( MM_TEXT, ScrollSize );

}


int CSelectedPhotoView::OutputText( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos /* = 0 */ )
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

int CSelectedPhotoView::OutputParagraph( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos /* = 0 */, int MaxWidth /* = 0 */  )
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

void CSelectedPhotoView::NewLine ( LPCTSTR pLocalFontName /* = NULL */ )
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
// CSelectedPhotoView diagnostics

#ifdef _DEBUG
void CSelectedPhotoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSelectedPhotoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSelectedPhotoView message handlers


/////////////////////////////////////////////////////////////////////////////
// CSelectedPhotoView FONT support

BOOL CSelectedPhotoView::AddFont( CDC *pDC, LPCTSTR pLocalName, LPCTSTR pFaceName, int PointSize, BOOL bBold /* = FALSE */, BOOL bItalic /* = FALSE */, BOOL bUnderLine /* = FALSE */ )
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


CFont *CSelectedPhotoView::GetFont( LPCTSTR pLocalName )
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

TEXTMETRIC *CSelectedPhotoView::GetFontInfo( LPCTSTR pLocalName )
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

void CSelectedPhotoView::DeleteFontCache()
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

void CSelectedPhotoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    // Perform the update only unless its only a tree view selection change
    if ( lHint != CSlideListDoc::TREE_SELECTION_CHANGE )
    {
        CScrollView::OnUpdate( pSender, lHint, pHint );
    }
}

void CSelectedPhotoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    // Work which selected photo as been clicked on

    if ( m_LastPhotoPosition > m_FirstPhotoPosition )
    {
        // Create a client DC to get the coordinates correct for the scroll view
        CClientDC Dc( this );
        OnPrepareDC( &Dc );
        Dc.DPtoLP( &point );

        // Mouse clicked within photo area ?
        if ( (point.y >= m_FirstPhotoPosition) && (point.y <= m_LastPhotoPosition) )
        {
            int PhotoIndex = (point.y - m_FirstPhotoPosition) / m_PhotoHeight;

			// Using the index we can determine the ID of the photo
			ASSERT(PhotoIndex <= m_PhotoIdArray.GetUpperBound());

			CSlideListDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			// Need to tell the tree view to select this photo
			POSITION Pos = pDoc->GetFirstViewPosition();
			ASSERT(Pos);

			CSlideListView *pTreeView = (CSlideListView *) pDoc->GetNextView( Pos );
			ASSERT(pTreeView);

			pTreeView->SelectPhotoUsingId( m_PhotoIdArray[PhotoIndex] );

			// Move focus to the tree view
			pTreeView->SetFocus();
        }
    }
}

void CSelectedPhotoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);

    if ( m_LastPhotoPosition > m_FirstPhotoPosition )
    {
        // Create a client DC to get the coordinates correct for the scroll view
        CClientDC Dc( this );
        OnPrepareDC( &Dc );
        Dc.DPtoLP( &point );

        // Mouse clicked within photo area ?
        if ( (point.y >= m_FirstPhotoPosition) && (point.y <= m_LastPhotoPosition) )
        {
		}
	}
}
