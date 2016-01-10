/*
 * TOSEnlace.h
 *
 *  Created on: Jan 10, 2016
 *      Author: root
 */

#ifndef TOSENLACE_H_
#define TOSENLACE_H_

#include "enlace/Enlace.h"
#include "TOSMsj.h"

namespace container {

class TOSEnlace: public Enlace {
public:
  TOSEnlace();
  virtual ~TOSEnlace();
  bool MsjTOS (bool import, string grua , int peso , char* msj);

};

} /* namespace container */

#endif /* TOSENLACE_H_ */
