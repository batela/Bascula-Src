/*
 * IOEnlace.h
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#ifndef IOENLACE_H_
#define IOENLACE_H_
#include <inttypes.h>
#include <unistd.h>
#include <cerrno>
#include "Env.h"
#include "modbus.h"
#include "modbus-ct.h"

#include "enlace/Enlace.h"
#include "PosicionBrazo.h"
#include "Locks.h"
namespace container {

class IOEnlace: public container::Enlace {
public:
	IOEnlace();
	virtual ~IOEnlace();
	int analizaTrama ( char * trama,int tipo);
	Locks * GetLocks() {return &locks;};
	void Configure (string a);
private:
	Locks locks;
};

} /* namespace container */
#endif /* IOENLACE_H_ */
