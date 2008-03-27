// SlideListDoc.cpp : implementation of the CSlideListDoc class
//

#include "stdafx.h"
#include "SlideList.h"

#include "SlideListDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFilmDetails
//

CFilmDetails::CFilmDetails()
{
	m_wYear = 1;
	m_wFilmNumber = 0;
	m_csDescription = _T("");
	m_wType = FILM_COLOUR_SLIDE;
	m_wSize = SIZE_35MM;
	m_csFilmMake = _T("");
	m_wNextPhotoNumber = 1;
	m_csNotes = _T("");
}

CFilmDetails::~CFilmDetails()
{
	// Remove all of the slides from the list
	while ( !m_PhotosList.IsEmpty() )
	{
		delete m_PhotosList.RemoveTail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Serialization support
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CFilmDetails, CObject, 0x0001 );

void CFilmDetails::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );

	if ( ar.IsStoring() )
	{
		// Save the film details
		ar << m_wYear;
		ar << m_wFilmNumber;
		ar << m_csDescription;
		ar << m_wType;
		ar << m_wSize;
		ar << m_csFilmMake;
		ar << m_wNextPhotoNumber;
		ar << m_csNotes;

        // String attributes
        m_Attributes.Serialize( ar );

		// Photos
		m_PhotosList.Serialize( ar );;
	}
	else
	{
		// Load the film details
		ar >> m_wYear;
		ar >> m_wFilmNumber;
		ar >> m_csDescription;
		ar >> m_wType;
		ar >> m_wSize;
		ar >> m_csFilmMake;
		ar >> m_wNextPhotoNumber;
		ar >> m_csNotes;

        // String attributes
        m_Attributes.Serialize( ar );

        // Photos
		m_PhotosList.Serialize( ar );;
	}

}


/////////////////////////////////////////////////////////////////////////////
// Public operations
/////////////////////////////////////////////////////////////////////////////

BOOL CFilmDetails::AddPhoto( CPhotoDetails *pNewPhoto )
{
	ASSERT(pNewPhoto);

	BOOL bAdded = TRUE;

	// Ensure the film year and film number are correct
	// The user doesn't have direct control over these
	pNewPhoto->SetFilmNumber( GetFilmNumber() );
	pNewPhoto->SetFilmYear( GetFilmYear() );

	// Add the supplied photo to the list
	m_PhotosList.AddTail( pNewPhoto );

	return( bAdded );
}

BOOL CFilmDetails::DeletePhoto( CPhotoDetails *pPhoto )
{
    BOOL bRemoved = FALSE;

    // Remove this from the list
    POSITION Pos = m_PhotosList.Find( pPhoto );
    if ( Pos )
    {
        m_PhotosList.RemoveAt( Pos );

        // Finished with this photo
        delete pPhoto;

        bRemoved = TRUE;
    }

    return( bRemoved );
}

CPhotoDetails *CFilmDetails::GetPhotoDetails( WORD wPhotoNumber )
{
    // Locate the detailsof the photo for this film matching the supplied number
    CPhotoDetails *pPhoto = NULL;

    POSITION Pos = GetPhotoListHead();
    while ( Pos )
    {
        pPhoto = GetNextPhoto( Pos );
        ASSERT(pPhoto);

        if ( pPhoto->GetPhotoNumber() == wPhotoNumber )
        {
            // Found it
            break;
        }
        pPhoto = NULL;
    }

    return( pPhoto );
}

CString CFilmDetails::GetFilmSummary( void )
{
	// Build a simple summary string for the film
	CString csFilmSummary;
	csFilmSummary.Format( "[%s] %s", (LPCTSTR) GetFilmReferenceNumber(), (LPCTSTR) GetDescription() );

	return( csFilmSummary );
}

CString CFilmDetails::GetFilmReferenceNumber( void )
{
	// Build a reference number string for the film
	CString csFilmSummary;
	csFilmSummary.Format( "%2.2u.%3.3u", GetFilmYear(), GetFilmNumber() );

	return( csFilmSummary );
}

int CFilmDetails::GetNumberPhotos( void )
{
	// Return the total number of photos
	int NumPhotos = 0;

	NumPhotos = m_PhotosList.GetCount();

	return( NumPhotos );
}

int CFilmDetails::GetNumberSelectPhotos( void )
{
	// This functions returns the number of selected photos in this film
	// It returns 0 if no photos are selected

	int NumSelected = 0;

	CPhotoDetails *pPhoto;
	POSITION Pos = GetPhotoListHead();
	while ( Pos )
	{
		pPhoto = GetNextPhoto( Pos );
		if ( pPhoto )
		{
			if ( pPhoto->IsSelected() )
			{
				NumSelected += 1;
			}
		}
	}

	return( NumSelected );
}

BOOL CFilmDetails::WriteFilmData( CDataFile& File, BOOL bSelectedPhotosOnly )
{
	BOOL bDataWritten = TRUE;

	// Start a new hash
	File.StartHash( GetFilmReferenceNumber(), GetFilmSummary() );

	// Output the film attributes
	File.AddAttribute( "Number", (ULONG) m_wFilmNumber );
	CString csYear;
	csYear.Format( "20%2.2u", m_wYear );
	File.AddAttribute( "Year", csYear );
	File.AddAttribute( "Description", m_csDescription );
	File.AddAttribute( "Notes", m_csNotes );
	File.AddAttribute( "Make", m_csFilmMake );
	switch ( m_wType )
	{
		case FILM_COLOUR_PRINT:
			File.AddAttribute( "Type", "Colour Print" );
			break;
		case FILM_COLOUR_SLIDE:
			File.AddAttribute( "Type", "Colour Slide" );
			break;
		case FILM_BW_PRINT:
			File.AddAttribute( "Type", "BW Print" );
			break;
		case FILM_BW_SLIDE:
			File.AddAttribute( "Type", "BW Slide" );
			break;
		case FILM_OTHER:
		default:
			File.AddAttribute( "Type", "Other" );
			break;
	}
	switch ( m_wSize )
	{
		case SIZE_35MM:
			File.AddAttribute( "Size", "35mm" );
			break;
		case SIZE_OTHER:
		default:
			File.AddAttribute( "Size", "Other" );
			break;
	}

	// Add a list containing the reference number for each photo in the film
	CString csPhotos;
	CPhotoDetails *pPhoto;
	POSITION Pos = GetPhotoListHead();
	while ( Pos )
	{
		pPhoto = GetNextPhoto( Pos );
		if ( pPhoto )
		{
			if ( (!bSelectedPhotosOnly) || (bSelectedPhotosOnly && pPhoto->IsSelected()) )
			{
				if ( csPhotos != "" )
				{
					csPhotos += " ";
				}
				csPhotos += pPhoto->GetPhotoReferenceNumber();
			}
		}
	}
	File.AddAttribute( "Photos", csPhotos );


	// Output any other attributes
	CString csName;
	CString csValue;
	Pos = m_Attributes.GetStartPosition();
	while ( Pos )
	{
		m_Attributes.GetNextAssoc( Pos, csName, csValue );
		File.AddAttribute( csName, csValue );
	}

	File.EndHash();

	return ( bDataWritten );
}

//
// 
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CPhotoDetails
//

CPhotoDetails::CPhotoDetails()
{
	m_wFilmYear = 0;
	m_wFilmNumber = 0;
	m_wPhotoNumber = 0;
	m_csLocation = _T("");
	m_csDescription = _T("");
	m_wRating = 1;
	m_csKeywords = _T("");
	m_csNotes = _T("");

	m_bSelected = FALSE;
}

CPhotoDetails::~CPhotoDetails()
{
}

/////////////////////////////////////////////////////////////////////////////
// Serialization support
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CPhotoDetails, CObject, 0x0001 );

void CPhotoDetails::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );

	if ( ar.IsStoring() )
	{
		// Save the photo details
		ar << m_wFilmYear;
		ar << m_wFilmNumber;
		ar << m_wPhotoNumber;
		ar << m_csLocation;
		ar << m_csDescription;
		ar << m_wRating;
		ar << m_csKeywords;
		ar << m_csNotes;

        // String attributes
        m_Attributes.Serialize( ar );
	}
	else
	{
		// Load the photo details
		ar >> m_wFilmYear;
		ar >> m_wFilmNumber;
		ar >> m_wPhotoNumber;
		ar >> m_csLocation;
		ar >> m_csDescription;
		ar >> m_wRating;
		ar >> m_csKeywords;
		ar >> m_csNotes;

        // String attributes
        m_Attributes.Serialize( ar );
	}

}

/////////////////////////////////////////////////////////////////////////////
// Public operations
/////////////////////////////////////////////////////////////////////////////

CString CPhotoDetails::GetPhotoSummary( void )
{
	CString csPhotoSummary;
	csPhotoSummary.Format( "[%s] %s", (LPCTSTR) GetPhotoReferenceNumber(), (LPCTSTR) GetDescription() );

    // Add location if present
    if ( !GetLocation().IsEmpty() )
    {
        csPhotoSummary += _T(", ");
        csPhotoSummary += GetLocation();
    }

	return( csPhotoSummary );
}

CString CPhotoDetails::GetPhotoReferenceNumber( void )
{
	// Generate a reference for the photo in the form yy-fff-pp
	CString csPhotoId;
	csPhotoId.Format( "%2.2u.%3.3u.%2.2u", GetFilmYear(), GetFilmNumber(), GetPhotoNumber() );

	return( csPhotoId );
}

void CPhotoDetails::GetFilmAndPhotoId( LPCTSTR pRefNumber, WORD *pYear, WORD *pFilmNum, WORD *pPhotoNum )
{
	// Format is yy.nnn.pp
	//           012345678

	CString RefNum = pRefNumber;

	*pYear = (WORD) atoi( RefNum.Mid( 0, 2 ) );
	*pFilmNum = (WORD) atoi( RefNum.Mid( 3, 3 ) );
	*pPhotoNum = (WORD) atoi( RefNum.Mid( 7, 2 ) );
}

BOOL CPhotoDetails::WritePhotoData( CDataFile& File )
{
	BOOL bDataWritten = TRUE;

	// Start a new hash
	File.StartHash( GetPhotoReferenceNumber(), GetPhotoSummary() );

	// Output the Photo attributes
	File.AddAttribute( "Film", GetPhotoReferenceNumber().Left(6) );
	File.AddAttribute( "Description", m_csDescription );
	File.AddAttribute( "Location", m_csLocation );
	File.AddAttribute( "Keywords", m_csKeywords );
	File.AddAttribute( "Notes", m_csNotes );
	File.AddAttribute( "Rating", (ULONG) m_wRating );

	// Output any other attributes
	CString csName;
	CString csValue;
	POSITION Pos = m_Attributes.GetStartPosition();
	while ( Pos )
	{
		m_Attributes.GetNextAssoc( Pos, csName, csValue );
		File.AddAttribute( csName, csValue );
	}

	File.EndHash();

	return ( bDataWritten );
}


//
// 
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// CSlideListDoc

IMPLEMENT_DYNCREATE(CSlideListDoc, CDocument)

BEGIN_MESSAGE_MAP(CSlideListDoc, CDocument)
	//{{AFX_MSG_MAP(CSlideListDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlideListDoc construction/destruction

CSlideListDoc::CSlideListDoc()
{
	// Nothing selected
	m_pSelectedFilm = NULL;
	m_pSelectedPhoto = NULL;

	m_SelectedPhotoCount = 0;
}

CSlideListDoc::~CSlideListDoc()
{
	// Remove all files and slides
	while ( !m_FilmList.IsEmpty() )
	{
		delete m_FilmList.RemoveTail();
	}
}

BOOL CSlideListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Remove all files and slides
	while ( !m_FilmList.IsEmpty() )
	{
		delete m_FilmList.RemoveTail();
	}

	// Remove any keywords that have been collected
	m_Keywords.RemoveAll();

	// Nothing selected
	m_pSelectedFilm = NULL;
	m_pSelectedPhoto = NULL;

	m_SelectedPhotoCount = 0;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSlideListDoc serialization

void CSlideListDoc::Serialize(CArchive& ar)
{
	// Save/Load the file list
	m_FilmList.Serialize( ar );

    // Save/load the keywords
    m_Keywords.Serialize( ar );

	if (ar.IsStoring())
	{
		// Changes have been saved
		SetModifiedFlag( FALSE );
	}
	else
	{
		// Nothing selected
		m_pSelectedFilm = NULL;
		m_pSelectedPhoto = NULL;

		m_SelectedPhotoCount = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Public operations

void CSlideListDoc::SelectionChange( CFilmDetails *pFilm, CPhotoDetails *pPhoto )
{
	// Store the selection information
	m_pSelectedFilm = pFilm;
	m_pSelectedPhoto = pPhoto;
}

WORD CSlideListDoc::GetNextFilmNumber( WORD wYear )
{
    // Determine the next film for the supplied year

    WORD wMaxCurrent = 0;
    POSITION Pos = m_FilmList.GetHeadPosition();
    while ( Pos )
    {
        CFilmDetails *pFilm = (CFilmDetails *) m_FilmList.GetNext( Pos );

        if ( pFilm->GetFilmYear() == wYear )
        {
            if ( pFilm->GetFilmNumber() > wMaxCurrent )
            {
                wMaxCurrent = pFilm->GetFilmNumber();
            }
        }
    }

    // Return one more that the current for the supplied year

    return( wMaxCurrent+1 );
}

BOOL CSlideListDoc::IsFilmNumberDuplicate( WORD wYear, WORD wNumber )
{
    BOOL bDuplicate = FALSE;

    POSITION Pos = m_FilmList.GetHeadPosition();
    while ( Pos )
    {
        CFilmDetails *pFilm = (CFilmDetails *) m_FilmList.GetNext( Pos );

        if ( pFilm->GetFilmYear() == wYear )
        {
            if ( pFilm->GetFilmNumber() == wNumber )
            {
                bDuplicate = TRUE;
                break;
            }
        }
    }

    return( bDuplicate );
}

BOOL CSlideListDoc::AddFilm( CFilmDetails *pNewFilm, CFilmDetails * &pPreviousFilm )
{
    ASSERT(pNewFilm);

    BOOL bAdded = TRUE;

    // Locate the previous file in the list
    CFilmDetails *pPrevFilm = NULL;
    POSITION Pos = m_FilmList.GetHeadPosition();
    POSITION PrevPos = Pos;
    while ( Pos )
    {
        CFilmDetails *pFilm = (CFilmDetails *) m_FilmList.GetNext( Pos );

        // Does this file go after the new one ?
        if ( pFilm->GetFilmYear() > pNewFilm->GetFilmYear() )
        {
            // Year is greater. so use previous
            break;
        }
        else if ( (pFilm->GetFilmYear() == pNewFilm->GetFilmYear()) && (pFilm->GetFilmNumber() > pNewFilm->GetFilmNumber()) )
        {
            // Year is the same but number is greater. so use previous
            break;
        }
        else
        {
            // Store this one and try again
            pPrevFilm = pFilm;
            // Remember the container position because its needed to insert
            PrevPos = Pos;
        }
    }

    if ( PrevPos )
    {
        // Add this film to the list
        m_FilmList.InsertBefore( PrevPos, pNewFilm );
        pPreviousFilm = pPrevFilm;
    }
    else
    {
        // Just add it at the end
        m_FilmList.AddTail( pNewFilm );
        pPreviousFilm = NULL;
    }

    // Mark document as modified
    SetModifiedFlag( TRUE );

    return( bAdded );
}

BOOL CSlideListDoc::UpdateFilm( CFilmDetails *pFilm )
{
    ASSERT(pFilm);

    BOOL bUpdated = TRUE;

    // Mark document as modified
    SetModifiedFlag( TRUE );

    return( bUpdated );
}

BOOL CSlideListDoc::DeleteFilm( CFilmDetails *pFilm )
{
    ASSERT(pFilm);

    BOOL bRemoved = FALSE;

	// If it is the selected film we are deleting, unselect it
	if ( pFilm == m_pSelectedFilm )
	{
		m_pSelectedFilm = NULL;
		m_pSelectedPhoto = NULL;
	}

    // Remove this from the list
    POSITION Pos = m_FilmList.Find( pFilm );
    if ( Pos )
    {
        m_FilmList.RemoveAt( Pos );

        // Finished with this film
        delete pFilm;

        // Mark document as modified
        SetModifiedFlag( TRUE );
        
        bRemoved = TRUE;
    }

    return( bRemoved );
}

BOOL CSlideListDoc::AddPhoto( CFilmDetails *pFilm, CPhotoDetails *pNewPhoto )
{
    ASSERT(pNewPhoto);
    ASSERT(pFilm);

    BOOL bAdded = TRUE;

    // Add this photo to the list
    pFilm->AddPhoto( pNewPhoto );

    // Increment the next film number
	pFilm->IncrementNextPhotoNumber();

    // Mark document as modified
    SetModifiedFlag( TRUE );

    // Add any new keywords from this photo
    AddToKeywords( pNewPhoto->GetKeywords() );

    return( bAdded );
}

BOOL CSlideListDoc::UpdatePhoto( CFilmDetails *pFilm, CPhotoDetails *pPhoto )
{
    ASSERT(pFilm);
    ASSERT(pPhoto);

    BOOL bUpdated = TRUE;

    // Mark document as modified
    SetModifiedFlag( TRUE );

    // Add any new keywords from this photo
    AddToKeywords( pPhoto->GetKeywords() );

    return( bUpdated );
}

BOOL CSlideListDoc::DeletePhoto( CFilmDetails *pFilm, CPhotoDetails *pPhoto )
{
    ASSERT(pFilm);
    ASSERT(pPhoto);

    BOOL bRemoved = FALSE;

	// If it is the selected photo we are deleting, unselect it
	if ( pPhoto == m_pSelectedPhoto )
	{
		m_pSelectedPhoto = NULL;
	}

    // Remove this from the list
    pFilm->DeletePhoto( pPhoto );

    // Mark document as modified
    SetModifiedFlag( TRUE );
    
    bRemoved = TRUE;

    return( bRemoved );
}

void CSlideListDoc::SelectAllPhotosInFilm( CFilmDetails *pFilm )
{
	// Ensure all of the photos in the supplied film are selected
	ASSERT(pFilm);

	CPhotoDetails *pPhoto;
	POSITION Pos = pFilm->GetPhotoListHead();
	while ( Pos )
	{
		pPhoto = pFilm->GetNextPhoto( Pos );
		if ( pPhoto )
		{
			SelectPhoto( pPhoto );
		}
	}
}

void CSlideListDoc::UnselectAllPhotosInFilm( CFilmDetails *pFilm )
{
	// Ensure all of the photos in the supplied film are unselected
	ASSERT(pFilm);

	CPhotoDetails *pPhoto;
	POSITION Pos = pFilm->GetPhotoListHead();
	while ( Pos )
	{
		pPhoto = pFilm->GetNextPhoto( Pos );
		if ( pPhoto )
		{
			UnselectPhoto( pPhoto );
		}
	}
}

void CSlideListDoc::UnselectAllPhotos( void )
{
	// Unselect every photo in all films
    POSITION Pos = m_FilmList.GetHeadPosition();
    while ( Pos )
    {
        CFilmDetails *pFilm = (CFilmDetails *) m_FilmList.GetNext( Pos );
        if ( pFilm )
        {
			UnselectAllPhotosInFilm( pFilm );
        }
    }
}

void CSlideListDoc::SelectPhoto( CPhotoDetails *pPhoto )
{
	ASSERT(pPhoto);

	if ( !pPhoto->IsSelected() )
	{
		pPhoto->SetSelected();

		m_SelectedPhotoCount += 1;

		ASSERT(m_SelectedPhotoCount >= 0);
	}
}

void CSlideListDoc::UnselectPhoto( CPhotoDetails *pPhoto )
{
	ASSERT(pPhoto);

	if ( pPhoto->IsSelected() )
	{
		pPhoto->ClearSelected();

		m_SelectedPhotoCount -= 1;

		ASSERT(m_SelectedPhotoCount >= 0);
	}
}

long int CSlideListDoc::DoPhotographSearch( LPCTSTR pSearchString, BOOL bKeywords, BOOL bDescription, BOOL bLocation, WORD SearchRating, CSlideListDoc::SearchMode Mode )
{
    long int NumberSelected = 0;

    // Do we need to clear existing selection ?
    if ( Mode == CLEAR_SELECTION_BEFORE )
    {
        this->UnselectAllPhotos();
    }

    // Build a list of strings to search for, this is a list
    // of strings taken from the supplied search string in upper case
    CStringList SearchList;

    CString csSearch = pSearchString;
    csSearch.MakeUpper();

    // Remove trailing spaces before we start
    csSearch.TrimRight();

    while ( csSearch != "" )
    {
        // Remove any leading spaces
        csSearch.TrimLeft();

        // Get the first word
        CString csWord = csSearch.SpanExcluding( " " );
        if ( csWord != "" )
        {
            // Add this to the list
            SearchList.AddTail( csWord );

            // Remove this from the search string
            csSearch = csSearch.Mid( csWord.GetLength() );
        }
    }

    // Iterate through all of the photos and search as we go
    POSITION Pos = m_FilmList.GetHeadPosition();
    while ( Pos )
    {
        CFilmDetails *pFilm = (CFilmDetails *) m_FilmList.GetNext( Pos );
        if ( pFilm )
        {
	        CPhotoDetails *pPhoto;
	        POSITION Pos = pFilm->GetPhotoListHead();
	        while ( Pos )
	        {
		        pPhoto = pFilm->GetNextPhoto( Pos );
		        if ( pPhoto )
		        {
                    // Search this photograph, don't bother if searching within selection is not selected
                    if ( (Mode == CLEAR_SELECTION_BEFORE) || (Mode == SEARCH_WITHIN_SELECTION && pPhoto->IsSelected()) || (Mode == ADD_EXISTING_SELECTION && !pPhoto->IsSelected()) )
                    {
                        BOOL bMatchFound = FALSE;

						// Check rating
						if ( (SearchRating == 0) || (pPhoto->GetRating() <= SearchRating) )
						{
							// Any strings gto check for a match? if not assume matched
							if ( SearchList.GetCount() > 0 )
							{
								// Check keywords
								if ( !bMatchFound && bKeywords )
								{
									bMatchFound = CheckSearchString( SearchList, pPhoto->GetKeywords() );
								}

								// Check description
								if ( !bMatchFound && bDescription )
								{
									bMatchFound = CheckSearchString( SearchList, pPhoto->GetDescription() );
								}

								// check location
								if ( !bMatchFound && bLocation )
								{
									bMatchFound = CheckSearchString( SearchList, pPhoto->GetLocation() );
								}
							}
							else
							{
								bMatchFound = TRUE;
							}
						}

                        if ( bMatchFound )
                        {
                            // Mark photo as selected
                            if ( !pPhoto->IsSelected() )
                            {
                               SelectPhoto( pPhoto );
                            }
                            NumberSelected += 1;
                        }
                        else
                        {
                            // Mark photo as unselected
                            if ( pPhoto->IsSelected() )
                            {
                               UnselectPhoto( pPhoto );
                            }
                        }
                    }
		        }
	        }
        }
    }

    return( NumberSelected );
}

BOOL CSlideListDoc::CheckSearchString( CStringList& SearchList, LPCTSTR pString )
{
    BOOL bMatch = FALSE;

    // Check each word in the search list to see if it appears in the supplied string
    // The search is for complete words

    CString csString = " ";
    csString += pString;
    csString += " ";

    // All tests are made in upper case
    csString.MakeUpper();

    POSITION Pos = SearchList.GetHeadPosition();
    while ( !bMatch && Pos )
    {
        CString csSearchWord = " ";
        csSearchWord += SearchList.GetNext( Pos );
        csSearchWord += " ";

        // Does this search word appear in the string ?
        if ( csString.Find( csSearchWord ) >= 0 )
        {
            bMatch = TRUE;
        }
    }

    return( bMatch );
}

/////////////////////////////////////////////////////////////////////////////
// Keyword support
void CSlideListDoc::AddToKeywords( LPCTSTR pNewKeywords )
{
    // Parses the supplied string for space delimited words
    // Each word is added to the keywords map if not already
    // present.
    // The key item in the map is stored in UPPER case
    // The keyword as entered by the user is stored as
    // the keyword data

    CString csKeywords(pNewKeywords);
    CString csCurrentKeyword;

    while ( csKeywords.GetLength() > 0 )
    {
        // Extract up to the first space
        int Position = csKeywords.Find( ' ' );
        if ( Position == -1 )
        {
            // No spaces left, the whole of the remaining string is the keyword
            csCurrentKeyword = csKeywords;
            csKeywords = _T("");
        }
        else
        {
            // Extract up to the space
            csCurrentKeyword = csKeywords.Left(Position);
            csKeywords = csKeywords.Mid( Position+1 );
        }

        if ( csCurrentKeyword.GetLength() > 0 )
        {
            // Create an upper case version of this keyword
            CString csCurrentUCKeyword = csCurrentKeyword;
            csCurrentUCKeyword.MakeUpper();

            // Does it exist in the keywords list ?
            CString csExistingKeyword;
            if ( !m_Keywords.Lookup( csCurrentUCKeyword, csExistingKeyword ) )
            {
                // Add this keyword to the map
                m_Keywords.SetAt( csCurrentUCKeyword, csCurrentKeyword );
            }
        }
    }
}

POSITION CSlideListDoc::GetKeywordHead( void )
{
    POSITION Pos;

    Pos = m_Keywords.GetStartPosition();

    return( Pos );
}

CString CSlideListDoc::GetNextKeyword( POSITION& Pos )
{
    // Return the next keyword value from the keywords map
    CString csKeyword;
    CString csUCKeyword;

    m_Keywords.GetNextAssoc( Pos, csUCKeyword, csKeyword );

    return( csKeyword );
}


BOOL CSlideListDoc::GenerateDataFile( LPCTSTR pFileName, BOOL bSelectedPhotosOnly )
{
	BOOL bGenerated = FALSE;

	// Generate file containing document data as a series of perl hash's

	// Open the file
	CDataFile DataFile;

	if ( DataFile.Open( pFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) )
	{
		POSITION		FilmPos = 0;
		CFilmDetails   *pFilmDetails = NULL;

		FilmPos = GetFilmListHead();
		// Get the first film details
		if ( FilmPos )
		{
			// Create the films variable
			DataFile.StartHashVariable( "Films", "Film Details" );

			while ( FilmPos )
			{
				pFilmDetails = GetNextFilm( FilmPos );
				if ( pFilmDetails )
				{
					if ( (!bSelectedPhotosOnly) || (bSelectedPhotosOnly && (pFilmDetails->GetNumberSelectPhotos() > 0)) )
						pFilmDetails->WriteFilmData( DataFile, bSelectedPhotosOnly );
				}
			}

			DataFile.EndHashVariable();

			// Create the photos variable
			FilmPos = GetFilmListHead();
			DataFile.StartHashVariable( "Photos", "Photograph Details" );

			while ( FilmPos )
			{
				pFilmDetails = GetNextFilm( FilmPos );
				if ( pFilmDetails )
				{
					// Output details of each photo graph
					POSITION PhotoPos = 0;
					CPhotoDetails *pPhotoDetails = NULL;

					PhotoPos = pFilmDetails->GetPhotoListHead();
					while ( PhotoPos )
					{
						pPhotoDetails = pFilmDetails->GetNextPhoto( PhotoPos );
						if ( pPhotoDetails )
						{
							if ( (!bSelectedPhotosOnly) || (bSelectedPhotosOnly && pPhotoDetails->IsSelected()) )
								pPhotoDetails->WritePhotoData( DataFile );
						}
					}

				}
			}

			DataFile.EndHashVariable();
		}
		else
		{
			DataFile.WriteString( "# No Films\n" );
		}

		DataFile.Close();

		bGenerated = TRUE;
	}

	return ( bGenerated );
}


/////////////////////////////////////////////////////////////////////////////
// CSlideListDoc diagnostics

#ifdef _DEBUG
void CSlideListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSlideListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSlideListDoc commands
