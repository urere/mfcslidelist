// SlideList.h : main header file for the SLIDELIST application
//

#if !defined(AFX_SLIDELIST_H__3CB82E19_7236_4E7F_BD6D_B4BCF4C5AB9B__INCLUDED_)
#define AFX_SLIDELIST_H__3CB82E19_7236_4E7F_BD6D_B4BCF4C5AB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSlideListApp:
// See SlideList.cpp for the implementation of this class
//

class CSlideListApp : public CWinApp
{
public:
	CSlideListApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideListApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSlideListApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDELIST_H__3CB82E19_7236_4E7F_BD6D_B4BCF4C5AB9B__INCLUDED_)
