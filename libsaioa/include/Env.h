/*
 * Env.h
 *
 *  Created on: Jul 1, 2014
 *      Author: borja
 */

#ifndef ENV_H_
#define ENV_H_

#include 	<iostream>
#include 	<string>
#include	<fstream>
#include <sstream>
#include	<vector>
#include 	<map>


#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"
#include "log4cpp/PropertyConfigurator.hh"

using namespace std;

namespace container {

class Env {
public:
	static bool initialized ;
	static Env * env;
	static Env * getInstance();
	static Env * getInstance(string a);
	virtual ~Env();
	void ReadFile();
	void ReadDB();
	string GetValue (string key);

private:
	Env();
	static string configFile;
	map <string,string> configValues;
	vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems) ;
	vector<std::string> split(const std::string &s, char delim) ;
};

} /* namespace container */
#endif /* ENV_H_ */
