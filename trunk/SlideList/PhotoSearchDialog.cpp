// PhotoSearchDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"

#include "SlideListDoc.h"

#include "PhotoSearchDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoSearchDialog dialog


CPhotoSearchDialog::CPhotoSearchDialog(CSlideListDoc *pDocument, CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoSearchDialog::IDD, pParent)
{
	m_pDocument = pDocument;

	//{{AFX_DATA_INIT(CPhotoSearchDialog)
	m_bDescription = TRUE;
	m_bKeyword = TRUE;
	m_bLocation = TRUE;
	m_SearchString = _T("");
	m_SelectionMode = 0;
	m_SearchRating = 0;
	//}}AFX_DATA_INIT
}


void CPhotoSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhotoSearchDialog)
	DDX_Control(pDX, IDC_SEARCH_KEYWORD_LIST, m_KeywordList);
	DDX_Check(pDX, IDC_DESCRIPTION_SEARCH, m_bDescription);
	DDX_Check(pDX, IDC_KEYWORD_SEARCH, m_bKeyword);
	DDX_Check(pDX, IDC_LOCATION_SEARCH, m_bLocation);
	DDX_Text(pDX, IDC_SEARCH_STRING, m_SearchString);
	DDX_Radio(pDX, IDC_SEARCH_CLEAR_EXISTING, m_SelectionMode);
	DDX_CBIndex(pDX, IDC_SEARCH_RATING, m_SearchRating);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhotoSearchDialog, CDialog)
	//{{AFX_MSG_MAP(CPhotoSearchDialog)
	ON_LBN_DBLCLK(IDC_SEARCH_KEYWORD_LIST, OnDblclkSearchKeywordList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotoSearchDialog message handlers

BOOL CPhotoSearchDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    // Add the keywords to the list
    POSITION Pos;
    Pos = m_pDocument->GetKeywordHead();
    while ( Pos )
    {
        m_KeywordList.AddString( m_pDocument->GetNextKeyword( Pos ) );
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPhotoSearchDialog::OnDblclkSearchKeywordList() 
{
    // Get the keyword thats been double clicked and add it to the search text

    int Selection = m_KeywordList.GetCurSel();

    if ( Selection != LB_ERR )
    {
        // Get the text
        CString csSelection;
        m_KeywordList.GetText( Selection, csSelection );

        // Get the current keywords for the photo
        CString csCurrentSearchText;
        GetDlgItem( IDC_SEARCH_STRING )->GetWindowText( csCurrentSearchText );

        if ( csCurrentSearchText.GetLength() > 0 )
        {
            csCurrentSearchText += _T(" ");
        }

        // Add the selected keyword
        csCurrentSearchText += csSelection;

        GetDlgItem( IDC_SEARCH_STRING )->SetWindowText( csCurrentSearchText );
    }
}
