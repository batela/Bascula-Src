/*
 * IOEnlace.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "../include/IOEnlaceGrua.h"

namespace container {
	extern log4cpp::Category &log;

	IOEnlaceGrua::IOEnlaceGrua() {
	  cfg = NULL;
	}

	IOEnlaceGrua::~IOEnlaceGrua() {
	}

	void IOEnlaceGrua::Configure (string a){

	  if (ConfigReadFile(a.data(), &cfg) != CONFIG_OK) {
	    log.error("%s: %s %s",__FILE__, "Error leyendo fichero de confguracion: ", a.data());
	    cfg = NULL;
	  }
	}
	/**
	 * @return 0 if OK
	 */
	int IOEnlaceGrua::analizaTrama (char * trama,int tipo){
		log.debug("%s: %s",__FILE__, "Comienza funcion AnalizaTrama");

		grua.setIo0(trama[0]);
		grua.setIo1(trama[1]);
		grua.setIo2(trama[2]);
		grua.setIo3(trama[3]);
		grua.setIo4(trama[4]);
		grua.setIo5(trama[5]);
		grua.setIo6(trama[6]);
		grua.setIo7(trama[7]);

		log.debug("%s: %s",__FILE__, "Fin de funcion AnalizaTrama");
		return 0;
	}


} /* namespace container */
