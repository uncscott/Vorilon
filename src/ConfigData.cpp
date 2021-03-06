/*Vorilon is a VOIP Conferencing system
 * Copyright (C) 2009  Daniel Myers <dan@moird.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "ConfigData.h"

namespace Vorilon{
	ConfigData::ConfigData(ServerData *servD){
		sData = *servD;
		
		//Paths to Config File and Config file name
		CFile = "vorilond.conf";
		
		//While in Debug mode also look for a config in the sources/server folder
		//Cause im lazy and I don't want to copy the config file while loading up
		//Vorilond for testing everytime I wipe out the cmake files
#if DEBUG
		CFPaths.push_back(VORILOND_DEBUGCONFPATH + CFile);
#endif /*DEBUG*/
		CFPaths.push_back("./" + CFile);
		CFPaths.push_back("~/.vorilond/" + CFile);
		CFPaths.push_back("/etc/" + CFile);
	}

	ConfigData::~ConfigData(){}

	void ConfigData::ReadData(){
		LoadConfigFile(CheckForConf());
		
		//Load Values from config file into ServerData
		//TODO Error Checking!
		sData.Port(boost::lexical_cast<unsigned short>(KeyValue.find("port")->second));
		Debug::ConsoleMsg("Config Port: " + boost::lexical_cast<std::string>(sData.Port()));
	}
	
	fs::path ConfigData::CheckForConf(){
		for(std::list<fs::path>::iterator it=CFPaths.begin(); it!=CFPaths.end(); ++it){
			if(fs::exists(*it)){
				Debug::ConsoleMsg("Config file found: " + it->string());
				return *it;
			}
		}
		return NULL;
	}

	fs::path ConfigData::CheckForConf(std::string conf){
		fs::path tmpPath = conf;
		if(fs::exists(tmpPath)){
			Debug::ConsoleMsg("Config file found: " + tmpPath.string());
			return tmpPath;
		}
		return NULL;
	}

	void ConfigData::LoadConfigFile(fs::path ConfFile){
		//At this point we know there is a Config File, but we need some error handling
		//TODO Error Checking
		std::string cfline;
		fs::ifstream cFin(ConfFile);
		while(getline(cFin, cfline)){
			ReadConfigFile(cfline);
		}
		cFin.close();
	}

	void ConfigData::ReadConfigFile(std::string cfline){
		if(cfline[0] == '#' || cfline == "/n"){
			return;
		}
		int pos = cfline.find("=");
		std::string Key = cfline.substr(0,pos);
		std::string Value = cfline.substr(++pos);
		KeyValue.insert(KeyValueEntry_t (Key,Value));
	}
}