// FilmAddPhotos.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "FilmAddPhotos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilmAddPhotosDialog dialog


CFilmAddPhotosDialog::CFilmAddPhotosDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFilmAddPhotosDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilmAddPhotosDialog)
	m_Number = _T("");
	m_StartAtZero = FALSE;
	//}}AFX_DATA_INIT
}


void CFilmAddPhotosDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilmAddPhotosDialog)
	DDX_CBString(pDX, IDC_NUMBER_PHOTOS, m_Number);
	DDV_MaxChars(pDX, m_Number, 2);
	DDX_Check(pDX, IDC_START_AT_ZERO, m_StartAtZero);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilmAddPhotosDialog, CDialog)
	//{{AFX_MSG_MAP(CFilmAddPhotosDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilmAddPhotosDialog message handlers



BOOL CFilmAddPhotosDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
