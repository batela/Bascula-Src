/*
 * Explorador.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "Explorador.h"


namespace container {
extern log4cpp::Category &log;

void* lector (void * explorador){
	log.debug("%s: %s",__FILE__, "Lanzado thread lector...");

	Explorador * exp = (Explorador*)explorador;
	int contador = 0 ;
	int res = 0 ;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	//tim.tv_nsec = 500000000L;
	tim.tv_nsec = atoi(Env::getInstance()->GetValue("exploradorperiod").data()) * 1000000L; //en milisegundos
	while (exp->sigue){
		res = 0;
		if (exp->getPuerto()->getIsOpen()== false) {
			res = exp->getPuerto()->abrir();
		}
		else {
			res = exp->Explora();
		}
		if (res < 0){
			exp->getPuerto()->cerrar();
		}
		if (contador++ >= 100000) {
			log.info("%s: %s",__FILE__,"Esperamos trama..");
			contador = 0;
		}
		nanosleep(&tim , &tim2);
	}
	printf ("Salimos thread explorador..");
}

Explorador::Explorador() {
	// Ayyyy que hostia se va a dar mas tarde....
	enlace = NULL;
	puerto = NULL;
	sigue = true ;
}
Explorador::Explorador(Enlace* e, Puerto* p, bool lanzar){
	enlace = e;
	puerto = p;
	sigue = lanzar;
	if (sigue) LanzarExplorador();
}
Explorador::Explorador(vector<Enlace*> e, Puerto* p,bool lanzar)
{
  enlaces = e;
  puerto = p;
  sigue = lanzar;
  if (sigue) LanzarExplorador();
}
Explorador::~Explorador() {
	// TODO Auto-generated destructor stub
	sigue = false ;
}

void Explorador::LanzarExplorador (){
	log.info("%s: %s", __FILE__, "Abriendo puerto");
	if (puerto->abrir() != 0) log.error("%s: %s", __FILE__, "Error abriendo puerto!!");
	int res = pthread_create( &idThLector, NULL, lector, this);

	struct sched_param params;
	  // We'll set the priority to the maximum.
	params.sched_priority = sched_get_priority_max(SCHED_FIFO);
	res = pthread_setschedparam(idThLector, SCHED_FIFO, &params);
	if (res != 0) {
		log.error("%s: %s", __FILE__, "Unsuccessful in setting thread realtime prio" );
	}

	int policy = 0;
	res = pthread_getschedparam(idThLector, &policy, &params);
	if (res != 0) {
		log.error("%s: %s", __FILE__,"Couldn't retrieve real-time scheduling paramers");
	}
}

int Explorador::Explora (){
	//log.info("%s: %s", __FILE__, "Comienza exploracion");

	int estado =  0;
	char data;
	int res = 0;
	int count = 0 ;
	int indice = 0;

	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = atoi(Env::getInstance()->GetValue("charperiod").data()) * 1000000L; //en milisegundos
	while (estado != 3){
		switch (estado){
				case 0:
					indice = 0 ;
					getEnlace()->rxbuffer[indice]= 0 ;
					count= ((RS232Puerto*)getPuerto())->leerSimple(data);
					if (count > 0 && data == enlace->GetStartByte()) {
						nanosleep(&tim , &tim2);
						getEnlace()->rxbuffer[indice++] = data;
						estado = 1;
					}
					else if (count <=0 ) {
						res = 1;
						estado = 3;
					}
				break;
				case 1:
					count = ((RS232Puerto*)getPuerto())->leerSimple(data);
					if (count == 0 || data==enlace->GetEndByte()) {
						estado = 2;
					}
					else {
						nanosleep(&tim , &tim2);
					}
					getEnlace()->rxbuffer[indice++] = data;
				break;
				case 2:
					getEnlace()->rxbuffer[indice] = 0 ;
					log.info("%s: %s %s", __FILE__, "Trama leida",getEnlace()->rxbuffer );
					res = getEnlace()->analizaTrama(getEnlace()->rxbuffer);
					estado = 3 ;
				break;
			}
			//nanosleep(&tim , &tim2);
	}
	//log.info("%s: %s", __FILE__, "Fin de exploracion");
	return 1;
}
int Explorador::Enviar (int longitud, Orden orden ){
	return 0 ;
}

int Explorador::Enviar (int longitud,char *buffer ){
	if (sigue == false) puerto->abrir();
	puerto->escribir(buffer,longitud);
	if (sigue == false) puerto->cerrar();
	return 0 ;
}

bool Explorador::Abrir ( ){
	int res = -1;
	if (this->getPuerto()->getIsOpen()== false) {
				res = this->getPuerto()->abrir();
	}
	return (res==0?true:false);
}

bool Explorador::Cerrar ( ){
	return (true);
}
} /* namespace container */
