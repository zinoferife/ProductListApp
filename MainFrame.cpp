#include "MainFrame.h"
#include "Application.h"
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
	EVT_SIZE(MainFrame::OnSize)
	EVT_MENU(wxID_NEW, MainFrame::OnNew)
	EVT_MENU(wxID_OPEN, MainFrame::OnLoad)
	EVT_MENU(wxID_SAVE, MainFrame::OnSave)

END_EVENT_TABLE()



MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size)
:wxFrame(parent,id , wxT("Dglopa product list"), position,size){
	//do initalize of system
	mFrameManager = new wxAuiManager(this);
	wxArtProvider::Push(new ArtProvider);
	if (!InitCreation())
	{
		wxMessageBox("Cannot create application, critical error", "ERROR MESSAGE", wxICON_ERROR | wxOK);
		isCreated = false;
	}

	isCreated = true;
}

MainFrame::~MainFrame()
{
	if (mFrameManager)
	{
		mFrameManager->UnInit();
		delete mFrameManager;
	}
}

bool MainFrame::InitCreation()
{
	CreateMenuBar();
	CreateProductList();


	return true;
}

void MainFrame::CreateToolBar()
{
}

void MainFrame::CreateMenuBar()
{
	wxMenuBar* menubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_NEW, "New\tCtrl-N");
	file->Append(wxID_SAVE, "Save\tCtrl-S");
	file->Append(wxID_OPEN, "Open\tCtrl-O");

	wxMenu* products = new wxMenu;
	products->Append(ID_NEW_CATEGORY, "New categorty\tCtrl-T");





	menubar->Append(file, wxT("&File"));
	menubar->Append(products, wxT("&Products"));
	SetMenuBar(menubar);
}

void MainFrame::CreateStatusBar()
{
}

void MainFrame::CreateProductList()
{
	mProductList = new ProductList(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	mFrameManager->AddPane(mProductList, wxAuiPaneInfo().Name("Product list").Caption("ProductList").CenterPane().Show());

	mFrameManager->Update();
}

void MainFrame::OnIdle(wxIdleEvent& event)
{
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	//check for clean up and save 

	event.Skip();
}

void MainFrame::OnSave(wxCommandEvent& event)
{
	wxMessageBox("Saving", "Save", wxOK);
}

void MainFrame::OnLoad(wxCommandEvent& event)
{
	wxMessageBox("loading", "load", wxOK);
}

void MainFrame::OnNew(wxCommandEvent& event)
{
	
}

void MainFrame::OnNewCategory(wxCommandEvent& event)
{
	wxMessageBox("category", "category", wxOK);
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
	event.Skip();
}
