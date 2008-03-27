#if !defined(AFX_PHOTODETAILSDIALOG_H__F2B98302_ECD9_11D5_9350_0080C88F400B__INCLUDED_)
#define AFX_PHOTODETAILSDIALOG_H__F2B98302_ECD9_11D5_9350_0080C88F400B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhotoDetailsDialog.h : header file
//

// Document class
class CSlideListDoc;

/////////////////////////////////////////////////////////////////////////////
// CPhotoDetailsDialog dialog

class CPhotoDetailsDialog : public CDialog
{
// Construction
public:
	CPhotoDetailsDialog( CSlideListDoc *pDocument, BOOL bAddMode, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhotoDetailsDialog)
	enum { IDD = IDD_PHOTO_DETAILS };
	CListBox	m_KeywordsList;
	CString	m_csDescription;
	CString	m_csNotes;
	CString	m_csKeywords;
	CString	m_csLocation;
	DWORD	m_Number;
	int		m_Rating;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoDetailsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhotoDetailsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkKeywordList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// Private attributes
private:
    CSlideListDoc  *m_pDocument;
    BOOL            m_bAddMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTODETAILSDIALOG_H__F2B98302_ECD9_11D5_9350_0080C88F400B__INCLUDED_)
