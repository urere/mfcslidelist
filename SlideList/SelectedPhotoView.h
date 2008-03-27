#if !defined(AFX_SELECTEDPHOTOVIEW_H__EF060EED_A02B_4E89_9E6B_77A7DEE3A169__INCLUDED_)
#define AFX_SELECTEDPHOTOVIEW_H__EF060EED_A02B_4E89_9E6B_77A7DEE3A169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectedPhotoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectedPhotoView view

class CSelectedPhotoView : public CScrollView
{
protected:
	CSelectedPhotoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSelectedPhotoView)

// Attributes
public:
	CSlideListDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectedPhotoView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSelectedPhotoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSelectedPhotoView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Private helper functions
private:
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

	CStringArray	m_PhotoIdArray;		// Array containing id's of selected photos

	int				m_TopMargin;
	int				m_BottomMargin;
	int				m_LeftMargin;
	int				m_RightMargin;

	long int		m_CurrentYPos;
	int				m_CurrentRowHeight;

    long int        m_FirstPhotoPosition;
    long int        m_LastPhotoPosition;
    long int        m_PhotoHeight;
};

#ifndef _DEBUG  // debug version in DetailView.cpp
inline CSlideListDoc* CSelectedPhotoView::GetDocument()
   { return (CSlideListDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTEDPHOTOVIEW_H__EF060EED_A02B_4E89_9E6B_77A7DEE3A169__INCLUDED_)
