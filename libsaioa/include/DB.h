/*
 * DB.h
 *
 *  Created on: Jun 18, 2014
 *      Author: borja
 */

#ifndef DB_H_
#define DB_H_

#include <string>
#include <iostream>
#include <sqlite3.h>
#include <list>

#include "Env.h"


using namespace std;
namespace container {
class DB {
public:
	DB();
	DB(string a);
	virtual ~DB();
	bool Open ();
	void Close ();
	bool isOpen (){return open;};
	void ReadConfig(map<string,string>&cfg);
protected:
	sqlite3 *db;
private:
	string dbPath;
	bool open;
};

} /* namespace a2pbeer */
#endif /* DB_H_ */
