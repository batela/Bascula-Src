/*
 * TOSExplorador.h
 *
 *  Created on: Jan 3, 2016
 *      Author: root
 */

#ifndef TOSEXPLORADOR_H_
#define TOSEXPLORADOR_H_

#include <pthread.h>
#include <string>
#include <vector>

#include "enlace/Enlace.h"
#include "enlace/TOSEnlace.h"
#include "Explorador.h"
namespace container {

class TOSExplorador: public Explorador {
public:
  TOSExplorador();
  virtual ~TOSExplorador();

  TOSExplorador(Enlace* e, Puerto* p);
  TOSExplorador(Enlace* e, Puerto* p, string file);
  TOSExplorador(vector<Enlace*> e, Puerto* p,string file) ;
  int Explora ();
  bool EscribeTramaTOS (bool import, string grua, int peso);

private:
    void LanzarExplorador();
    void ExploraEquipo();
    pthread_mutex_t thread_mutex;
    Config* cfg;
};

} /* namespace container */

#endif /* TOSEXPLORADOR_H_ */
