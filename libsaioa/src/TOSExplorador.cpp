/*
 * TOSExplorador.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: root
 */

#include "../include/TOSExplorador.h"

namespace container {
extern log4cpp::Category &log;
/**
 *
 */
TOSExplorador::TOSExplorador() {
  cfg = NULL;
}
/**
 *
 */
TOSExplorador::~TOSExplorador() {

}
/**
 *
 */
TOSExplorador::TOSExplorador(Enlace* e, Puerto* p,string file) : Explorador (e,p,false){
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
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;
}
/**
 *
 */
TOSExplorador::TOSExplorador(vector<Enlace*> e, Puerto* p,string file) : Explorador (e,p,false){
  Abrir();
  cfg = NULL;
  int lanzar = 0 ;

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
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;
}

/**
 *
 */
void TOSExplorador::LanzarExplorador ()
{

}
/**
 *
 */
int TOSExplorador::Explora ()
{
    return 0;
}

bool TOSExplorador::EscribeTramaTOS(bool import, string grua, int peso)
{
  bool res = false  ;

  char msj[100];
  if ((res =((TOSEnlace*)this->getEnlace())->MsjTOS(import,grua,peso,msj)) == true)
    this->getPuerto()->escribir(msj,strlen(msj));
  else
    log.error("%s: %s %s %s %d",__FILE__, "Error generado mensaje TOS ", (import)?"Import":"Export",grua.c_str(),peso);

  return res;
}

/**
 *
 */
void TOSExplorador::ExploraEquipo() {
}

} /* namespace container */
