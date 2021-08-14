#pragma once
#include <map>
#include <iostream>
#include <string>
#include <fstream>

#include <wx/wxprec.h>
#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif // ! WX_PRECOMP

class PLConfig
{
public:
	PLConfig();
	PLConfig(const std::string& configFIle);

	void SaveConFigFile();
	void LoadConfigFile();
	std::string& operator[](const std::string& key);
	bool InsertConfig(const std::string& key, const std::string& value);


private:
	std::map<std::string, std::string> mConfigMap;
	std::string mConfigFile;
	bool doLoad(std::fstream& file);
	bool doSave(std::fstream& file);
};

