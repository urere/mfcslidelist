// FilmDetailsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "FilmDetailsDialog.h"

#include "SlideListDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilmDetailsDialog dialog

CFilmDetailsDialog::CFilmDetailsDialog(CSlideListDoc *pDocument, BOOL bAddMode, CWnd* pParent /* = NULL */ )
	: CDialog(CFilmDetailsDialog::IDD, pParent)
{
	// Store mode this will be used to create a title
	m_bAddMode = bAddMode;
    m_pDocument = pDocument;

	//{{AFX_DATA_INIT(CFilmDetailsDialog)
	m_csDescription = _T("");
	m_csNotes = _T("");
	m_csMake = _T("");
	m_Number = 0;
	m_Year = 0;
	m_Size = -1;
	m_Type = -1;
	//}}AFX_DATA_INIT
}


void CFilmDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilmDetailsDialog)
	DDX_Text(pDX, IDC_FILE_DESCRIPTION, m_csDescription);
	DDV_MaxChars(pDX, m_csDescription, 36);
	DDX_Text(pDX, IDC_FILE_NOTES, m_csNotes);
	DDV_MaxChars(pDX, m_csNotes, 160);
	DDX_Text(pDX, IDC_FILM_MAKE, m_csMake);
	DDV_MaxChars(pDX, m_csMake, 36);
	DDX_Text(pDX, IDC_FILM_NUMBER, m_Number);
	DDV_MinMaxDWord(pDX, m_Number, 1, 999);
	DDX_Text(pDX, IDC_FILM_YEAR, m_Year);
	DDV_MinMaxDWord(pDX, m_Year, 0, 99);
	DDX_CBIndex(pDX, IDC_FILM_SIZE, m_Size);
	DDX_CBIndex(pDX, IDC_FILM_TYPE, m_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilmDetailsDialog, CDialog)
	//{{AFX_MSG_MAP(CFilmDetailsDialog)
	ON_EN_KILLFOCUS(IDC_FILM_YEAR, OnKillfocusFilmYear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilmDetailsDialog message handlers

BOOL CFilmDetailsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    // Setup title, depending on mode
    CString csTitle;
    if ( m_bAddMode )
    {
        csTitle.LoadString(IDS_FILM_TITLE_ADD);
    }
    else
    {
        csTitle.LoadString(IDS_FILM_TITLE_CHANGE);
    }
    SetWindowText( csTitle );

    // In change mode can't change year and number
    if ( !m_bAddMode )
    {
        GetDlgItem( IDC_FILM_YEAR )->EnableWindow( FALSE );
        GetDlgItem( IDC_FILM_NUMBER )->EnableWindow( FALSE );
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilmDetailsDialog::OnKillfocusFilmYear() 
{
    // Film year may have been changed. Generate a new default
    // film number

    CString csYear;
    GetDlgItem( IDC_FILM_YEAR )->GetWindowText( csYear );

    DWORD dwNewYear = (DWORD) atol( (LPCTSTR) csYear );

    // Year as changed, get the next best number
    WORD wNewNumber = m_pDocument->GetNextFilmNumber( (WORD) dwNewYear );
    
    // Display the new number
    CString csNewNumber;
    csNewNumber.Format( "%u", wNewNumber );

    GetDlgItem( IDC_FILM_NUMBER )->SetWindowText( csNewNumber );
}

void CFilmDetailsDialog::OnOK()
{
    // Check to ensure film number is not a duplicate
    // before allowing exit
    // This only applies when adding a new film

    if ( m_bAddMode )
    {
        CString csYear;
        GetDlgItem( IDC_FILM_YEAR )->GetWindowText( csYear );
        WORD wNewYear = (WORD) atol( (LPCTSTR) csYear );

        CString csNumber;
        GetDlgItem( IDC_FILM_NUMBER )->GetWindowText( csNumber );
        WORD wNewNumber = (WORD) atol( (LPCTSTR) csNumber );

        if ( m_pDocument->IsFilmNumberDuplicate( wNewYear, wNewNumber ) )
        {
            // Display a warning message to the user 
            CString csMessageFormat;
            csMessageFormat.LoadString( IDS_FILM_DUPLICATE_MESSAGE );

            CString csMessage;
            csMessage.Format( csMessageFormat, wNewNumber );

            CString csCaption;
            csCaption.LoadString( IDS_FILM_DUPLICATE_CAPTION );

            MessageBox( csMessage, csCaption, MB_ICONERROR | MB_OK ); 

            // Entered film number is a duplicate
            GetDlgItem( IDC_FILM_NUMBER )->SetFocus();
        }
        else
        {
            // Number is OK
            CDialog::OnOK();
        }
    }
    else
    {
        CDialog::OnOK();
    }
}
