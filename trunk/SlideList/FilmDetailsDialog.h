#if !defined(AFX_FILMDETAILSDIALOG_H__BEE3F4E6_765C_4E4B_8230_4629B2135774__INCLUDED_)
#define AFX_FILMDETAILSDIALOG_H__BEE3F4E6_765C_4E4B_8230_4629B2135774__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilmDetailsDialog.h : header file
//

// Document class needed for this document
class CSlideListDoc;

/////////////////////////////////////////////////////////////////////////////
// CFilmDetailsDialog dialog

class CFilmDetailsDialog : public CDialog
{
// Construction
public:
	CFilmDetailsDialog(CSlideListDoc *pDocument, BOOL bAddMode, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilmDetailsDialog)
	enum { IDD = IDD_FILM_DETAILS };
	CString	m_csDescription;
	CString	m_csNotes;
	CString	m_csMake;
	DWORD	m_Number;
	DWORD	m_Year;
	int		m_Size;
	int		m_Type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilmDetailsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilmDetailsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusFilmYear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    virtual void OnOK();

// private attributes
private:
	BOOL		        m_bAddMode;
    CSlideListDoc      *m_pDocument;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILMDETAILSDIALOG_H__BEE3F4E6_765C_4E4B_8230_4629B2135774__INCLUDED_)
