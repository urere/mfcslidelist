// ReportSelection.cpp : implementation file
//

#include "stdafx.h"
#include "SlideList.h"
#include "ReportSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REPORT_DESCRIPTION "# REPORT:"

/////////////////////////////////////////////////////////////////////////////
// CReportSelection dialog


CReportSelection::CReportSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CReportSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportSelection)
	m_SelectedOnly = FALSE;
	//}}AFX_DATA_INIT
	m_ReportPathName = "";

	m_bAllowSelectedOnly = FALSE;
}


void CReportSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportSelection)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_REPORT_LIST, m_ReportList);
	DDX_Check(pDX, IDC_SELECTED_PHOTOS, m_SelectedOnly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportSelection, CDialog)
	//{{AFX_MSG_MAP(CReportSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSelection message handlers


BOOL CReportSelection::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Need to populate the list box with details of available reports
	// These files all have the file extension .rep
	// The first list if prefixed by # REPORT: contains a description

	CFileFind FileFind;
	DWORD ReportIndex = 0;

	// Look for reports in the same directory as the executable
	TCHAR PathBuffer[1024];

	GetModuleFileName( NULL, PathBuffer, sizeof(PathBuffer) );

	CString csSearchPath = PathBuffer;
	csSearchPath = csSearchPath.Left( csSearchPath.ReverseFind('\\') );
	csSearchPath += "\\*.rep";

	BOOL bWorking = FileFind.FindFile(csSearchPath);
	while (bWorking)
	{
		bWorking = FileFind.FindNextFile();
		      
		// Process this file
		CStdioFile ReportFile;
		if ( ReportFile.Open( FileFind.GetFilePath(), CFile::modeRead | CFile::typeText ) )
		{
			CString csLine;
			CString csDescriptionMarker = REPORT_DESCRIPTION;
			int Index;
			if ( ReportFile.ReadString( csLine ) )
			{
				if ( csLine.Left(csDescriptionMarker.GetLength()) == csDescriptionMarker )
				{
					// Show the description
					Index = m_ReportList.AddString( csLine.Mid(csDescriptionMarker.GetLength()) );
				}
				else
				{
					// Just store the file name
					Index = m_ReportList.AddString( FileFind.GetFileName() );
				}
				
				// Store the full report file name in the array at the returned index value
				// This is done so the file name can be retreived later
				m_ReportList.SetItemData( Index, ReportIndex );
				m_ReportFileNames.SetAtGrow( ReportIndex, FileFind.GetFilePath() );

				ReportIndex += 1;
			}

			ReportFile.Close();
		}
	}

	// Select the first report, if there are none, disable the OK button
	if ( ReportIndex > 0 )
	{
		m_ReportList.SetCurSel( 0 );
		m_Ok.EnableWindow( TRUE );
	}
	else
	{
		m_Ok.EnableWindow( FALSE );
	}
	
	if ( m_bAllowSelectedOnly )
	{
		// Enable selective printing and select the control
		GetDlgItem( IDC_SELECTED_PHOTOS )->EnableWindow( TRUE );
	}
	else
	{
		// No selective printing
		GetDlgItem( IDC_SELECTED_PHOTOS )->EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CReportSelection::OnOK() 
{
	int Index = m_ReportList.GetCurSel();
	if ( Index != LB_ERR )
	{
		// Get the full path name for the report
		m_ReportPathName = m_ReportFileNames[ m_ReportList.GetItemData( Index ) ];

		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}
