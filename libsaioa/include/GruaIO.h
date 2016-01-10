/*
 * GruaIO.h
 *
 *  Created on: Nov 15, 2015
 *      Author: root
 */

#ifndef GRUAIO_H_
#define GRUAIO_H_
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "Env.h"

namespace container {
class GruaIO {
public:
  GruaIO();
  virtual ~GruaIO();

  int GetGruaIO (int i);

  int getIo0() const {
    return io0;
  }

  void setIo0(int io0) {
    this->io0 = io0;
  }

  int getIo1() const {
    return io1;
  }

  void setIo1(int io1) {
    this->io1 = io1;
  }

  int getIo2() const {
    return io2;
  }

  void setIo2(int io2) {
    this->io2 = io2;
  }

  int getIo3() const {
    return io3;
  }

  void setIo3(int io3) {
    this->io3 = io3;
  }

  int getIo4() const {
    return io4;
  }

  void setIo4(int io4) {
    this->io4 = io4;
  }

  int getIo5() const {
    return io5;
  }

  void setIo5(int io5) {
    this->io5 = io5;
  }

  int getIo6() const {
    return io6;
  }

  void setIo6(int io6) {
    this->io6 = io6;
  }

  int getIo7() const {
    return io7;
  }

  void setIo7(int io7) {
    this->io7 = io7;
  }

private:
  int io0;
  int io1;
  int io2;
  int io3;
  int io4;
  int io5;
  int io6;
  int io7;

};
}//namespace container
#endif /* GRUAIO_H_ */
