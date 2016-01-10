/*
 * GPSEnlace.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "GPSEnlace.h"


namespace container {
extern log4cpp::Category &log;

GPSEnlace::GPSEnlace() {
	SetStartByte('$');
	SetEndByte('\n');
}

GPSEnlace::~GPSEnlace() {
	// TODO Auto-generated destructor stub
}
/**
 * Devuelve: -1 si la trama es desconcocida, 1 si el tratamiento de la trama es erroneo, 0 si va bine
 *
 */
int GPSEnlace::analizaTrama(char buffer[]){
	int res = -1;
	log.info("%s: %s %s",__FILE__, "Analizando trama.." , buffer);
	std::vector<std::string> items = split(buffer,',');
	if ((items.size() > 0) && (strcmp(items[0].data(),"$GPGGA"))== 0)
		res = completaGPS (items , gps);

	return res;
}
/**
 * res = 0 no error
 */
int GPSEnlace::completaGPS (vector<std::string> items, GPS &gps){
	log.info("%s: %s",__FILE__, "Comenzando funcion..");

	int res = 1;
	log.debug("%s: %s  %d",__FILE__, "Array de datos longitud: " ,items.size()  );
	if (items.size() == GPSEnlace::LONGITUD_GPGGA){

		string stLatAux = items[3] + items[2] ;
		string stLonAux = items[5]+ items[4] ;
		log.debug("%s: %s  %s %s",__FILE__, "Coordenadas: " ,stLatAux.data(),stLonAux.data()  );
		CLongLatString strLat(stLatAux,"HDDMM.mmmmmmm",LL_LATITUDE);
		CLongLatString strLon(stLonAux,"HDDDMM.mmmmmmm",LL_LONGITUDE);


		gps.setFecha(atoi(items[1].data()));
		gps.setLatitud(strLat.GetDecimalDegree());
		gps.setCLatitud(items[3].data()[0]);
		gps.setLongitud(strLon.GetDecimalDegree());
		gps.setCLongitud(items[5].data()[0]);
		gps.setSatelites(atoi(items[7].data()));
		gps.setCalidad(atoi(items[6].data()));
		gps.setAltitud(atof(items[9].data()));
		res =  0 ;
	}
	log.info("%s: %s",__FILE__, "Terminando funcion!!");
	return res;
}
} /* namespace container */
