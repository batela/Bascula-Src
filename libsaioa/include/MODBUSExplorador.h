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
public:;
	MODBUSExplorador(Enlace* e, Puerto* p);
	MODBUSExplorador(Enlace* e, Puerto* p, string file);
	MODBUSExplorador(vector<Enlace*> e, Puerto* p,string file) ;
	virtual ~MODBUSExplorador();
	int Explora ();
	int ExplorarBus();
	int EscribeCoil(int dirMB,int coil, int val);
private:
	void LanzarExplorador();
	void ExploraEquipo();
	void LeerHoldingRegisters(int kc);
	void LeerInputRegisters(int kc);
	int LeerRegistros(int kc,int codigo,Enlace *);
	pthread_mutex_t thread_mutex;
	Config* cfg;

};

} /* namespace container */

#endif /* MODBUSEXPLORADOR_H_ */
