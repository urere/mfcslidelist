#if !defined(AFX_REPORTSELECTION_H__318943AD_1A4A_4E5A_BE72_C684A0B7EB65__INCLUDED_)
#define AFX_REPORTSELECTION_H__318943AD_1A4A_4E5A_BE72_C684A0B7EB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportSelection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportSelection dialog

class CReportSelection : public CDialog
{
// Construction
public:
	CReportSelection(CWnd* pParent = NULL);   // standard constructor

	CString GetReportPathName( void ) { return( m_ReportPathName ); };

// Dialog Data
	//{{AFX_DATA(CReportSelection)
	enum { IDD = IDD_REPORT_SELECTION };
	CButton	m_Ok;
	CListBox	m_ReportList;
	BOOL	m_SelectedOnly;
	//}}AFX_DATA

	BOOL	m_bAllowSelectedOnly;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportSelection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportSelection)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray	m_ReportFileNames;
	CString			m_ReportPathName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTSELECTION_H__318943AD_1A4A_4E5A_BE72_C684A0B7EB65__INCLUDED_)
