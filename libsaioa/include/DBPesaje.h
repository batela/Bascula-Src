/*
 * DBPesaje.h
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#ifndef DBPESAJE_H_
#define DBPESAJE_H_

#include "../include/DB.h"

namespace container {

class DBPesaje: public DB {
public:
	DBPesaje(string path);
	virtual ~DBPesaje();
	int ReadHistoricData (string startdate, string enddate,string &data);
	int ReadMaxDayData (string startdate, string enddate,string limit, string &data);
	int ReadLastTenData (string &data);
	int InsertData (int canal,float valor);
};

} /* namespace container */

#endif /* DBPESAJE_H_ */
