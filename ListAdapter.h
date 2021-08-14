#pragma once
#include <wx/wx.h>
#include <wx/addremovectrl.h>
class ListBoxAdaptor : public wxAddRemoveAdaptor
{
public:
	explicit ListBoxAdaptor(wxListBox* lbox) : m_lbox(lbox) { }

	virtual wxWindow* GetItemsCtrl() const { return m_lbox; }

	virtual bool CanAdd() const { return true; }
	virtual bool CanRemove() const { return m_lbox->GetSelection() != wxNOT_FOUND; }
	virtual void OnAdd() { }
	virtual void OnRemove() { m_lbox->Delete(m_lbox->GetSelection()); }

private:
	wxListBox* m_lbox;
};
