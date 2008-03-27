// SlideListDoc.h : interface of the CSlideListDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIDELISTDOC_H__268F0CB8_3EFA_4973_B864_21C6450A76E2__INCLUDED_)
#define AFX_SLIDELISTDOC_H__268F0CB8_3EFA_4973_B864_21C6450A76E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DataFile.h"

class CPhotoDetails;

//////////////////
// CFilmDetails //
//////////////////

class CFilmDetails : public CObject
{
	DECLARE_SERIAL(CFilmDetails);

// Public constants etc
public:
	enum FilmType
	{
		FILM_COLOUR_PRINT = 0,
		FILM_COLOUR_SLIDE,
		FILM_BW_PRINT,
		FILM_BW_SLIDE,
		FILM_OTHER
	};
	enum FilmSize
	{
		SIZE_35MM = 0,
		SIZE_OTHER
	};

// Construction/Destruction
public:
	CFilmDetails();
	virtual ~CFilmDetails();

// Public operations
public:
	void Serialize( CArchive& ar );
	CString GetFilmSummary( void );
	CString GetFilmReferenceNumber( void );

public:
// Attribute access
	WORD GetFilmYear( void ) { return( m_wYear ); };
	void SetFilmYear( WORD wYear ) { m_wYear = wYear; };

	WORD GetFilmNumber( void ) { return( m_wFilmNumber ); };
	void SetFilmNumber( WORD wFilmNumber ) { m_wFilmNumber = wFilmNumber; };

	CString& GetDescription( void ) { return( m_csDescription ); };
	void SetDescription( LPCTSTR pDescription ) { m_csDescription = pDescription; };

	WORD GetType( void ) { return( m_wType ); };
	void SetType( WORD wType ) { m_wType = wType; };

	WORD GetSize( void ) { return( m_wSize ); };
	void SetSize( WORD wSize ) { m_wSize = wSize; };

	CString& GetFilmMake( void ) { return( m_csFilmMake ); };
	void SetFilmMake( LPCTSTR pFilmMake ) { m_csFilmMake = pFilmMake; };

	WORD GetNextPhotoNumber( void ) { return( m_wNextPhotoNumber ); };
	void SetNextPhotoNumber( WORD wNextPhotoNumber ) { m_wNextPhotoNumber = wNextPhotoNumber; };
    void IncrementNextPhotoNumber( void ) { m_wNextPhotoNumber += 1; };

	CString& GetNotes( void ) { return( m_csNotes ); };
	void SetNotes( LPCTSTR pNotes ) { m_csNotes = pNotes; };

	BOOL AddPhoto( CPhotoDetails *pNewPhoto );
	BOOL DeletePhoto( CPhotoDetails *pPhoto );
    CPhotoDetails *GetPhotoDetails( WORD wPhotoNumber ); 

	POSITION GetPhotoListHead( void ) { return ( m_PhotosList.GetHeadPosition() ); };
	CPhotoDetails *GetNextPhoto( POSITION& Pos ) { return((CPhotoDetails *) m_PhotosList.GetNext( Pos )); };

	int GetNumberPhotos( void );
	int GetNumberSelectPhotos( void );

	BOOL WriteFilmData( CDataFile& File, BOOL bSelectedPhotosOnly );

// Attributes
private:
	WORD		            m_wYear;
	WORD		            m_wFilmNumber;
	CString		            m_csDescription;
	WORD		            m_wType;
	WORD		            m_wSize;
	CString		            m_csFilmMake;
	WORD		            m_wNextPhotoNumber;
	CString		            m_csNotes;
    CMapStringToString      m_Attributes;
	CObList		            m_PhotosList;
};


///////////////////
// CPhotoDetails //
///////////////////

class CPhotoDetails : public CObject
{
	DECLARE_SERIAL(CPhotoDetails);

// Construction/Destruction
public:
	CPhotoDetails();
	virtual ~CPhotoDetails();

// Public operations
public:
	void Serialize( CArchive& ar );
	CString GetPhotoSummary( void );
	CString GetPhotoReferenceNumber( void );
	static void GetFilmAndPhotoId( LPCTSTR pRefNumber, WORD *pYear, WORD *pFilmNum, WORD *pPhotoNum );

public:
// Attribute access
	WORD GetFilmYear( void ) { return( m_wFilmYear ); };
	void SetFilmYear( WORD wFilmYear ) { m_wFilmYear = wFilmYear; };

	WORD GetFilmNumber( void ) { return( m_wFilmNumber ); };
	void SetFilmNumber( WORD wFilmNumber ) { m_wFilmNumber = wFilmNumber; };

	WORD GetPhotoNumber( void ) { return( m_wPhotoNumber ); };
	void SetPhotoNumber( WORD wPhotoNumber ) { m_wPhotoNumber = wPhotoNumber; };

	CString& GetLocation( void ) { return( m_csLocation ); };
	void SetLocation( LPCTSTR pLocation ) { m_csLocation = pLocation; };

	CString& GetDescription( void ) { return( m_csDescription ); };
	void SetDescription( LPCTSTR pDescription ) { m_csDescription = pDescription; };

	WORD GetRating( void ) { return( m_wRating ); };
	void SetRating( WORD wRating ) { m_wRating = wRating; };

	CString& GetKeywords( void ) { return( m_csKeywords ); };
	void SetKeywords( LPCTSTR pKeywords ) { m_csKeywords = pKeywords; };

	CString& GetNotes( void ) { return( m_csNotes ); };
	void SetNotes( LPCTSTR pNotes ) { m_csNotes = pNotes; };

	void SetSelected( void ) { m_bSelected = TRUE; };
	void ClearSelected( void ) { m_bSelected = FALSE; };
	BOOL IsSelected( void ) { return(m_bSelected);  };

// public operations
	BOOL WritePhotoData( CDataFile& File );

// Attributes
private:
	WORD		            m_wFilmYear;
	WORD		            m_wFilmNumber;
	WORD		            m_wPhotoNumber;
	CString		            m_csLocation;
	CString		            m_csDescription;
	WORD		            m_wRating;
	CString		            m_csKeywords;
    CMapStringToString      m_Attributes;
	CString		            m_csNotes;

	BOOL					m_bSelected;
};


class CSlideListDoc : public CDocument
{
// Public constants etc
public:

	enum ViewUpdateHint
	{
        TREE_SELECTION_CHANGE = 1,
		FILM_ADDED,
		FILM_CHANGED,
		FILM_DELETED,
		PHOTO_ADDED,
		PHOTO_CHANGED,
		PHOTO_DELETED,
		PHOTO_SELECTED,
		PHOTO_UNSELECTED
	};

    enum SearchMode
    {
        CLEAR_SELECTION_BEFORE = 1,
        ADD_EXISTING_SELECTION,
        SEARCH_WITHIN_SELECTION
    };

protected: // create from serialization only
	CSlideListDoc();
	DECLARE_DYNCREATE(CSlideListDoc)

// Attributes
public:

// Operations
public:
	POSITION GetFilmListHead( void ) { return ( m_FilmList.GetHeadPosition() ); };
	CFilmDetails *GetNextFilm( POSITION& Pos ) { return((CFilmDetails *) m_FilmList.GetNext( Pos )); };
    WORD GetNextFilmNumber( WORD wYear );
    BOOL IsFilmNumberDuplicate( WORD wYear, WORD wNumber );
	int GetNumberOfFilms( void ) { return ( m_FilmList.GetCount() ); };
	long int GetSelectedPhotoCount( void ) { return (m_SelectedPhotoCount); };

	BOOL AddFilm( CFilmDetails *pNewFilm, CFilmDetails * &pPreviousFilm );
	BOOL UpdateFilm( CFilmDetails *pFilm );
	BOOL DeleteFilm( CFilmDetails *pFilm );

   	BOOL AddPhoto( CFilmDetails *pFilm, CPhotoDetails *pNewPhoto );
	BOOL UpdatePhoto( CFilmDetails *pFilm, CPhotoDetails *pPhoto );
	BOOL DeletePhoto( CFilmDetails *pFilm, CPhotoDetails *pPhoto );

	long int CSlideListDoc::DoPhotographSearch( LPCTSTR pSearchString, BOOL bKeywords, BOOL bDescription, BOOL bLocation, WORD SearchRating, CSlideListDoc::SearchMode Mode );

	void SelectPhoto( CPhotoDetails *pPhoto );
	void UnselectPhoto( CPhotoDetails *pPhoto );

	void SelectAllPhotosInFilm( CFilmDetails *pFilm );
	void UnselectAllPhotosInFilm( CFilmDetails *pFilm );

	void UnselectAllPhotos( void );

    void AddToKeywords( LPCTSTR pNewKeywords );
    POSITION GetKeywordHead( void );
    CString GetNextKeyword( POSITION& Pos );

	BOOL GenerateDataFile( LPCTSTR pFileName, BOOL bSelectedPhotosOnly );

	void SelectionChange( CFilmDetails *pFilm, CPhotoDetails *pPhoto );

	CFilmDetails *GetSelectedFilm( void ) { return(m_pSelectedFilm); };
	CPhotoDetails *GetSelectedPhoto( void ) { return(m_pSelectedPhoto); };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideListDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSlideListDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
BOOL CheckSearchString( CStringList& SearchList, LPCTSTR pString );

// Attributes
private:
	CObList		        m_FilmList;     
	CMapStringToString  m_Keywords;
	CFilmDetails	   *m_pSelectedFilm;
	CPhotoDetails	   *m_pSelectedPhoto;
	long int			m_SelectedPhotoCount;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSlideListDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDELISTDOC_H__268F0CB8_3EFA_4973_B864_21C6450A76E2__INCLUDED_)
