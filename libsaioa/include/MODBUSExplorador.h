/*
 * MODBUSExplorador.h
 *
 *  Created on: Dec 17, 2014
 *      Author: root
 */

#ifndef MODBUSEXPLORADOR_H_
#define MODBUSEXPLORADOR_H_

#include "Explorador.h"
#include "MODBUSPuerto.h"
#include "configini.h"
namespace container {

class MODBUSExplorador: public Explorador {
public:
	MODBUSExplorador(Enlace* e, Puerto* p);
	MODBUSExplorador(Enlace* e, Puerto* p, string file);
	virtual ~MODBUSExplorador();
	int Explora ();
private:
	void LanzarExplorador();
	void ExploraEquipo();
	void LeerHoldingRegisters(int kc);
	void LeerInputRegisters(int kc);
	Config* cfg;
};

} /* namespace container */

#endif /* MODBUSEXPLORADOR_H_ */
