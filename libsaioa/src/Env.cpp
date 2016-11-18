/*
 * Env.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: borja
 */

#include "../include/Env.h"
#include "../include/DB.h"


namespace container {
log4cpp::Category &log  = log4cpp::Category::getRoot();

bool Env::initialized= false ;
Env* Env::env = NULL;
string Env::configFile = "" ;


Env::Env() {
  printf ("Leyendo base de datos\n");
  if (this->configFile.compare("") == 0){
    printf ("Leyendo base de datos\n");
    ReadDB();
  }
  else
    ReadFile();
}

Env::~Env() {
	// TODO Auto-generated destructor stub
}

Env * Env::getInstance(){
	if (!initialized){
		env =new Env();
		initialized = true;
	}
	return env;
}

Env * Env::getInstance(string a){
	configFile = a ;
	return (getInstance());
}

void Env::ReadFile()
{
    string id, eq, val,line;
    ifstream cfgfile (configFile.c_str());
    printf ("Leyendo fichero\n");
    while(getline((std::istream&)cfgfile, line))
    {
    	//cout << "Leido.." << line << "\n";
      if ((line[0] == '#') || (line.find("=")==std::string::npos)) continue;  // skip comments
      vector<string> items = this->split (line,'=');
      configValues.insert(std::pair<string,string> (items[0],items[1]));
    }
}

void Env::ReadDB()
{
   printf ("Leyendo base de datos\n");
   DB db("/home/batela/bascula/db/kemen.db");
   if (db.Open()==true){
     db.ReadConfig(configValues);
   }//if
   db.Close();
}

string Env::GetValue (string key){
	string res ="";
	std::map<string,string>::iterator it;
	it  = configValues.find(key);
	if (it != configValues.end()) {
	 res = (it->second);
	}
	return res;
}

vector<std::string> &Env::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::istringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
  	//cout << "Config Item .." << item << "\n";
  	if (!item.empty() )elems.push_back(item);
  }
    return elems;
}


vector<std::string> Env::split(const std::string &s, char delim) {
	vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

} /* namespace a2pbeer */
