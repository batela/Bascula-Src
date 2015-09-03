/*
 * MODBUSExplorador.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: root
 */

#include "../include/MODBUSExplorador.h"
#include "../include/IOEnlace.h"
namespace container {
extern log4cpp::Category &log;

void* lectorModbus (void * explorador){
	log.debug("%s: %s",__FILE__, "Lanzado thread lector...");

	MODBUSExplorador * exp = (MODBUSExplorador*)explorador;
	int contador = 0 ;
	int res = 0 ;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	//tim.tv_nsec = 500000000L;
	tim.tv_nsec = atoi(Env::getInstance()->GetValue("modbusperiod").data()) * 1000000L; //en milisegundos
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

/***
 *
 */

MODBUSExplorador::MODBUSExplorador(Enlace* e, Puerto* p) : Explorador (e,p,false){
	Abrir();
	cfg = NULL;
}

MODBUSExplorador::MODBUSExplorador(Enlace* e, Puerto* p,string file) : Explorador (e,p,false){
	Abrir();
	cfg = NULL;
	int lanzar = 0 ;
	string ficheroCfg= Env::getInstance()->GetValue("dxconfig");
	if (ConfigReadFile(file.data(), &cfg) != CONFIG_OK) {
		log.error("%s: %s %s",__FILE__, "Error leyendo fichero de confguracion: ", ficheroCfg.data());
		cfg = NULL;
	}
	else {
		ConfigReadInt(cfg,"general","lanzar",&lanzar,0);
		if (lanzar >0) {
			this->sigue= true ;
			LanzarExplorador();
		}
	}
}
/***
 *
 */
MODBUSExplorador::~MODBUSExplorador() {
}
/**
 *
 */
void MODBUSExplorador::LanzarExplorador (){
	log.info("%s: %s", __FILE__, "Abriendo puerto");
	if (this->getPuerto()->getIsOpen()== false && this->getPuerto()->abrir() != 0) log.error("%s: %s", __FILE__, "Error abriendo puerto!!");
	int res = pthread_create( &idThLector, NULL, lectorModbus, this);

	struct sched_param params;
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

/***
 *
 */
int MODBUSExplorador::Explora (){
	log.debug("%s: %s",__FILE__, "Comienza exploracion MODBUS");
	int res = 1 ;
	char buffer[256];
	res = 0 ;
	memset (buffer,0,256);

	if (this->getPuerto()->getIsOpen() == false) {
		this->Cerrar();
		this->Abrir();
	}
	else {
		if (cfg == NULL){
			if (((MODBUSPuerto*)getPuerto())->leer(0x01,0,4,buffer) > 0){
			  res = (getEnlace())->analizaTrama(buffer);
			}
		}
		else{
			ExploraEquipo();
		}
	}
	log.debug("%s: %s",__FILE__, "Termina exploracion MODBUS");
	return res;
}
/**
 *
 */
void MODBUSExplorador::ExploraEquipo() {
	int kc = 0 ;
	if ((kc=ConfigGetKeyCount(cfg,"0x04"))>0){
		LeerHoldingRegisters(kc);
	}
	if ((kc=ConfigGetKeyCount(cfg,"0x03"))>0){
		//LeerInputRegisters(kc);
	}
}
/***
 *
 */
void MODBUSExplorador::LeerHoldingRegisters(int kc) {

	log.debug("%s: %s",__FILE__, "Iniciando lectura Holding Registers");
	char buffer[256];
	memset (buffer,0,256);

	char kini[100];
	char kcou[100];
	int inicio;
	int bytes;
	for (int i = 0 ; i < kc /2 ; i++){
		sprintf (kini,"ini%d",i);
		sprintf (kcou,"cou%d",i);
		ConfigReadInt(cfg,"0x04",kini,&inicio,0);
		ConfigReadInt(cfg,"0x04",kcou,&bytes,0);
		if (((MODBUSPuerto*)getPuerto())->leer(0x04,inicio,bytes,buffer) <= 0){
		  (getEnlace())->trataError();
		}
		else {
		  (getEnlace())->analizaTrama(buffer,i);
		}
	}
	log.debug("%s: %s",__FILE__, "Finalizando lectura Holding Registers");
}
/***
 *
 */
void MODBUSExplorador::LeerInputRegisters(int kc) {
	log.debug("%s: %s",__FILE__, "Finalizando lectura Input Registers");

	char buffer[256];
	memset (buffer,0,256);
	char kini[100];
	char kcou[100];
	int inicio;
	int bytes;
	for (int i = 0 ; i < kc /2 ; i++){
		sprintf (kini,"ini%d",i);
		sprintf (kcou,"cou%d",i);
		ConfigReadInt(cfg,"0x03",kini,&inicio,0);
		ConfigReadInt(cfg,"0x03",kcou,&bytes,0);
		if (((MODBUSPuerto*)getPuerto())->leer(0x03,inicio,bytes,buffer) <= 0) memset (buffer,255,bytes);
		(getEnlace())->analizaTrama(buffer);

	}

	log.debug("%s: %s",__FILE__, "Finalizando lectura Input Registers");
}

} /* namespace container */
