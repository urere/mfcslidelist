// SlideListView.cpp : implementation of the CSlideListView class
//

#include "stdafx.h"
#include "SlideList.h"

#include "SlideListDoc.h"
#include "SlideListView.h"

// Dialog box support
#include "FilmDetailsDialog.h"
#include "PhotoDetailsDialog.h"
#include "FilmAddPhotos.h"
#include "PhotoSearchDialog.h"

// Printing support
#include "PrintingSupport.h"
#include "ReportSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REPORT_DATA "printdata.pl"
#define REPORT_FILE "report.txt"

/////////////////////////////////////////////////////////////////////////////
// CSlideListView

IMPLEMENT_DYNCREATE(CSlideListView, CTreeView)

BEGIN_MESSAGE_MAP(CSlideListView, CTreeView)
	//{{AFX_MSG_MAP(CSlideListView)
	ON_COMMAND(IDM_ADD_FILM, OnAddFilm)
	ON_COMMAND(IDM_FILM_CHANGE, OnFilmChange)
	ON_COMMAND(IDM_FILM_REMOVE, OnFilmRemove)
	ON_UPDATE_COMMAND_UI(IDM_ADD_FILM, OnUpdateAddFilm)
	ON_UPDATE_COMMAND_UI(IDM_FILM_CHANGE, OnUpdateFilmChange)
	ON_UPDATE_COMMAND_UI(IDM_FILM_REMOVE, OnUpdateFilmRemove)
	ON_COMMAND(IDM_PHOTO_ADD, OnAddPhoto)
	ON_COMMAND(IDM_PHOTO_CHANGE, OnChangePhoto)
	ON_COMMAND(IDM_PHOTO_REMOVE, OnRemovePhoto)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_ADD, OnUpdateAddPhoto)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_CHANGE, OnUpdateChangePhoto)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_REMOVE, OnUpdateRemovePhoto)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_COMMAND(IDM_PHOTO_SELECT, OnPhotoSelect)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_SELECT, OnUpdatePhotoSelect)
	ON_COMMAND(IDM_PHOTO_UNSELECT, OnPhotoUnselect)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_UNSELECT, OnUpdatePhotoUnselect)
	ON_COMMAND(IDM_FILM_SELECT, OnFilmSelect)
	ON_UPDATE_COMMAND_UI(IDM_FILM_UNSELECT, OnUpdateFilmUnselect)
	ON_UPDATE_COMMAND_UI(IDM_FILM_SELECT, OnUpdateFilmSelect)
	ON_COMMAND(IDM_FILM_UNSELECT, OnFilmUnselect)
	ON_COMMAND(IDM_PHOTO_SEARCH, OnPhotoSearch)
	ON_UPDATE_COMMAND_UI(IDM_PHOTO_SEARCH, OnUpdatePhotoSearch)
	ON_COMMAND(IDM_UNSELECT_ALL, OnUnselectAll)
	ON_UPDATE_COMMAND_UI(IDM_UNSELECT_ALL, OnUpdateUnselectAll)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlideListView construction/destruction

CSlideListView::CSlideListView()
{
	m_CurrentViewMode = VIEW_FILMS;
	m_pPrintingSupport = NULL;
}

CSlideListView::~CSlideListView()
{
	if ( m_pPrintingSupport )
	{
		delete m_pPrintingSupport;
		m_pPrintingSupport = NULL;
	}
}

BOOL CSlideListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Change the style of the tree view
	cs.style |= (TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT);

    return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSlideListView drawing

void CSlideListView::OnDraw(CDC* pDC)
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CSlideListView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

    if ( !GetTreeCtrl().GetImageList( TVSIL_NORMAL ) )
    {
        // Sort out the image list
        m_ImageList.Create( MAKEINTRESOURCE(IDB_VIEW_IMAGE_LIST), 16, 5, 0x00808000 );
        GetTreeCtrl().SetImageList( &m_ImageList, TVSIL_NORMAL );
    }

	// Load the film view to start with
	LoadFilmView();
}

void CSlideListView::LoadFilmView()
{
	// Load the film view from scratch
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_CurrentViewMode = VIEW_FILMS;

	// Remove existing contents, ensure nothing is selected before
	GetTreeCtrl().SelectItem( NULL );
	GetTreeCtrl().DeleteAllItems();

	POSITION FilmPos = pDoc->GetFilmListHead();
	while ( FilmPos )
	{
		CFilmDetails *pFilm = pDoc->GetNextFilm( FilmPos );
		ASSERT(pFilm);

		// Add this film to the view
        int Image = GetFilmImage( pFilm );
		HTREEITEM hFilmItem = GetTreeCtrl().InsertItem( pFilm->GetFilmSummary(), Image, Image, TVI_ROOT );

        // Store pointer to this film
        GetTreeCtrl().SetItemData( hFilmItem, (DWORD) pFilm );

		// Add the photos within the film to the view
		POSITION PhotoPos = pFilm->GetPhotoListHead();
		while ( PhotoPos )
		{
			CPhotoDetails *pPhoto = pFilm->GetNextPhoto( PhotoPos );
			ASSERT(pPhoto);

            int Image = GetPhotoImage( pPhoto );
			HTREEITEM hPhotoItem = GetTreeCtrl().InsertItem( pPhoto->GetPhotoSummary(), Image, Image, hFilmItem );

            // Store pointer to this photo
            GetTreeCtrl().SetItemData( hPhotoItem, (DWORD) pPhoto );
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// CSlideListView diagnostics

#ifdef _DEBUG
void CSlideListView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CSlideListView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CSlideListDoc* CSlideListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSlideListDoc)));
	return (CSlideListDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CSlideListView::IsFilmEntry( HTREEITEM hTreeItem )
{
    // Returns if the supplied item is a film
    BOOL bFilm = FALSE;

    if ( hTreeItem )
    {
        CObject *pObject = (CObject *) GetTreeCtrl().GetItemData( hTreeItem );
        if ( pObject )
        {
            if ( pObject->IsKindOf( RUNTIME_CLASS(CFilmDetails) ) )
            {
                bFilm = TRUE;
            }
        }
    }

    return( bFilm );
}

BOOL CSlideListView::IsPhotoEntry( HTREEITEM hTreeItem )
{
    // Returns if the supplied item is a photo
    BOOL bPhoto = FALSE;

    if ( hTreeItem )
    {
        CObject *pObject = (CObject *) GetTreeCtrl().GetItemData( hTreeItem );
        if ( pObject )
        {
            if ( pObject->IsKindOf( RUNTIME_CLASS(CPhotoDetails) ) )
            {
                bPhoto = TRUE;
            }
        }
    }

    return( bPhoto );
}

HTREEITEM CSlideListView::FindFilmItem( CFilmDetails *pFilm )
{
    HTREEITEM hItem = NULL;

    // Where we search depends on the current view mode
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        hItem = GetTreeCtrl().GetRootItem();
        while ( hItem )
        {
            if ( (CFilmDetails *) GetTreeCtrl().GetItemData( hItem ) == pFilm )
            {
                break;
            }
            hItem = GetTreeCtrl().GetNextSiblingItem( hItem );
        }
    }

    return( hItem );
}

int CSlideListView::GetFilmImage( CFilmDetails *pFilm )
{
    int Image = TREE_IMAGE_BLANK;

	// If ANY photos in this film are selected, then indicate this
	// in the icon for the film

	if ( pFilm->GetNumberSelectPhotos() > 0 )
	{
	    Image = TREE_IMAGE_SELECTED_BLANK;

		switch ( pFilm->GetType() )
		{
			case CFilmDetails::FILM_COLOUR_PRINT:
				Image = TREE_IMAGE_SELECTED_COLOUR_PRINT;
				break;
			case CFilmDetails::FILM_COLOUR_SLIDE:
				Image = TREE_IMAGE_SELECTED_COLOUR_SLIDE;
				break;
			case CFilmDetails::FILM_BW_PRINT:
				Image = TREE_IMAGE_SELECTED_BW_PRINT;
				break;
			case CFilmDetails::FILM_BW_SLIDE:
				Image = TREE_IMAGE_SELECTED_BW_SLIDE;
				break;
			case CFilmDetails::FILM_OTHER:
				Image = TREE_IMAGE_SELECTED_OTHER;
				break;
			default:
				ASSERT(0);
				break;
		}
	}
	else
	{
		switch ( pFilm->GetType() )
		{
			case CFilmDetails::FILM_COLOUR_PRINT:
				Image = TREE_IMAGE_COLOUR_PRINT;
				break;
			case CFilmDetails::FILM_COLOUR_SLIDE:
				Image = TREE_IMAGE_COLOUR_SLIDE;
				break;
			case CFilmDetails::FILM_BW_PRINT:
				Image = TREE_IMAGE_BW_PRINT;
				break;
			case CFilmDetails::FILM_BW_SLIDE:
				Image = TREE_IMAGE_BW_SLIDE;
				break;
			case CFilmDetails::FILM_OTHER:
				Image = TREE_IMAGE_OTHER;
				break;
			default:
				ASSERT(0);
				break;
		}
	}

    return( Image );
}

int CSlideListView::GetPhotoImage( CPhotoDetails *pPhoto )
{
    int Image = TREE_IMAGE_BLANK;

	// Selected image depends if the photo is selected

	if ( pPhoto->IsSelected() )
	{
	    Image = TREE_IMAGE_SELECTED_BLANK;

		if ( pPhoto->GetRating() == 1 )
		{
			Image = TREE_IMAGE_SELECTED_GRADE_1_PHOTO;
		}
		else if ( pPhoto->GetRating() == 2 )
		{
			Image = TREE_IMAGE_SELECTED_GRADE_2_PHOTO;
		}
	}
	else
	{
		if ( pPhoto->GetRating() == 1 )
		{
			Image = TREE_IMAGE_GRADE_1_PHOTO;
		}
		else if ( pPhoto->GetRating() == 2 )
		{
			Image = TREE_IMAGE_GRADE_2_PHOTO;
		}
	}

    return( Image );
}


/////////////////////////////////////////////////////////////////////////////
// CSlideListView message handlers

void CSlideListView::OnAddFilm()
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Capture new film details using dialog
	CFilmDetailsDialog FilmDetails( pDoc, TRUE );

    // Create sensible defaults
    FilmDetails.m_Year = (CTime::GetCurrentTime().GetYear()) - 2000;
    FilmDetails.m_Number = pDoc->GetNextFilmNumber( (WORD) FilmDetails.m_Year );
	FilmDetails.m_csDescription = _T("");
	FilmDetails.m_csNotes = _T("");
	FilmDetails.m_csMake.LoadString(IDS_DEFAULT_FILM_MAKE);
    FilmDetails.m_Size = CFilmDetails::SIZE_35MM;
	FilmDetails.m_Type = CFilmDetails::FILM_COLOUR_SLIDE;

	if ( FilmDetails.DoModal() == IDOK ) 
	{
		// User wishes to add a new film, create a new film class
        CFilmDetails *pNewFilm = (CFilmDetails *) new CFilmDetails();
        if ( pNewFilm )
        {
            pNewFilm->SetFilmNumber( (WORD) FilmDetails.m_Number );
            pNewFilm->SetFilmYear( (WORD) FilmDetails.m_Year );
            pNewFilm->SetDescription( FilmDetails.m_csDescription );
            pNewFilm->SetFilmMake( FilmDetails.m_csMake );
            pNewFilm->SetNotes( FilmDetails.m_csNotes );
            pNewFilm->SetSize( FilmDetails.m_Size );
            pNewFilm->SetType( FilmDetails.m_Type );

            // Get the document to add this film
            CFilmDetails *pPreviousFilm = NULL;
            pDoc->AddFilm( pNewFilm, pPreviousFilm );

            // Locate previous film
            HTREEITEM hFilmPrevious = TVI_LAST;
            if ( pPreviousFilm )
            {
                // Need to locate the item that matches this item
                hFilmPrevious = FindFilmItem( pPreviousFilm );
            }

            // Add film to the tree view
            int Image = GetFilmImage( pNewFilm );
    		HTREEITEM hFilmItem = GetTreeCtrl().InsertItem( pNewFilm->GetFilmSummary(), Image, Image, TVI_ROOT, hFilmPrevious );

            // Store pointer to this film
            GetTreeCtrl().SetItemData( hFilmItem, (DWORD) pNewFilm );

            // Ensure this is selected, ready for adding pictures
            GetTreeCtrl().SelectItem( hFilmItem );
            GetTreeCtrl().EnsureVisible( hFilmItem );

            // Prompt the user to see if the photos should be pre-created
            CFilmAddPhotosDialog AddPhotosDialog;
            // Default is 37 exposure
            AddPhotosDialog.m_Number = _T("36");
            if ( AddPhotosDialog.DoModal() == IDOK )
            {
                HTREEITEM hFirstPhoto = NULL;

				// User can force the photos to start at zero
				if ( AddPhotosDialog.m_StartAtZero )
				{
					pNewFilm->SetNextPhotoNumber( 0 );
				}

                // Add the required number of photos to this film
                WORD NumPhotos = (WORD) atol((LPCTSTR) AddPhotosDialog.m_Number);
                while ( NumPhotos )
                {
                    // Create the photo
                    CPhotoDetails *pNewPhoto = new CPhotoDetails();
                    pNewPhoto->SetPhotoNumber( (WORD) pNewFilm->GetNextPhotoNumber() );
                    pNewPhoto->SetDescription( _T("") );
                    pNewPhoto->SetLocation( _T("") );
                    pNewPhoto->SetKeywords( _T("") );
                    pNewPhoto->SetNotes( _T("") );
                    pNewPhoto->SetRating( 3 );

                    // Add it to the document and the view
                    pDoc->AddPhoto( pNewFilm, pNewPhoto );

                    // Add photo to the tree view
                    int Image = GetPhotoImage( pNewPhoto );
    		        HTREEITEM hPhotoItem = GetTreeCtrl().InsertItem( pNewPhoto->GetPhotoSummary(), Image, Image, hFilmItem );

                    // Store pointer to this photo
                    GetTreeCtrl().SetItemData( hPhotoItem, (DWORD) pNewPhoto );

                    // If this the first photo remember its possition
                    if ( hFirstPhoto == NULL )
                    {
                        hFirstPhoto = hPhotoItem;
                    }

                    // Next photo
                    NumPhotos -=1;
                }

                if ( hFirstPhoto )
                {
                    // Select the first photo graph
                    GetTreeCtrl().SelectItem( hFirstPhoto );
                    GetTreeCtrl().EnsureVisible( hFirstPhoto );
                }
            }
        }

		// Update all of the views
        pDoc->UpdateAllViews( this, CSlideListDoc::FILM_ADDED );
	}

}

void CSlideListView::OnUpdateAddFilm(CCmdUI* pCmdUI) 
{
    // Only possible to add a film in view film mode
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}


void CSlideListView::OnFilmChange() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Get point from currently selected film
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( hItem )
        {
            // If the parent of this item is null then this must be a film
            if ( IsFilmEntry( hItem ) )
            {
                CFilmDetails *pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
                ASSERT(pFilm);

                // Update existing film details using dialog
	            CFilmDetailsDialog FilmDetails( pDoc, FALSE );

                // Load the dialog with details of this file
                FilmDetails.m_Number = pFilm->GetFilmNumber();
	            FilmDetails.m_csDescription = pFilm->GetDescription();
	            FilmDetails.m_csNotes = pFilm->GetNotes();
	            FilmDetails.m_csMake = pFilm->GetFilmMake();
                FilmDetails.m_Year = pFilm->GetFilmYear();
                FilmDetails.m_Size = pFilm->GetSize();
	            FilmDetails.m_Type = pFilm->GetType();

	            if ( FilmDetails.DoModal() == IDOK ) 
	            {
                    // Update the film object
                    pFilm->SetFilmNumber( (WORD) FilmDetails.m_Number );
                    pFilm->SetFilmYear( (WORD) FilmDetails.m_Year );
                    pFilm->SetDescription( FilmDetails.m_csDescription );
                    pFilm->SetFilmMake( FilmDetails.m_csMake );
                    pFilm->SetNotes( FilmDetails.m_csNotes );
                    pFilm->SetSize( FilmDetails.m_Size );
                    pFilm->SetType( FilmDetails.m_Type );

                    // Inform the document of the change
                    pDoc->UpdateFilm( pFilm );

                    // Update the tree view
                    GetTreeCtrl().SetItemText( hItem, pFilm->GetFilmSummary() );
                    int Image = GetFilmImage( pFilm );
                    GetTreeCtrl().SetItemImage( hItem, Image, Image );

					// Update all of the views
                    pDoc->UpdateAllViews( this, CSlideListDoc::FILM_CHANGED );
                }
            }
        }
    }
}

void CSlideListView::OnUpdateFilmChange(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to change a film in view film mode if a film is selected
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( hItem )
        {
            // If the parent of this item is null then this must be a film
            if ( IsFilmEntry( hItem ) )
            {
                pCmdUI->Enable( TRUE );
            }
        }
    }
}


void CSlideListView::OnFilmRemove() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Get point from currently selected film
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( hItem )
        {
            // If the parent of this item is null then this must be a film
            if ( IsFilmEntry( hItem ) )
            {
                CFilmDetails *pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
                ASSERT(pFilm);

                CString csFilmRemoveCaption;
                csFilmRemoveCaption.LoadString( IDS_FILM_REMOVE_CAPTION );
                
                CString csFilmRemoveMessage;
                csFilmRemoveMessage.LoadString( IDS_FILM_REMOVE_MESSAGE );

	            if ( MessageBox( csFilmRemoveMessage, csFilmRemoveCaption, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES ) 
	            {
                    // Get the document to remove the film
                    pDoc->DeleteFilm( pFilm );

                    // Remove it from the view
                    GetTreeCtrl().DeleteItem( hItem );

					// Update all of the views
                    pDoc->UpdateAllViews( this, CSlideListDoc::FILM_DELETED );
                }
            }
        }
    }
}

void CSlideListView::OnUpdateFilmRemove(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to change a film in view film mode if a film is selected
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( hItem )
        {
            // If the parent of this item is null then this must be a film
            if ( IsFilmEntry( hItem ) )
            {
                pCmdUI->Enable( TRUE );
            }
        }
    }
}

void CSlideListView::OnAddPhoto() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Get the film that corresponds to the currently selected film or photo
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        CFilmDetails *pFilm = NULL;
        HTREEITEM hParentItem = NULL;
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( hItem )
        {
            // If the parent of this item is null then this must be a film
            if ( IsFilmEntry( hItem ) )
            {
                pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
                ASSERT(pFilm);
                hParentItem = hItem;
            }
            else
            {
                // Its photo that currently selected, so get its parent that the film
                hParentItem = GetTreeCtrl().GetParentItem( hItem );
                pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
                ASSERT(pFilm);
            }
        }

        // Got to film to which the photo should be added
        if ( pFilm )
        {
            // Capture new photo details using dialog
	        CPhotoDetailsDialog PhotoDetails( pDoc, TRUE );

            // Create sensible defaults
            PhotoDetails.m_Number = pFilm->GetNextPhotoNumber();
	        PhotoDetails.m_csDescription = _T("");
	        PhotoDetails.m_csNotes = _T("");
	        PhotoDetails.m_csLocation = _T("");
            PhotoDetails.m_csKeywords = _T("");
            PhotoDetails.m_Rating = 2;

            // Create default description if description is blank
            SetDefaultPhotoDescription( *pFilm, NULL, &PhotoDetails );

	        if ( PhotoDetails.DoModal() == IDOK )
            {
                CPhotoDetails *pNewPhoto = (CPhotoDetails *) new CPhotoDetails();
                if ( pNewPhoto )
                {
                    pNewPhoto->SetPhotoNumber( (WORD) PhotoDetails.m_Number );
                    pNewPhoto->SetDescription( PhotoDetails.m_csDescription );
                    pNewPhoto->SetLocation( PhotoDetails.m_csLocation );
                    pNewPhoto->SetKeywords( PhotoDetails.m_csKeywords );
                    pNewPhoto->SetNotes( PhotoDetails.m_csNotes );
                    pNewPhoto->SetRating( (WORD) PhotoDetails.m_Rating+1 );

                    // Get the document to add this photo
                    pDoc->AddPhoto( pFilm, pNewPhoto );

                    // Add photo to the tree view
                    int Image = GetPhotoImage( pNewPhoto );
    		        HTREEITEM hPhotoItem = GetTreeCtrl().InsertItem( pNewPhoto->GetPhotoSummary(), Image, Image, hParentItem );

                    // Store pointer to this photo
                    GetTreeCtrl().SetItemData( hPhotoItem, (DWORD) pNewPhoto );

                    // Ensure this is selected, ready for adding pictures
                    GetTreeCtrl().SelectItem( hPhotoItem );
                    GetTreeCtrl().EnsureVisible( hPhotoItem );
                }
				// Update all of the views
                pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_ADDED );
            }
        }
    }
}

void CSlideListView::OnUpdateAddPhoto(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to add a photo if a film or photo is selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
            if ( IsFilmEntry( hItem ) || IsPhotoEntry( hItem ) )
        {
            pCmdUI->Enable( TRUE );
        }
    }
}

void CSlideListView::OnChangePhoto() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Get the film that corresponds to the currently selected film or photo
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        CFilmDetails *pFilm = NULL;
        CPhotoDetails *pPhoto = NULL;
        HTREEITEM hParentItem = NULL;
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
            // Its photo that currently selected, so get its parent that the film
            hParentItem = GetTreeCtrl().GetParentItem( hItem );
            if ( hParentItem )
            {
                pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
                ASSERT(pFilm);

                pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
                ASSERT(pPhoto);
            }
        }

        // Got the film and photo
        if ( pFilm && pPhoto )
        {
            // Capture photo details using dialog
	        CPhotoDetailsDialog PhotoDetails( pDoc, FALSE );

            // Create sensible defaults
            PhotoDetails.m_Number = pPhoto->GetPhotoNumber();
	        PhotoDetails.m_csDescription = pPhoto->GetDescription();
	        PhotoDetails.m_csNotes = pPhoto->GetNotes();
	        PhotoDetails.m_csLocation = pPhoto->GetLocation();
            PhotoDetails.m_csKeywords = pPhoto->GetKeywords();
            PhotoDetails.m_Rating = pPhoto->GetRating()-1;

            // Create default description if description is blank
            SetDefaultPhotoDescription( *pFilm, pPhoto, &PhotoDetails );

	        if ( PhotoDetails.DoModal() == IDOK )
            {
                // Update the photo details
                pPhoto->SetPhotoNumber( (WORD) PhotoDetails.m_Number );
                pPhoto->SetDescription( PhotoDetails.m_csDescription );
                pPhoto->SetLocation( PhotoDetails.m_csLocation );
                pPhoto->SetKeywords( PhotoDetails.m_csKeywords );
                pPhoto->SetNotes( PhotoDetails.m_csNotes );
                pPhoto->SetRating( (WORD) PhotoDetails.m_Rating+1 );

                // Get the document to add this photo
                pDoc->UpdatePhoto( pFilm, pPhoto );

                // Update the tree view
                GetTreeCtrl().SetItemText( hItem, pPhoto->GetPhotoSummary() );
                int Image = GetPhotoImage( pPhoto );
                GetTreeCtrl().SetItemImage( hItem, Image, Image );

				// Update all of the views
                pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_CHANGED );
            }
        }
    }
}

void CSlideListView::OnUpdateChangePhoto(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to update a photo if a photo is selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
            pCmdUI->Enable( TRUE );
        }
    }
}

void CSlideListView::OnRemovePhoto() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // Get the film that corresponds to the currently selected film or photo
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        CFilmDetails *pFilm = NULL;
        CPhotoDetails *pPhoto = NULL;
        HTREEITEM hParentItem = NULL;
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
            // Its photo that currently selected, so get its parent that the film
            hParentItem = GetTreeCtrl().GetParentItem( hItem );
            if ( hParentItem )
            {
                pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
                ASSERT(pFilm);

                pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
                ASSERT(pPhoto);
            }
        }

        // Got the film and photo
        if ( pFilm && pPhoto )
        {
            CString csPhotoRemoveCaption;
            csPhotoRemoveCaption.LoadString( IDS_PHOTO_REMOVE_CAPTION );
            
            CString csPhotoRemoveMessage;
            csPhotoRemoveMessage.LoadString( IDS_PHOTO_REMOVE_MESSAGE );

	        if ( MessageBox( csPhotoRemoveMessage, csPhotoRemoveCaption, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES ) 
	        {
                // Remove this photo
                pDoc->DeletePhoto( pFilm, pPhoto );

                // Remove it from the view
                GetTreeCtrl().DeleteItem( hItem );

				// Update all of the views
                pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_DELETED );
            }
        }
    }
}

void CSlideListView::OnUpdateRemovePhoto(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to delete a photo if a photo is selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
            pCmdUI->Enable( TRUE );
        }
    }
}

void CSlideListView::OnPhotoSelect() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // To be a photo it must have a parent
    HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM hParentItem = GetTreeCtrl().GetParentItem( hItem );
    if ( IsPhotoEntry( hItem ) )
    {
		// It's a photo item, now is it currently unselected ?
	    CPhotoDetails *pPhoto = NULL;
        pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
        ASSERT(pPhoto);

		if ( !pPhoto->IsSelected() )
		{
			// Tell the document to select this photo
			pDoc->SelectPhoto( pPhoto );

			// Update the image for this photo
            int Image = GetPhotoImage( pPhoto );
            GetTreeCtrl().SetItemImage( hItem, Image, Image );

			// Update the film image incase it needs to change
	        CFilmDetails *pFilm = NULL;
            pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
            ASSERT(pFilm);

			Image = GetFilmImage( pFilm );
            GetTreeCtrl().SetItemImage( hParentItem, Image, Image );

			// Update all of the views
            pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_SELECTED );
		}
    }
}

void CSlideListView::OnUpdatePhotoSelect(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to select/unselect a photo if a photo is selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
			// It's a photo item, now is it currently unselected ?
	        CPhotoDetails *pPhoto = NULL;
            pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
            ASSERT(pPhoto);

			if ( !pPhoto->IsSelected() )
			{
			    pCmdUI->Enable( TRUE );
			}
        }
    }
}

void CSlideListView::OnPhotoUnselect() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    // To be a photo it must have a parent
    HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM hParentItem = GetTreeCtrl().GetParentItem( hItem );
    if ( IsPhotoEntry( hItem ) )
    {
		// It's a photo item, now is it currently selected ?
	    CPhotoDetails *pPhoto = NULL;
        pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
        ASSERT(pPhoto);

		if ( pPhoto->IsSelected() )
		{
			// Tell the document to unselect this photo
			pDoc->UnselectPhoto( pPhoto );

			// Update the image for this photo
            int Image = GetPhotoImage( pPhoto );
            GetTreeCtrl().SetItemImage( hItem, Image, Image );

			// Update the film image incase it needs to change
	        CFilmDetails *pFilm = NULL;
            pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
            ASSERT(pFilm);

			Image = GetFilmImage( pFilm );
            GetTreeCtrl().SetItemImage( hParentItem, Image, Image );

			// Update all of the views
            pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_UNSELECTED );
		}
    }
}

void CSlideListView::OnUpdatePhotoUnselect(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to select/unselect a photo if a photo is selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
			// It's a photo item, now is it currently selected ?
	        CPhotoDetails *pPhoto = NULL;
            pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
            ASSERT(pPhoto);

			if ( pPhoto->IsSelected() )
			{
			    pCmdUI->Enable( TRUE );
			}
        }
    }
}

void CSlideListView::OnFilmSelect() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
    if ( IsFilmEntry( hItem ) )
    {
		// It's a film item 
	    CFilmDetails *pFilm = NULL;
        pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
        ASSERT(pFilm);

		// Tell the document to select all photos in this film
		pDoc->SelectAllPhotosInFilm( pFilm );

		// Update all images for this and its children
		UpdateImages( hItem );

		// Update all of the views
        pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_SELECTED );
    }
}

void CSlideListView::OnUpdateFilmSelect(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to select all photos in a film if film is currently selected
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsFilmEntry( hItem ) )
        {
			// It's a film item
	        CFilmDetails *pFilm = NULL;
            pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
            ASSERT(pFilm);

			// Number currently selected must be less than the number of photos in the film
			if ( pFilm->GetNumberSelectPhotos() < pFilm->GetNumberPhotos() )
			{
			    pCmdUI->Enable( TRUE );
			}
        }
    }
}

void CSlideListView::OnFilmUnselect() 
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Should only happen in film mode
    ASSERT(m_CurrentViewMode == VIEW_FILMS);

    HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
    if ( IsFilmEntry( hItem ) )
    {
		// It's a film item 
	    CFilmDetails *pFilm = NULL;
        pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
        ASSERT(pFilm);

		// Tell the document to unselect all photos in this film
		pDoc->UnselectAllPhotosInFilm( pFilm );

		// Update all images for this and its children
		UpdateImages( hItem );

		// Update all of the views
        pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_UNSELECTED );
    }
}

void CSlideListView::OnUpdateFilmUnselect(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to unselect all photos in a film if film is currently selected
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsFilmEntry( hItem ) )
        {
			// It's a film item
	        CFilmDetails *pFilm = NULL;
            pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
            ASSERT(pFilm);

			// Number currently selected > 0 to make unselect available
			if ( pFilm->GetNumberSelectPhotos() > 0 )
			{
			    pCmdUI->Enable( TRUE );
			}
        }
    }
}

void CSlideListView::OnUnselectAll() 
{
    // Only possible to unselect all photos if there are some selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
	    CSlideListDoc* pDoc = GetDocument();
	    ASSERT_VALID(pDoc);

        if ( pDoc->GetSelectedPhotoCount() > 0 )
        {
            pDoc->UnselectAllPhotos();

            // Update all images
		    UpdateAllImages();

		    // Update all of the views
            pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_UNSELECTED );
        }
    }
}

void CSlideListView::OnUpdateUnselectAll(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable( FALSE );

    // Only possible to unselect all photos if there are some selected.
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
	    CSlideListDoc* pDoc = GetDocument();
	    ASSERT_VALID(pDoc);

        if ( pDoc->GetSelectedPhotoCount() > 0 )
        {
            pCmdUI->Enable( TRUE );
        }
    }
}

void CSlideListView::OnPhotoSearch() 
{
	// Perform the search operation
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Create and display the search dialog to start with
	CPhotoSearchDialog SearchDialog( pDoc );

	if ( SearchDialog.DoModal() == IDOK )
	{
        CSlideListDoc::SearchMode Mode = CSlideListDoc::CLEAR_SELECTION_BEFORE;

        if ( SearchDialog.m_SelectionMode == 1 )
        {
            Mode = CSlideListDoc::ADD_EXISTING_SELECTION;
        }
        else if ( SearchDialog.m_SelectionMode == 2 )
        {
            Mode = CSlideListDoc::SEARCH_WITHIN_SELECTION;
        }

        // Get the document to perform the search
        long int NumberFound = pDoc->DoPhotographSearch( SearchDialog.m_SearchString, SearchDialog.m_bKeyword, SearchDialog.m_bDescription, SearchDialog.m_bLocation, SearchDialog.m_SearchRating, Mode );

        // Tell the user if nothing was found
        if ( NumberFound == 0 )
        {
            AfxMessageBox( "No photographs found.", MB_OK | MB_ICONINFORMATION );
        }

		// Update all images
		UpdateAllImages();

		// Update all of the views
        pDoc->UpdateAllViews( this, CSlideListDoc::PHOTO_SELECTED );
	}

}

void CSlideListView::OnUpdatePhotoSearch(CCmdUI* pCmdUI) 
{
	// Only enable searching if there are some films available
    pCmdUI->Enable( FALSE );

    // Only possible to unselect all photos in a film if film is currently selected
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
		CSlideListDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if ( pDoc->GetNumberOfFilms() > 0 )
		{
			pCmdUI->Enable( TRUE );
		}
	}
}

void CSlideListView::SelectPhotoUsingId( LPCTSTR pPhotoId )
{
	// Extract the film year, film number and photo number from the id
	WORD Year = 0;
	WORD FilmNum = 0;
	WORD PhotoNum = 0;

	CPhotoDetails::GetFilmAndPhotoId( pPhotoId, &Year, &FilmNum, &PhotoNum );

	// Search the tree view looking for the film and photo
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
		CFilmDetails *pFilm = NULL;
		CPhotoDetails *pPhoto = NULL;
        HTREEITEM hItem = GetTreeCtrl().GetRootItem();

		while ( hItem )
		{
			// Does this match ?
			pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
			ASSERT(pFilm);
			if ( (pFilm->GetFilmYear() == Year) && (pFilm->GetFilmNumber() == FilmNum) )
			{
				// Found the film, no try and locate the photo
				hItem = GetTreeCtrl().GetChildItem( hItem );
				while ( hItem )
				{
					pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
					ASSERT(pPhoto);
					if ( pPhoto->GetPhotoNumber() == PhotoNum )
					{
						// Found the matching photo, select this item
						GetTreeCtrl().Select( hItem, TVGN_CARET );

						// Finished
						break;
					}
					else
					{
						// Try next photo
						hItem = GetTreeCtrl().GetNextSiblingItem( hItem );
					}
				}
				// Finshed
				break;
			}
			else
			{
				// Try next film
				hItem = GetTreeCtrl().GetNextSiblingItem( hItem );
			}
		}
	}
}

void CSlideListView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Tree select changed
    // Allow change of film or photo, depending on
    // current selection

    // Default allow normal processing
    *pResult = 0;

    // FILM view mode processing
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
		CFilmDetails *pFilm = NULL;
		CPhotoDetails *pPhoto = NULL;
		HTREEITEM hParentItem = NULL;
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
			// Inform the document that a photo as been selected
			if ( IsPhotoEntry( hItem ) )
			{
				// Its photo that currently selected, so get its parent that the film
				hParentItem = GetTreeCtrl().GetParentItem( hItem );
				if ( hParentItem )
				{
					pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hParentItem );
					ASSERT(pFilm);

					pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hItem );
					ASSERT(pPhoto);
				}
			}
        }
        else if ( IsFilmEntry( hItem ) )
        {
			// Inform the document that a film as been selected
			pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hItem );
			ASSERT(pFilm);
        }

		// Supply the document with information about what's selected
		CSlideListDoc* pDoc = GetDocument();
		pDoc->SelectionChange( pFilm, pPhoto );

		// Update the views
        pDoc->UpdateAllViews( this, CSlideListDoc::TREE_SELECTION_CHANGE );
    }
}

void CSlideListView::UpdateImages( HTREEITEM hStartingItem )
{
	// This function updates the images for all films and photos starting
	// with the supplied item and all of its children
	//
	// It is recursive

	// Update the supplied item
	if ( IsPhotoEntry( hStartingItem ) )
	{
		CPhotoDetails *pPhoto = NULL;
		pPhoto = (CPhotoDetails *) GetTreeCtrl().GetItemData( hStartingItem );
		ASSERT(pPhoto);
		int Image = GetPhotoImage( pPhoto );
		GetTreeCtrl().SetItemImage( hStartingItem, Image, Image );
	}
	else if ( IsFilmEntry( hStartingItem ) )
	{
		CFilmDetails *pFilm = NULL;
		pFilm = (CFilmDetails *) GetTreeCtrl().GetItemData( hStartingItem );
		ASSERT(pFilm);
		int Image = GetFilmImage( pFilm );
		GetTreeCtrl().SetItemImage( hStartingItem, Image, Image );
	}

    // Update all of child items of the starting item
	HTREEITEM hItem = NULL;
	hItem = GetTreeCtrl().GetChildItem( hStartingItem );
	while ( hItem )
	{
		UpdateImages( hItem );

		// Get the next child
		hItem = GetTreeCtrl().GetNextSiblingItem( hItem );
	}
}

void CSlideListView::UpdateAllImages( void )
{
    // Update images for all items in the tree view

    // Get the root item, update it and all its siblings
    HTREEITEM hItem = GetTreeCtrl().GetRootItem();
	while ( hItem )
	{
		UpdateImages( hItem );

		// Get the next child
		hItem = GetTreeCtrl().GetNextSiblingItem( hItem );
	}
}

void CSlideListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Tree view double clicked
    // Allow change of film or photo, depending on
    // current selection

    // Default allow normal processing
    *pResult = 0;

    // FILM view mode processing
    if ( m_CurrentViewMode == VIEW_FILMS )
    {
        // To be a photo it must have a parent
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
        if ( IsPhotoEntry( hItem ) )
        {
            // This is a photo, allow update
            OnChangePhoto();

            // prevent normal processing
            *pResult = 1;
        }
        else if ( IsFilmEntry( hItem ) )
        {
            // No parent, its film
            OnFilmChange();

            // prevent normal processing
            *pResult = 1;
        }
    }
}

void CSlideListView::SetDefaultPhotoDescription( CFilmDetails& Film, CPhotoDetails *pPhoto, CPhotoDetailsDialog *pPhotoDetailsDialog )
{
    // if the description is blank use the description from the previous photo
    // in THIS film

    CPhotoDetails *pPreviousPhoto = NULL;

    // Does the photo already exist ?
    if ( pPhoto )
    {
        if ( (pPhoto->GetDescription() == _T("")) && (pPhoto->GetPhotoNumber() > 1) )
        {
            pPreviousPhoto = Film.GetPhotoDetails( pPhoto->GetPhotoNumber() - 1 );
        }
    }
    else
    {
        // New photo, assume the dialog contains the new number
        pPreviousPhoto = Film.GetPhotoDetails( (WORD) pPhotoDetailsDialog->m_Number - 1 );
    }

    // Get the actual defaults
    if ( pPreviousPhoto )
    {
        pPhotoDetailsDialog->m_csDescription = pPreviousPhoto->GetDescription();
        pPhotoDetailsDialog->m_csLocation = pPreviousPhoto->GetLocation();
    }
}




/////////////////////////////////////////////////////////////////////////////
// CSlideListView Printing Support Function

BOOL CSlideListView::OnPreparePrinting( CPrintInfo* pInfo )
{
	// Create support object for the printing, the actual printing 
	// is delagated to this object

	BOOL bPrintProcede = FALSE;

	ASSERT(m_pPrintingSupport==NULL);

	TCHAR PathBuffer[1024];
	GetModuleFileName( NULL, PathBuffer, sizeof(PathBuffer) );

	// Generate output file name
	CString csReportFile = PathBuffer;
	csReportFile = csReportFile.Left( csReportFile.ReverseFind('\\')+1 );
	csReportFile += REPORT_FILE;

	if ( m_CurrentViewMode == VIEW_FILMS )
	{
		m_pPrintingSupport = new CPrintingSupport( GetDocument(), csReportFile );
	}

	// Got some thing to delagate printing too ?
	if ( m_pPrintingSupport )
	{

		bPrintProcede = m_pPrintingSupport->OnPreparePrinting( pInfo );
		if ( bPrintProcede )
		{
			bPrintProcede = DoPreparePrinting( pInfo );
		}

		// If not allowed to procede, remove printing support object
		if ( !bPrintProcede )
		{
			delete m_pPrintingSupport;
			m_pPrintingSupport = NULL;
		}
	}
	else
	{
		// No support available for printing the current view mode
		bPrintProcede = FALSE;
	}

	return( bPrintProcede );
}

void CSlideListView::OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo )
{
	ASSERT(m_pPrintingSupport);

	m_pPrintingSupport->OnBeginPrinting( pDC, pInfo );
}

void CSlideListView::OnPrepareDC( CDC* pDC, CPrintInfo* pInfo ) 
{
	ASSERT(m_pPrintingSupport);

	CView::OnPrepareDC(pDC, pInfo);

	m_pPrintingSupport->OnPrepareDC( pDC, pInfo );
}				

void CSlideListView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pPrintingSupport);

	m_pPrintingSupport->OnPrint( pDC, pInfo );
}

void CSlideListView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo )
{
	if ( m_pPrintingSupport )
	{
		m_pPrintingSupport->OnEndPrinting( pDC, pInfo );

		// Delete the printing support
		delete m_pPrintingSupport;
		m_pPrintingSupport = NULL;
	}
}



void CSlideListView::OnFilePrint() 
{
	if ( GenerateReport() )
	{
		CView::OnFilePrint();
	}
}

void CSlideListView::OnFilePrintPreview() 
{
	if ( GenerateReport() )
	{
		CView::OnFilePrintPreview();
	}
}

BOOL CSlideListView::GenerateReport( void )
{
	CSlideListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bGenerated = FALSE;

	CReportSelection ReportDialog;

	// Only allow selective printing if there are some selected
	if ( pDoc->GetSelectedPhotoCount() > 0 )
	{
		ReportDialog.m_bAllowSelectedOnly = TRUE;
		ReportDialog.m_SelectedOnly = TRUE;
	}
	else
	{
		ReportDialog.m_bAllowSelectedOnly = FALSE;
		ReportDialog.m_SelectedOnly = FALSE;
	}

	if ( ReportDialog.DoModal() == IDOK )
	{
		// Report data and report files are generated in the same directory as this program
		TCHAR PathBuffer[1024];
		GetModuleFileName( NULL, PathBuffer, sizeof(PathBuffer) );

		// Generate data file name
		CString csReportDataFile = PathBuffer;
		csReportDataFile = csReportDataFile.Left( csReportDataFile.ReverseFind('\\')+1 );
		csReportDataFile += REPORT_DATA;

		// Generate output file name
		CString csReportFile = PathBuffer;
		csReportFile = csReportFile.Left( csReportFile.ReverseFind('\\')+1 );
		csReportFile += REPORT_FILE;

		// Delete data and report files
		DeleteFile( csReportDataFile );
		DeleteFile( csReportFile );

		// Generate the data file
		GetDocument()->GenerateDataFile( csReportDataFile, ReportDialog.m_SelectedOnly );

		// Run Report
		CString csReportCommand = "perl ";
		csReportCommand += ReportDialog.GetReportPathName();
		csReportCommand += " ";
		csReportCommand += csReportDataFile;
		csReportCommand += " ";
		csReportCommand += csReportFile;

		PROCESS_INFORMATION ProcInfo = { 0 };
		STARTUPINFO StartUp = { 0 };
		StartUp.cb = sizeof(StartUp);
		StartUp.dwFlags = STARTF_USESHOWWINDOW;
		StartUp.wShowWindow = SW_HIDE;

		if ( CreateProcess( NULL, (LPTSTR) (LPCTSTR) csReportCommand, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartUp, &ProcInfo ) )
		{
			// Wait for the report to stop running, a maximum of 60 seconds
			WaitForSingleObject( ProcInfo.hProcess, 60000 );

			// Close the handles
			CloseHandle( ProcInfo.hThread );
			CloseHandle( ProcInfo.hProcess );
		}

		bGenerated = TRUE;
	}

	return ( bGenerated );
}


