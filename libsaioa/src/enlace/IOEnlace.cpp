/*
 * IOEnlace.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "../include/enlace/IOEnlace.h"

namespace container {
	extern log4cpp::Category &log;
	IOEnlace::IOEnlace() {
	  cfg = NULL;
	}
	IOEnlace::~IOEnlace() {
		// TODO Auto-generated destructor stub
	}

	void IOEnlace::Configure (string a){
	  if (ConfigReadFile(a.data(), &cfg) != CONFIG_OK) {
	    log.error("%s: %s %s",__FILE__, "Error leyendo fichero de confguracion: ", a.data());
	    cfg = NULL;
	  }
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

	int IOEnlace::trataError (){
    log.debug("%s: %s",__FILE__, "Comienza funcion TrataError");
    int res = 0 ;
    locks.SetLock01(0);
    locks.SetLock02(0);
    locks.SetLock03(0);
    locks.SetLock04(0);
    log.debug("%s: %s",__FILE__, "Fin de funcion TrataError");
    return res;
  }


} /* namespace container */
