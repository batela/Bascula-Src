/*
 * Explorador.h
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#ifndef EXPLORADOR_H_
#define EXPLORADOR_H_


#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "Enlace.h"
#include "Puerto.h"
#include "RS232Puerto.h"
#include "Orden.h"
#include "Env.h"

using namespace std;
namespace container {
class Explorador {
public:
	Explorador();
	Explorador(Enlace* e, Puerto* p,bool lanzar);
	virtual ~Explorador();
	void LanzarExplorador ();
	friend void *lector( void *ptr );
	int Enviar (int longitud, Orden orden );
	int Enviar (int longitud, char* buffer );
	int Explora();
	bool Abrir();
	bool Cerrar();
	bool sigue ;
	Puerto * getPuerto (){return puerto;}
	Enlace * getEnlace (){return enlace;}
protected:
	pthread_t idThLector;
private:
	Enlace* enlace;
	Puerto* puerto;

	//thread first;     // spawn new thread that calls foo()
};

} /* namespace container */
#endif /* EXPLORADOR_H_ */
