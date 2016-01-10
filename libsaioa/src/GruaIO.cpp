/*
 * GruaIO.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: root
 */

#include "../include/GruaIO.h"
namespace container {
extern log4cpp::Category &log;
  GruaIO::GruaIO() {
  }

  GruaIO::~GruaIO() {
  }

  int GruaIO::GetGruaIO (int i){
    int res = 0;
    switch (i){
      case 0: res = this->getIo0(); break;
      case 1: res = this->getIo1(); break;
      case 2: res = this->getIo2(); break;
      case 3: res = this->getIo3(); break;
      case 4: res = this->getIo4() ; break;
      case 5: res = this->getIo5(); break;
      case 6: res = this->getIo6(); break;
      case 7: res = this->getIo7(); break;
      default: res = 0; break;
    }
    return res;
  }
}

