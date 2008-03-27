#if !defined(AFX_FILMADDPHOTOS_H__71585CE0_F266_11D5_9350_F50281E7471C__INCLUDED_)
#define AFX_FILMADDPHOTOS_H__71585CE0_F266_11D5_9350_F50281E7471C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilmAddPhotos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilmAddPhotosDialog dialog

class CFilmAddPhotosDialog : public CDialog
{
// Construction
public:
	CFilmAddPhotosDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilmAddPhotosDialog)
	enum { IDD = IDD_CREATE_PHOTOS };
	CString	m_Number;
	BOOL	m_StartAtZero;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilmAddPhotosDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilmAddPhotosDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILMADDPHOTOS_H__71585CE0_F266_11D5_9350_F50281E7471C__INCLUDED_)
