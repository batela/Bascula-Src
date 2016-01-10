/*
 * Bascula.h
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#ifndef BASCULA_H_
#define BASCULA_H_
#include <ctime>
#include <pthread.h>
using namespace std;
namespace container {

class Bascula {
public:
	Bascula();
	virtual ~Bascula();

	int GetPeso() 	;
	char GetSigno()	;
	void SetPeso(int peso)	;
	void SetIO(int a, int b, int c, int d)	;
	void SetGruaIO(int a, int b, int c, int d,int e, int f, int g, int h);
	void GetIO(int &a, int &b, int &c, int &d);
	void GetGruaIO(int &a, int &b, int &c, int &d,int &e, int &f, int &g, int &h);
	void SetSigno(char signo) ;
	int Actualiza(int peso, char signo);

	time_t GetUltimaActualizacion() const {		return ultimaActualizacion;	}
	void GetUltimaActualizacion(time_t ultimaActualizacion) {		this->ultimaActualizacion = ultimaActualizacion;	}
	void SetEstable (bool a) 	{isEstable = a;};
	bool GetEstable () 				{return isEstable ;};
private:
	int peso;
	char signo;
	int isCarro;
	int isPalpa;
	int isTwisl;
	int isSubir;
	int isIOg0;
	int isIOg1;
	int isIOg2;
	int isIOg3;
	int isIOg4;
	int isIOg5;
	int isIOg6;
	int isIOg7;


	time_t ultimaActualizacion;
	bool isEstable;
	pthread_mutex_t mtxBascula;
};

} /* namespace container */

#endif /* BASCULA_H_ */
