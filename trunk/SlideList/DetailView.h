#if !defined(AFX_DETAILVIEW_H__B0F98C84_DB48_4FB1_AA8C_0072F783DDBC__INCLUDED_)
#define AFX_DETAILVIEW_H__B0F98C84_DB48_4FB1_AA8C_0072F783DDBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailView view

class CDetailView : public CScrollView
{
protected:
	CDetailView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDetailView)

// Attributes
public:
	CSlideListDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDetailView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDetailView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Private helper functions
private:
	int DrawFilmDetails(CDC* pDC, CFilmDetails *pFilm );
	int DrawPhotoDetails(CDC* pDC, CPhotoDetails *pPhoto );
    int DrawFilmPhotoList(CDC* pDC, CFilmDetails *pFilm );
	int OutputText( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos = 0 );
	int OutputParagraph( CDC* pDC, LPCTSTR pLocalFontName, LPCTSTR pText, int XPos = 0, int MaxWidth = 0 );
	void NewLine ( LPCTSTR pLocalFontName = NULL );
	BOOL AddFont( CDC *pDC, LPCTSTR pLocalName, LPCTSTR pFaceName, int PointSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE );
	CFont *GetFont( LPCTSTR pLocalName );
	TEXTMETRIC *GetFontInfo( LPCTSTR pLocalName );
	int FontsInCache( void ) { return(m_FontCache.GetCount()); };
	void DeleteFontCache();

// private attributes
private:
	CMapStringToOb	m_FontCache;		// Cache of fonts
	CMapStringToPtr m_FontInfoCache;	// Cache of LOGFONTS

	int				m_TopMargin;
	int				m_BottomMargin;
	int				m_LeftMargin;
	int				m_RightMargin;

	long int		m_CurrentYPos;
	int				m_CurrentRowHeight;
};

#ifndef _DEBUG  // debug version in DetailView.cpp
inline CSlideListDoc* CDetailView::GetDocument()
   { return (CSlideListDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILVIEW_H__B0F98C84_DB48_4FB1_AA8C_0072F783DDBC__INCLUDED_)
