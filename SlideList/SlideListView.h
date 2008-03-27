// SlideListView.h : interface of the CSlideListView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIDELISTVIEW_H__166515ED_0F3E_4B91_8A97_66DA0E8C3FC2__INCLUDED_)
#define AFX_SLIDELISTVIEW_H__166515ED_0F3E_4B91_8A97_66DA0E8C3FC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPhotoDetailsDialog;
class CPrintingSupport;

class CSlideListView : public CTreeView
{
protected: // create from serialization only
	CSlideListView();
	DECLARE_DYNCREATE(CSlideListView)

// Attributes
public:
	CSlideListDoc* GetDocument();

// Operations
public:
	void SelectPhotoUsingId( LPCTSTR pPhotoId );
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideListView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSlideListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSlideListView)
	afx_msg void OnAddFilm();
	afx_msg void OnFilmChange();
	afx_msg void OnFilmRemove();
	afx_msg void OnUpdateAddFilm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilmChange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilmRemove(CCmdUI* pCmdUI);
	afx_msg void OnAddPhoto();
	afx_msg void OnChangePhoto();
	afx_msg void OnRemovePhoto();
	afx_msg void OnUpdateAddPhoto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChangePhoto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemovePhoto(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPhotoSelect();
	afx_msg void OnUpdatePhotoSelect(CCmdUI* pCmdUI);
	afx_msg void OnPhotoUnselect();
	afx_msg void OnUpdatePhotoUnselect(CCmdUI* pCmdUI);
	afx_msg void OnFilmSelect();
	afx_msg void OnUpdateFilmUnselect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilmSelect(CCmdUI* pCmdUI);
	afx_msg void OnFilmUnselect();
	afx_msg void OnPhotoSearch();
	afx_msg void OnUpdatePhotoSearch(CCmdUI* pCmdUI);
	afx_msg void OnUnselectAll();
	afx_msg void OnUpdateUnselectAll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Private constants etc
private:
	enum ViewMode
	{
		VIEW_FILMS = 1,
		VIEW_KEYWORDS,
		VIEW_SEARCH
	};
    enum TreeImages
    {
        TREE_IMAGE_BLANK = 0,
        TREE_IMAGE_COLOUR_PRINT,
        TREE_IMAGE_COLOUR_SLIDE,
        TREE_IMAGE_BW_PRINT,
        TREE_IMAGE_BW_SLIDE,
        TREE_IMAGE_OTHER,
        TREE_IMAGE_GRADE_1_PHOTO,
        TREE_IMAGE_GRADE_2_PHOTO,
        TREE_IMAGE_SELECTED_BLANK = 10,
        TREE_IMAGE_SELECTED_COLOUR_PRINT,
        TREE_IMAGE_SELECTED_COLOUR_SLIDE,
        TREE_IMAGE_SELECTED_BW_PRINT,
        TREE_IMAGE_SELECTED_BW_SLIDE,
        TREE_IMAGE_SELECTED_OTHER,
        TREE_IMAGE_SELECTED_GRADE_1_PHOTO,
        TREE_IMAGE_SELECTED_GRADE_2_PHOTO,
    };

// Attributes
private:
	ViewMode			m_CurrentViewMode;
    CImageList			m_ImageList;
	CPrintingSupport   *m_pPrintingSupport;

// Private helper functions
private:
	void LoadFilmView( void );
    BOOL IsFilmEntry( HTREEITEM hTreeItem );
    BOOL IsPhotoEntry( HTREEITEM hTreeItem );
    HTREEITEM FindFilmItem( CFilmDetails *pFilm );
    int GetFilmImage( CFilmDetails *pFilm );
    int GetPhotoImage( CPhotoDetails *pPhoto );
	void UpdateImages( HTREEITEM hStartingItem );
	void UpdateAllImages( void );
    void SetDefaultPhotoDescription( CFilmDetails& Film, CPhotoDetails *pPhoto, CPhotoDetailsDialog *pPhotoDetailsDialog );
	BOOL GenerateReport( void );
};

#ifndef _DEBUG  // debug version in SlideListView.cpp
inline CSlideListDoc* CSlideListView::GetDocument()
   { return (CSlideListDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDELISTVIEW_H__166515ED_0F3E_4B91_8A97_66DA0E8C3FC2__INCLUDED_)
