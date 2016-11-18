/*
 * src\TOSMsj.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */

#include "TOSMsj.h"

#include <Category.hh>
#include <cstdio>

namespace container {

extern log4cpp::Category &log;

TOSMsj* TOSMsj::m_pInstance = NULL;

/**
 *
 */
TOSMsj::TOSMsj(TOSMsj const&)
{
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;
  rotacion = -1;
}
/**
 *
 */
TOSMsj::TOSMsj()
{
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;
  rotacion = -1;
}

/**
 *
 */
TOSMsj* TOSMsj::Instance()
{
  if (!m_pInstance)   // Only allow one instance of class to be generated.
    m_pInstance = new TOSMsj();
  return m_pInstance;

}

int TOSMsj::getRotacion()
{
  if (rotacion == 9)
    rotacion = 0;
  else
    rotacion++;
  return rotacion;
}
/**
 *
 */
void TOSMsj::msjTOS(bool import, string grua, int peso, char * msj)
{
  pthread_mutex_lock( &thread_mutex );
  log.debug("%s: %s",__FILE__, "Comienza funcion msjTOS");
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,80,"%Y%m%d",timeinfo);
  int ldate = atoi(buffer);
  strftime(buffer,80,"%H%M%S",timeinfo);
  int ltime = atoi(buffer);

  sprintf (msj,"{%d%d %d %06d %05d %s}\n",getRotacion(),(import?8:9),ldate,ltime,peso,grua.c_str());
  log.debug("%s: %s",__FILE__, "Fin funcion msjTOS");
  pthread_mutex_unlock( &thread_mutex );
}

} /* namespace container */
