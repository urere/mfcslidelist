// PrintingSupport.h: interface for the CPrintingSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTINGSUPPORT_H__483FD2A3_5E20_4B2A_A1AB_EFF7ED24C1A5__INCLUDED_)
#define AFX_PRINTINGSUPPORT_H__483FD2A3_5E20_4B2A_A1AB_EFF7ED24C1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSlideListDoc;

class CPrintingSupport : public CObject 
{
public:
	CPrintingSupport( CSlideListDoc *pDocument, LPCTSTR pReportFileName );
	virtual ~CPrintingSupport();

// Functions to be overridden in derived classes
public:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	// Font factory functions
	BOOL AddFont( CDC *pDC, LPCTSTR pLocalName, LPCTSTR pFaceName, int PointSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE );
	CFont *GetFont( LPCTSTR pLocalName );
	TEXTMETRIC *GetFontInfo( LPCTSTR pLocalName );

	// Page printing support
	void DefinePageLayout( int TopMargin, int BottomMargin, int LeftMargin, int RightMargin );
	void StartPage( CDC *pDC, CPrintInfo* pInfo );
	BOOL OutputText( LPCTSTR pLocalFontName, LPCTSTR pText );
	BOOL DrawLine( int Width, int XStart = -1, int XStop = -1 );
	void NextRow();

// Utility functions used able in derived classes
protected:
	void DeleteFontCache();

protected:
	CString			m_csReportFileName;	// Full name of file containing report to be printed

	CSlideListDoc  *m_pDocument;		// Access to document data
	CDWordArray		m_PageFilePosition;	// Page Position Cache
	CMapStringToOb	m_FontCache;		// Cache of fonts
	CMapStringToPtr m_FontInfoCache;	// Cache of LOGFONTS

										// Page layout details
	CDC			   *m_pDC;
	CPrintInfo	   *m_pInfo;
	long int		m_CurrentYPos;
	int				m_CurrentRowHeight;
	int				m_TopMargin;
	int				m_BottomMargin;
	int				m_LeftMargin;
	int				m_RightMargin;

};

#endif // !defined(AFX_PRINTINGSUPPORT_H__483FD2A3_5E20_4B2A_A1AB_EFF7ED24C1A5__INCLUDED_)
