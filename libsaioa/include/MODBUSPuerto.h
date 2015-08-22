/*
 * MODBUSPuerto.h
 *
 *  Created on: Nov 15, 2014
 *      Author: root
 */

#ifndef MODBUSPUERTO_H_
#define MODBUSPUERTO_H_

#include <cerrno>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <unistd.h>
#include <cerrno>
#include "Puerto.h"
#include "Env.h"
#include "modbus.h"
#include "modbus-ct.h"

namespace container {

class MODBUSPuerto: public container::Puerto {
public:
	MODBUSPuerto(string id, int br=9600);
	virtual ~MODBUSPuerto();
	int abrir();
	void cerrar();
	int reabrir();
	int leer (char buffer[]){return 0;};
	int leer (int cod, int inicio, int tam,char  buffer[]);
	int escribir (char buffer[], int count);
private:
		int baudrate;
		modbus_t *ctx;
};

} /* namespace Container */

#endif /* MODBUSPUERTO_H_ */
