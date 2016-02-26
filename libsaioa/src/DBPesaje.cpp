/*
 * DBPesaje.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#include "../include/DBPesaje.h"

namespace container {
extern log4cpp::Category &log ;
DBPesaje::DBPesaje(string path) : DB(path) {

}

DBPesaje::~DBPesaje() {
}

int DBPesaje::ReadHistoricData (string startdate, string enddate,string &data){

	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	bool res = false ;
	string sql ="SELECT idCanal,valor,fecha FROM thistorico WHERE fecha>='" +startdate + "' and fecha<='" + enddate +"';";
	log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , sql.data()) ;

	int idCanal;
	double valor;
	string fecha;
  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,sql.c_str(),-1,&statement,0) == SQLITE_OK ) {
  	int ctotal = sqlite3_column_count(statement);
    int res = SQLITE_ROW;
    while ( res != SQLITE_DONE && res!=SQLITE_ERROR )  {
    	res = sqlite3_step(statement);
      if ( res == SQLITE_ROW ){
      	for ( int i = 0; i < ctotal; i++ ) {
      		switch (i){
      			case 0: idCanal =((int)sqlite3_column_int(statement, i));break;
      			case 1:	valor 	= ((double)sqlite3_column_double(statement, i)); break;
      			case 2:	fecha 	=((char*)sqlite3_column_text(statement, i)); break;
      		}
        }
      	char raw[256];
      	sprintf(raw,"%d;%.0f;%s\n",idCanal,valor,fecha.data());
      	data = data + raw;
      }
    }
    sqlite3_finalize(statement);
  }
  if (data.compare("")== 0) data = "NO DATA\n";
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return res ;
}

int DBPesaje::ReadMaxDayData (string startdate, string enddate, string count, string &data){

	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	bool res = false ;
	string sql ="SELECT idCanal,valor,fecha FROM thistorico WHERE fecha>='" +startdate + " 00:00:00' and fecha<='" + enddate +" 23:59:59'" +
			+ "ORDER BY valor DESC LIMIT " + count ;
	log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , sql.data()) ;

	int idCanal;
	double valor = 0 ;
	string fecha;
  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,sql.c_str(),-1,&statement,0) == SQLITE_OK ) {
  	int ctotal = sqlite3_column_count(statement);
    int res = SQLITE_ROW;
    while ( res != SQLITE_DONE && res!=SQLITE_ERROR )  {
    	res = sqlite3_step(statement);
      if ( res == SQLITE_ROW ){
      	for ( int i = 0; i < ctotal; i++ ) {
      		switch (i){
      			case 0: idCanal =((int)sqlite3_column_int(statement, i));break;
      			case 1:	valor 	= ((double)sqlite3_column_double(statement, i)); break;
      			case 2:	fecha 	=((char*)sqlite3_column_text(statement, i)); break;
      		}
        }
      	char raw[256];
      	sprintf(raw,"%d;%.0f;%s\n",idCanal,valor,fecha.data());
      	data = data + raw;
      }
    }
    sqlite3_finalize(statement);
  }
  if (data.compare("")== 0) data = "NO DATA\n";
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return res ;
}

int DBPesaje::ReadLastTenData (string &data){

	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	int isError = 1;
	string sql ="SELECT idCanal,valor,fecha FROM thistorico ORDER BY fecha DESC LIMIT 10;";
	log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , sql.data()) ;

	int idCanal;
	double valor = 0 ;
	string fecha;
  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,sql.c_str(),-1,&statement,0) == SQLITE_OK ) {
  	int ctotal = sqlite3_column_count(statement);
    int res = SQLITE_ROW;
    while ( res != SQLITE_DONE && res!=SQLITE_ERROR )  {
    	res = sqlite3_step(statement);
      if ( res == SQLITE_ROW ){
      	for ( int i = 0; i < ctotal; i++ ) {
      		switch (i){
      			case 0: idCanal =((int)sqlite3_column_int(statement, i));break;
      			case 1:	valor 	= ((double)sqlite3_column_double(statement, i)); break;
      			case 2:	fecha 	=((char*)sqlite3_column_text(statement, i)); break;
      		}
      	}
      	char raw[256];
      	sprintf(raw,"%d;%.0f;%s\n",idCanal,valor,fecha.data());
      	data = data + raw;
      }
    }
    isError = 0;
    sqlite3_finalize(statement);
  }
  if (data.compare("")== 0) data = "NO DATA\n";
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return isError ;
}

int DBPesaje::InsertData (int canal,float valor){

	log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Starting function..") ;

	bool res = false ;

	time_t rawtime;
	struct tm * timeinfo;
	char now [20];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (now,20,"%F %T",timeinfo);
	char query[80];

	sprintf (query,"UPDATE tvalores SET valor=%d,fecha='%s' WHERE idCanal=%d",(int)valor,now,canal);
	//string sql ="UPDATE tvalores SET valor=" +buffer+",fecha='" + now +"' WHERE idCanal=" + canal;
	log.debug("%s-%s: %s %s", __FILE__,__FUNCTION__, "Executing query:" , query) ;

  sqlite3_stmt *statement;
  if ( sqlite3_prepare(db,query,-1,&statement,0) == SQLITE_OK ) {
  		res = sqlite3_step(statement);
  		sqlite3_finalize(statement);
  }
  else {
  	log.error("%s-%s: %s", __FILE__,__FUNCTION__, "Error al ejecutar query") ;
  }
  log.info("%s-%s: %s", __FILE__,__FUNCTION__, "Ending function..") ;
	return res ;
}

} /* namespace container */
