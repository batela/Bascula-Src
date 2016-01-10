/*
 * TOSEnlace.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: root
 */

#include "../include/enlace/TOSEnlace.h"

namespace container {
/***
 *
 */
TOSEnlace::TOSEnlace() {

}
/***
 *
 */
TOSEnlace::~TOSEnlace() {

}
/***
 *
 */
bool TOSEnlace::MsjTOS(bool import, string grua, int peso, char* msj)
{
  bool res = false ;
  TOSMsj::Instance()->msjTOS(import, grua, peso, msj);
  if (strlen(msj) != 0) res = true ;
  return res ;
}

} /* namespace container */
