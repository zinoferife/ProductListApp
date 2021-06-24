#include "ArtProvider.h"
#include "Application.h"

ArtProvider::ArtProvider()
{
	wxGetApp().mApplicationPath;

	mPath = wxGetApp().mApplicationPath;
	CreateImageStore();
}

void ArtProvider::CreateImageStore()
{
	mImageStore.insert(std::make_pair("add", wxBitmap(wxString(mPath + "\\icon\\action_add.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("check", wxBitmap(wxString(mPath + "\\icon\\action_check.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("delete", wxBitmap(wxString(mPath + "\\icon\\action_delete.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("back", wxBitmap(wxString(mPath + "\\icon\\arrow_back.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("next", wxBitmap(wxString(mPath + "\\icon\\arrow_next.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("file", wxBitmap(wxString(mPath + "\\icon\\file.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("folder", wxBitmap(wxString(mPath + "\\icon\\folder.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("folder_open", wxBitmap(wxString(mPath + "\\icon\\folder_open.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("folder_files", wxBitmap(wxString(mPath + "\\icon\\folder_files.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("reply", wxBitmap(wxString(mPath + "\\icon\\reply.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("save", wxBitmap(wxString(mPath + "\\icon\\save.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("search", wxBitmap(wxString(mPath + "\\icon\\search.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("user", wxBitmap(wxString(mPath + "\\icon\\user.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("remove", wxBitmap(wxString(mPath + "\\icon\\action_remove.ico"), wxBITMAP_TYPE_ICO)));
	mImageStore.insert(std::make_pair("download", wxBitmap(wxString(mPath + "\\icon\\download.ico"), wxBITMAP_TYPE_ICO)));

	for (auto& i : mImageStore)
	{
		if (!i.second.Ok())
		{
			wxMessageBox("Cannot create art image store, error loading from file", "ART PROVIDER", wxICON_ERROR | wxOK);
			return;
		}
	}
}

wxBitmap ArtProvider::CreateBitmap(const wxArtID& id, const wxArtClient& client, const wxSize& size)
{
	std::string Data = id.ToStdString();
	auto found = mImageStore.find(Data);
	wxBitmap& resource = found->second;
	return resource;
}
