/*
 * IOEnlace.h
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#ifndef IOENLACEGRUA_H_
#define IOENLACEGRUA_H_
#include <inttypes.h>
#include <unistd.h>
#include <cerrno>
#include "Env.h"
#include "modbus.h"
#include "modbus-ct.h"

#include "enlace/Enlace.h"
#include "PosicionBrazo.h"
#include "GruaIO.h"

namespace container {

class IOEnlaceGrua: public container::Enlace {
public:
	IOEnlaceGrua();
	virtual ~IOEnlaceGrua();
	int analizaTrama ( char * trama,int tipo);
	GruaIO * GetIOGrua() {return &grua;};
	void Configure (string a);
private:
	GruaIO grua;
};

} /* namespace container */
#endif /* IOENLACE_H_ */
