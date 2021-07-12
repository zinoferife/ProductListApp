#include "PLConfig.h"
#include "Application.h"


//hack
static std::string gEmptyString;

PLConfig::PLConfig()
{
	// do nothing 
}

PLConfig::PLConfig(const std::string& configFIle)
{
	mConfigFile = wxGetApp().mApplicationPath.ToStdString() + "\\" + configFIle;
}

void PLConfig::SaveConFigFile()
{
	if (mConfigFile.empty() || mConfigMap.empty())
	{
		return;
	}
	std::fstream file(mConfigFile, std::ios::out);
	if (file.is_open())
	{
		if (!doSave(file))
		{
			wxMessageBox("Failed to save application configuration to file, fatal error, corrupted file or invalid setting", "SYSTEM ERROR", wxOK | wxICON_ERROR);
			return;
		}
	}
	else
	{
		wxMessageBox("Failed to save application configuration, configuration file missing", "SYSTEM ERROR", wxOK | wxICON_ERROR);
		return;
	}
	//all all buffer to the file 
	file.flush();
}

void PLConfig::LoadConfigFile()
{
	std::fstream  file(mConfigFile, std::ios::in);
	if (file.is_open())
	{
		if (!doLoad(file))
		{
			wxMessageBox("Failed to configure application, configuration file corrupted", "SYSTEM ERROR", wxOK | wxICON_ERROR);
		}
	}
	else
	{
		wxMessageBox("Failed to configure appliation, configuration file missing", "SYSTEM ERROR", wxOK | wxICON_ERROR);
	}
}

std::string& PLConfig::operator[](const std::string& key)
{
	std::map<std::string, std::string>::iterator mapIter;
	if (mConfigFile.empty() || (mapIter = mConfigMap.find(key)) == mConfigMap.end())
	{
		return gEmptyString;
	}
	return mapIter->second;
}

bool PLConfig::InsertConfig(const std::string& key, const std::string& value)
{
	auto i = mConfigMap.insert(std::make_pair(key, value));
	if (!i.second)
	{
		auto iter = mConfigMap.find(key);
		if (iter != mConfigMap.end())
		{
			iter->second = value;
		}
	}
	return true;
}

bool PLConfig::doLoad(std::fstream& file)
{
	std::string line;
	while (!file.eof() && !file.bad())
	{
		std::getline(file, line);
		if (line.empty())
		{
			//file end?? 
			return true;
		}
		auto pos = line.find_first_of("=");
		if (pos == std::string::npos)
		{
			//corrupt file
			return false;
		}
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.size());
		mConfigMap[key] = value;
	}
	if (file.bad())
	{
		//corrupted file 
		return false;
	}
	//file completly read
	return true;
}

bool PLConfig::doSave(std::fstream& file)
{
	for (auto iter : mConfigMap)
	{
		std::string line = iter.first + "=" + iter.second + '\n';
		file << line;
		if (file.bad())
		{
			//stream failed 
			return false;
		}
	}
	//config map empty
	return true;
}

