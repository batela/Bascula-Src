/*
 * MODBUSExplorador.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: root
 */

#include "MODBUSExplorador.h"
#include "IOEnlace.h"

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
	thread_mutex = PTHREAD_MUTEX_INITIALIZER;
}

MODBUSExplorador::MODBUSExplorador(Enlace* e, Puerto* p,string file) : Explorador (e,p,false){
	Abrir();
	cfg = NULL;
	int lanzar = 0 ;
	thread_mutex = PTHREAD_MUTEX_INITIALIZER;
	//ActualizaEstadoCom(false);

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
/**
 *
 */
MODBUSExplorador::MODBUSExplorador(vector<Enlace*> e, Puerto* p,string file) : Explorador (e,p,false){
  Abrir();
  cfg = NULL;
  int lanzar = 0 ;
  //ActualizaEstadoCom(false);
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;

  if (ConfigReadFile(file.data(), &cfg) != CONFIG_OK) {
    log.error("%s: %s %s",__FILE__, "Error leyendo fichero de confguracion: ", file.data());
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
 * Esta funcion esta pensada para leer una serie de equipos modbuss en un puerto.
 * cada equipo tiene su configuración almacenada en un fichero de texto .ini
 */
int MODBUSExplorador::ExplorarBus (){
  log.debug("%s: %s",__FILE__, "Comienza exploracion de BUS ModBUS");

  for(std::vector<Enlace*>::iterator it = getEnlaces()->begin(); it != getEnlaces()->end(); ++it) {
    int dirMB= atoi ((*it)->getItemCfg("equipo","dir").data());
    int kc = 0;
    enlace = (Enlace *)*it;
    ((MODBUSPuerto*)getPuerto())->setAddress(dirMB);
    //Entradas digitales
    if ((kc=((Enlace*)*it)->getItemCnt("0x01"))>0){
      LeerRegistros(kc,0x01,(*it));
    }
    //Registros de solo lectura
    if ((kc=((Enlace*)*it)->getItemCnt("0x03"))>0){
      LeerInputRegisters(kc);
    }
    //Registros de lectura escritura
    if ((kc=((Enlace*)*it)->getItemCnt("0x04"))>0){
      LeerRegistros(kc,0x04,(*it));
    }
  }

  log.debug("%s: %s",__FILE__, "Finaliza exploracion de BUS ModBUS");
  return 0;
}

/***
 * Esta funcion explora un solo equipo que bien tiene una configuración predefinida o tiene
 * un fichero de configuracion, tenderá a desaparcer.
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
			//ExploraEquipo();
			ExplorarBus();
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
		LeerInputRegisters(kc);
	}
}
/***
 *
 */
int  MODBUSExplorador::LeerRegistros(int kc,int codigo, Enlace *enlace)
{
  log.debug("%s: %s",__FILE__, "Iniciando lectura de Registros....");
  char buffer[256];
  memset (buffer,0,256);

  char kini[100];
  char kcou[100];
  char cfItem [5];
  sprintf (cfItem,"0x0%d",codigo);
  int inicio;
  int bytes;
  pthread_mutex_lock( &thread_mutex );
  for (int i = 0 ; i < kc /2 ; i++){
    sprintf (kini,"ini%d",i);
    sprintf (kcou,"cou%d",i);
    inicio = atoi (enlace->getItemCfg(cfItem,kini).data());
    bytes = atoi (enlace->getItemCfg(cfItem,kcou).data());
    log.debug("%s: %s %d %d %d",__FILE__, "Lanzando lectura ModBUS:", codigo,inicio,bytes);
    memset (buffer,0,256);
    if (((MODBUSPuerto*)getPuerto())->leer(codigo,inicio,bytes,buffer) <= 0){
      enlace->trataError();
      ActualizaEstadoCom(true);
    }
    else {
      enlace->analizaTrama(buffer,i);
      ActualizaEstadoCom(false);
    }
  }
  pthread_mutex_unlock( &thread_mutex );

  log.debug("%s: %s",__FILE__, "Finalizando lectura de registros");
  return 0;
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
	pthread_mutex_lock( &thread_mutex );
	for (int i = 0 ; i < kc /2 ; i++){
		sprintf (kini,"ini%d",i);
		sprintf (kcou,"cou%d",i);
		ConfigReadInt(cfg,"0x04",kini,&inicio,0);
		ConfigReadInt(cfg,"0x04",kcou,&bytes,0);
		if (((MODBUSPuerto*)getPuerto())->leer(0x04,inicio,bytes,buffer) <= 0){
		  (getEnlace())->trataError();
		  ActualizaEstadoCom(true);
		}
		else {
		  (getEnlace())->analizaTrama(buffer,i);
		  ActualizaEstadoCom(false);
		}
	}
	pthread_mutex_unlock( &thread_mutex );
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

	pthread_mutex_lock( &thread_mutex );
	for (int i = 0 ; i < kc /2 ; i++){
		sprintf (kini,"ini%d",i);
		sprintf (kcou,"cou%d",i);
		ConfigReadInt(cfg,"0x03",kini,&inicio,0);
		ConfigReadInt(cfg,"0x03",kcou,&bytes,0);
		if (((MODBUSPuerto*)getPuerto())->leer(0x03,inicio,bytes,buffer) <= 0){
		  (getEnlace())->trataError();
		  ActualizaEstadoCom(true);
		}
		else {
		  (getEnlace())->analizaTrama(buffer,i);
		  ActualizaEstadoCom(false);
		}

	}
	pthread_mutex_unlock( &thread_mutex );

	log.debug("%s: %s",__FILE__, "Finalizando lectura Input Registers");
}

int MODBUSExplorador::EscribeCoil(int dirMB,int coil, int val) {

  log.debug("%s: %s",__FILE__, "Comenzando escritura de coil");

  int res = 1 ; //Si error 1
  pthread_mutex_lock( &thread_mutex );
  ((MODBUSPuerto*)getPuerto())->setAddress(dirMB);
  if (((MODBUSPuerto*)getPuerto())->escribir(0x00,coil,val) == 1) res = 0;
  pthread_mutex_unlock( &thread_mutex );

  log.debug("%s: %s",__FILE__, "Finalizando escritura de coil");
  return  res;
}

} /* namespace container */
