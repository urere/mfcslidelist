#if !defined(AFX_PHOTOSEARCHDIALOG_H__A3542088_E607_4D31_B3B4_3E5E0D10102F__INCLUDED_)
#define AFX_PHOTOSEARCHDIALOG_H__A3542088_E607_4D31_B3B4_3E5E0D10102F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhotoSearchDialog.h : header file
//

class CSlideListDoc;

/////////////////////////////////////////////////////////////////////////////
// CPhotoSearchDialog dialog

class CPhotoSearchDialog : public CDialog
{
// Construction
public:
	CPhotoSearchDialog(CSlideListDoc *pDocument, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhotoSearchDialog)
	enum { IDD = IDD_SEARCH_DIALOG };
	CListBox	m_KeywordList;
	BOOL	m_bDescription;
	BOOL	m_bKeyword;
	BOOL	m_bLocation;
	CString	m_SearchString;
	int		m_SelectionMode;
	int		m_SearchRating;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoSearchDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhotoSearchDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkSearchKeywordList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CSlideListDoc  *m_pDocument;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOSEARCHDIALOG_H__A3542088_E607_4D31_B3B4_3E5E0D10102F__INCLUDED_)
