#include "Application.h"
IMPLEMENT_APP(Application)


wxString wxFindAppPath(const wxString& argv0, const wxString& cwd, const wxString& appVariableName,
	const wxString& appName)
{
	wxString str;

#ifndef __WXWINCE__
	// Try appVariableName
	if (!appVariableName.IsEmpty())
	{
		str = wxGetenv(appVariableName);
		if (!str.IsEmpty())
			return str;
	}
#endif

#if defined(__WXMAC__) && !defined(__DARWIN__)
	return cwd;
#endif

	if (wxIsAbsolutePath(argv0))
		return wxPathOnly(argv0);
	else
	{
		// Is it a relative path?
		if (!cwd.IsEmpty())
		{
			wxString currentDir(cwd);
			if (currentDir.Last() != wxFILE_SEP_PATH)
				currentDir += wxFILE_SEP_PATH;

			str = currentDir + argv0;
			if (wxFileExists(str))
				return wxPathOnly(str);
#ifdef __WXMAC__
			// The current directory may be above the actual
			// bundle. So if we find the bundle below it,
			// we know where we are.
			if (!appName.IsEmpty())
			{
				wxString p = currentDir + appName + wxT(".app");
				if (wxDirExists(p))
				{
					p += wxFILE_SEP_PATH;
					p += wxT("Content/MacOS");
					return p;
				}
			}
#endif
		}
	}

	// OK, it's neither an absolute path nor a relative path.
	// Search PATH.

	wxPathList pathList;
	pathList.AddEnvList(wxT("PATH"));
	str = pathList.FindAbsoluteValidPath(argv0);
	if (!str.IsEmpty())
		return wxPathOnly(str);

	// Failed
	return wxEmptyString;
}





Application::Application()
{
}

bool Application::OnInit()
{
	//init image readers/writers
	wxInitAllImageHandlers();
	
	//find working directory
	wxString curDir = wxGetCwd();
	mApplicationPath = wxFindAppPath(argv[0], curDir, wxT("DPROLIST"), wxT("Dglopa Product list"));
	
	//create main app frame
	MainFrame* frame = new MainFrame(NULL, MAIN_FRAME, wxDefaultPosition, wxSize(800,600));
	if (!frame->isCreated)
	{
		return false;
	}
	frame->CenterOnScreen();
	frame->Show(true);
	frame->Load();
	return true;
}
