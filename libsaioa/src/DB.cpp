/*
 * DB.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: borja
 */
#include "../include/DB.h"

namespace container {
extern log4cpp::Category &log ;
DB::DB() {
	// TODO Auto-generated constructor stub
  this->db = 0;
  //this->dbPath= Env::getInstance()->GetValue("dbpath");
}

DB::DB(string a) {
	// TODO Auto-generated constructor stub
  this->db = NULL;
  this->dbPath= a;
  open = false;
}

DB::~DB() {
	// TODO Auto-generated destructor stub
}

bool DB::Open ()	{
	log.info("Starting function..") ;
	log.info("%s - %s: %s", __FILE__,__FUNCTION__, "Starting function..") ;
	thread_mutex = PTHREAD_MUTEX_INITIALIZER;

	int rc = sqlite3_open(this->dbPath.c_str(), &db);
	bool res = false ;
	if(rc){
		log.error("%s-%s: %s", __FILE__,__FUNCTION__, "Can't open database") ;
	  sqlite3_close(db) ;
	  db = NULL;
	} else {
		log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Open database successfully") ;
	  res = true ;
	  open = true;
	}
	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return res;
}

/**
 *

bool DB::SelectEquipmentListByAreaName (string arg, list <Equipment> &l){

	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	bool res = false ;
	string equipment;
	string protocol;
	string address;
	string area;

	string sql ="SELECT * FROM vwEquipmentArea WHERE area='" + arg + "';";
	log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , sql.data()) ;

  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,sql.c_str(),-1,&statement,0) == SQLITE_OK ) {
  	int ctotal = sqlite3_column_count(statement);
    int res = SQLITE_ROW;
    while ( res != SQLITE_DONE && res!=SQLITE_ERROR )  {
    	res = sqlite3_step(statement);
      if ( res == SQLITE_ROW ){
      	Equipment e ;
      	for ( int i = 0; i < ctotal; i++ ) {
      		switch (i){
      			case 0:	e.setName((char*)sqlite3_column_text(statement, i)); break;
      			case 1: e.setProtocol((char*)sqlite3_column_text(statement, i));break;
      			case 2:	e.setAddress((char*)sqlite3_column_text(statement, i)); break;
      			case 3:	e.setAreaName((char*)sqlite3_column_text(statement, i)); break;
      		}
        }
      	l.push_back(e);
        res = true ;
      }
    }
  }
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return res ;
}
*/

void DB::ReadConfig(map <string,string> &cfg)
{
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;
  bool res = false ;
  string sql ="SELECT clave , valor FROM tconfiguracion";
  log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , sql.data()) ;
  string clave;
  string valor;
  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,sql.c_str(),-1,&statement,0) == SQLITE_OK ) {
    int ctotal = sqlite3_column_count(statement);
    int res = SQLITE_ROW;
    while ( res != SQLITE_DONE && res!=SQLITE_ERROR )  {
      res = sqlite3_step(statement);
      if ( res == SQLITE_ROW ){
        for ( int i = 0; i < ctotal; i++ ) {
          switch (i){
            case 0: clave =((char*)sqlite3_column_text(statement, i)); break;
            case 1: valor =((char*)sqlite3_column_text(statement, i)); break;
          }
        }
        cfg.insert(std::pair<string,string> (clave,valor));
      }
    }
  }
}

void DB::Close ()	{
	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	if (db != NULL) sqlite3_close(db);
	open = false;
	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
}
} /* namespace a2pbeer */
