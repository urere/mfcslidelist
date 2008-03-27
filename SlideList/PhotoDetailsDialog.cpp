// PhotoDetailsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "PhotoDetailsDialog.h"

#include "SlideListDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoDetailsDialog dialog


CPhotoDetailsDialog::CPhotoDetailsDialog(CSlideListDoc *pDocument, BOOL bAddMode, CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoDetailsDialog::IDD, pParent)
{
    m_bAddMode = bAddMode;
    m_pDocument = pDocument;

	//{{AFX_DATA_INIT(CPhotoDetailsDialog)
	m_csDescription = _T("");
	m_csNotes = _T("");
	m_csKeywords = _T("");
	m_csLocation = _T("");
	m_Number = 0;
	m_Rating = -1;
	//}}AFX_DATA_INIT
}


void CPhotoDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhotoDetailsDialog)
	DDX_Control(pDX, IDC_KEYWORD_LIST, m_KeywordsList);
	DDX_Text(pDX, IDC_FILE_DESCRIPTION, m_csDescription);
	DDV_MaxChars(pDX, m_csDescription, 36);
	DDX_Text(pDX, IDC_FILE_NOTES, m_csNotes);
	DDV_MaxChars(pDX, m_csNotes, 160);
	DDX_Text(pDX, IDC_PHOTO_KEYWORDS, m_csKeywords);
	DDV_MaxChars(pDX, m_csKeywords, 120);
	DDX_Text(pDX, IDC_PHOTO_LOCATION, m_csLocation);
	DDV_MaxChars(pDX, m_csLocation, 36);
	DDX_Text(pDX, IDC_PHOTO_NUMBER, m_Number);
	DDV_MinMaxDWord(pDX, m_Number, 0, 50);
	DDX_CBIndex(pDX, IDC_RATING_LIST, m_Rating);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhotoDetailsDialog, CDialog)
	//{{AFX_MSG_MAP(CPhotoDetailsDialog)
	ON_LBN_DBLCLK(IDC_KEYWORD_LIST, OnDblclkKeywordList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotoDetailsDialog message handlers

BOOL CPhotoDetailsDialog::OnInitDialog() 
{
    ASSERT(m_pDocument);

	CDialog::OnInitDialog();
	
    // Setup title, depending on mode
    CString csTitleFormat;
    if ( m_bAddMode )
    {
        csTitleFormat.LoadString(IDS_PHOTO_TITLE_ADD);
    }
    else
    {
        csTitleFormat.LoadString(IDS_PHOTO_TITLE_CHANGE);
    }
    CString csTitle;
    csTitle.Format( csTitleFormat, m_Number );
    SetWindowText( csTitle );

    // Add the keywords to the list
    POSITION Pos;
    Pos = m_pDocument->GetKeywordHead();
    while ( Pos )
    {
        m_KeywordsList.AddString( m_pDocument->GetNextKeyword( Pos ) );
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPhotoDetailsDialog::OnDblclkKeywordList() 
{
    // Get the keyword thats been double clicked and add it to the keywords
    // for this photo

    int Selection = m_KeywordsList.GetCurSel();

    if ( Selection != LB_ERR )
    {
        // Get the text
        CString csSelection;
        m_KeywordsList.GetText( Selection, csSelection );

        // Get the current keywords for the photo
        CString csCurrentKeywords;
        GetDlgItem( IDC_PHOTO_KEYWORDS )->GetWindowText( csCurrentKeywords );

        if ( csCurrentKeywords.GetLength() > 0 )
        {
            csCurrentKeywords += _T(" ");
        }

        // Add the selected keyword
        csCurrentKeywords += csSelection;

        GetDlgItem( IDC_PHOTO_KEYWORDS )->SetWindowText( csCurrentKeywords );

    }

}
