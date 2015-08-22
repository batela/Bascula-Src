/*
 * IOEnlace.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "../include/IOEnlace.h"

namespace container {
	extern log4cpp::Category &log;
	IOEnlace::IOEnlace() {
	}
	IOEnlace::~IOEnlace() {
		// TODO Auto-generated destructor stub
	}
	/**
	 * @return 0 if OK
	 */
	int IOEnlace::analizaTrama (char * trama,int tipo){
		log.debug("%s: %s",__FILE__, "Comienza funcion AnalizaTrama");
		int res = 0 ;
		locks.SetLock01(trama[0]);
		locks.SetLock02(trama[1]);
		locks.SetLock03(trama[2]);
		locks.SetLock04(trama[3]);
		log.debug("%s: %s",__FILE__, "Fin de funcion AnalizaTrama");
		return res;
	}


} /* namespace container */
