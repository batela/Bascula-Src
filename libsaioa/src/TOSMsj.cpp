/*
 * src\TOSMsj.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */

#include "../include/TOSMsj.h"

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
  rotacion = 0;
}
/**
 *
 */
TOSMsj::TOSMsj()
{
  thread_mutex = PTHREAD_MUTEX_INITIALIZER;
  rotacion = 0;
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

/**
 *
 */
void TOSMsj::msjTOS(bool import, string grua, int peso, char * msj)
{
  pthread_mutex_lock( &thread_mutex );
  log.debug("%s: %s",__FILE__, "Comienza funcion msjTOS");

  sprintf (msj,"[%d%d %d %d %d %s]\n",rotacion,(import?1:2),20151231,125959,peso,grua.c_str());

   log.debug("%s: %s",__FILE__, "Fin funcion msjTOS");
  pthread_mutex_unlock( &thread_mutex );
}

} /* namespace container */
