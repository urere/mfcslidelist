; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPhotoDetailsDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SlideList.h"
LastPage=0

ClassCount=13
Class1=CSlideListApp
Class2=CSlideListDoc
Class3=CSlideListView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDR_MAINFRAME (English (U.S.))
Resource4=IDD_CREATE_PHOTOS
Class6=CFilmDetailsDialog
Resource5=IDD_PHOTO_DETAILS
Class7=CPhotoDetailsDialog
Resource6=IDD_FILM_DETAILS
Class8=CFilmAddPhotosDialog
Resource7=IDD_ABOUTBOX (English (U.S.))
Class9=CReportSelection
Class10=CDetailView
Class11=CSelectedPhotoView
Class12=CExScrollView
Resource8=IDD_REPORT_SELECTION
Class13=CPhotoSearchDialog
Resource9=IDD_SEARCH_DIALOG

[CLS:CSlideListApp]
Type=0
HeaderFile=SlideList.h
ImplementationFile=SlideList.cpp
Filter=N

[CLS:CSlideListDoc]
Type=0
HeaderFile=SlideListDoc.h
ImplementationFile=SlideListDoc.cpp
Filter=N
LastObject=CSlideListDoc

[CLS:CSlideListView]
Type=0
HeaderFile=SlideListView.h
ImplementationFile=SlideListView.cpp
Filter=C
BaseClass=CTreeView
VirtualFilter=VWC
LastObject=IDM_UNSELECT_ALL


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=SlideList.cpp
ImplementationFile=SlideList.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
CommandCount=16
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command16=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_PRINT
Command4=IDM_FILM_CHANGE
Command5=IDM_ADD_FILM
Command6=IDM_FILM_REMOVE
Command7=IDM_PHOTO_CHANGE
Command8=IDM_PHOTO_ADD
Command9=IDM_PHOTO_REMOVE
Command10=IDM_PHOTO_SELECT
Command11=IDM_PHOTO_UNSELECT
Command12=IDM_PHOTO_SEARCH
CommandCount=12

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=IDM_ADD_FILM
Command11=IDM_FILM_CHANGE
Command12=IDM_FILM_REMOVE
Command13=IDM_PHOTO_ADD
Command14=IDM_PHOTO_CHANGE
Command15=IDM_PHOTO_REMOVE
Command16=IDM_PHOTO_SEARCH
Command17=IDM_PHOTO_SELECT
Command18=IDM_PHOTO_UNSELECT
Command19=IDM_FILM_SELECT
Command20=IDM_FILM_UNSELECT
Command21=IDM_UNSELECT_ALL
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FILM_DETAILS]
Type=1
Class=CFilmDetailsDialog
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FILM_YEAR,edit,1350639744
Control3=IDC_STATIC,static,1342177280
Control4=IDC_FILM_NUMBER,edit,1350639744
Control5=IDC_STATIC,static,1342177280
Control6=IDC_FILE_DESCRIPTION,edit,1350631552
Control7=IDC_STATIC,static,1342177280
Control8=IDC_FILE_NOTES,edit,1350631428
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_FILM_MAKE,combobox,1344340226
Control12=IDC_STATIC,static,1342177280
Control13=IDC_FILM_TYPE,combobox,1344339971
Control14=IDC_STATIC,static,1342177280
Control15=IDC_FILM_SIZE,combobox,1344339971
Control16=IDOK,button,1342373889
Control17=IDCANCEL,button,1342242816

[CLS:CFilmDetailsDialog]
Type=0
HeaderFile=FilmDetailsDialog.h
ImplementationFile=FilmDetailsDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CFilmDetailsDialog
VirtualFilter=dWC

[DLG:IDD_PHOTO_DETAILS]
Type=1
Class=CPhotoDetailsDialog
ControlCount=15
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PHOTO_NUMBER,edit,1484859520
Control3=IDC_STATIC,static,1342177280
Control4=IDC_RATING_LIST,combobox,1344339971
Control5=IDC_STATIC,static,1342177280
Control6=IDC_FILE_DESCRIPTION,edit,1350631552
Control7=IDC_STATIC,static,1342177280
Control8=IDC_PHOTO_LOCATION,edit,1350631552
Control9=IDC_STATIC,static,1342177280
Control10=IDC_PHOTO_KEYWORDS,edit,1350631552
Control11=IDC_STATIC,static,1342177280
Control12=IDC_FILE_NOTES,edit,1350631428
Control13=IDOK,button,1342373889
Control14=IDCANCEL,button,1342242816
Control15=IDC_KEYWORD_LIST,listbox,1353712387

[CLS:CPhotoDetailsDialog]
Type=0
HeaderFile=PhotoDetailsDialog.h
ImplementationFile=PhotoDetailsDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PHOTO_NUMBER

[DLG:IDD_CREATE_PHOTOS]
Type=1
Class=CFilmAddPhotosDialog
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NUMBER_PHOTOS,combobox,1344339971
Control3=IDC_STATIC,static,1342308352
Control4=IDC_START_AT_ZERO,button,1342242851
Control5=IDOK,button,1342373889
Control6=IDCANCEL,button,1342242816

[CLS:CFilmAddPhotosDialog]
Type=0
HeaderFile=filmaddphotos.h
ImplementationFile=filmaddphotos.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CFilmAddPhotosDialog

[DLG:IDD_REPORT_SELECTION]
Type=1
Class=CReportSelection
ControlCount=4
Control1=IDC_REPORT_LIST,listbox,1352728835
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_SELECTED_PHOTOS,button,1342251267

[CLS:CReportSelection]
Type=0
HeaderFile=ReportSelection.h
ImplementationFile=ReportSelection.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CReportSelection

[CLS:CDetailView]
Type=0
HeaderFile=DetailView.h
ImplementationFile=DetailView.cpp
BaseClass=CScrollView
Filter=C
LastObject=CDetailView

[CLS:CSelectedPhotoView]
Type=0
HeaderFile=SelectedPhotoView.h
ImplementationFile=SelectedPhotoView.cpp
BaseClass=CScrollView
Filter=C

[CLS:CExScrollView]
Type=0
HeaderFile=ExScrollView.h
ImplementationFile=ExScrollView.cpp
BaseClass=CScrollView
Filter=C

[DLG:IDD_SEARCH_DIALOG]
Type=1
Class=CPhotoSearchDialog
ControlCount=15
Control1=IDC_STATIC,button,1342177287
Control2=IDC_SEARCH_STRING,edit,1350631552
Control3=IDC_SEARCH_KEYWORD_LIST,listbox,1353777923
Control4=IDC_STATIC,button,1342177287
Control5=IDC_LOCATION_SEARCH,button,1342373891
Control6=IDC_DESCRIPTION_SEARCH,button,1342242819
Control7=IDC_KEYWORD_SEARCH,button,1342242819
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SEARCH_RATING,combobox,1344339971
Control10=IDC_STATIC,button,1342177287
Control11=IDC_SEARCH_CLEAR_EXISTING,button,1342373897
Control12=IDC_SEARCH_ADD_EXISTING,button,1342242825
Control13=IDC_SEARCH_WITHIN,button,1342242825
Control14=IDOK,button,1342373889
Control15=IDCANCEL,button,1342242816

[CLS:CPhotoSearchDialog]
Type=0
HeaderFile=PhotoSearchDialog.h
ImplementationFile=PhotoSearchDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SEARCH_RATING

