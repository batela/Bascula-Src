/*
 * include\TOSMsj.h
 *
 *  Created on: Jan 2, 2016
 *      Author: root
 */

#ifndef TOSMSJ_H_
#define TOSMSJ_H_

#include <pthread.h>
#include <string>

using namespace std;
namespace container {

class TOSMsj {
private:

   TOSMsj();              // Private so that it can  not be called
   TOSMsj(TOSMsj const&); // copy constructor is private
   // TOSMsj& operator=(TOSMsj const&){rotacion = 0;};  // assignment operator is private
   static TOSMsj* m_pInstance;
   int rotacion;
   pthread_mutex_t thread_mutex;
public:
   static TOSMsj* Instance();
   void msjTOS(bool import, string grua, int peso,char * msj);

};

} /* namespace container */

#endif /* TOSMSJ_H_ */
