/*
 * DX80Enlace.h
 *
 *  Created on: May 1, 2015
 *      Author: root
 */

#ifndef DX80ENLACE_H_
#define DX80ENLACE_H_


#include "DX80.h"
#include "enlace/Enlace.h"

namespace container {

class DX80Enlace: public Enlace {
public:
	DX80Enlace();
	virtual ~DX80Enlace();
	int analizaTrama (char *buffer, int tipo=0);
	int trataError ();
	int VerificaTrama (char *buffer);
	void VerificaEstadoRadio (char* buffer);
	void Configure (string a);
	DX80 * getDX () {return &dx;} ;
private:
	  float lecturaEnVacio ;
	  float lecturaEnPeso  ;
	  float lecturaEnPeso1 ;
	  float lecturaEnPeso2 ;
	  float lecturaEnPeso3 ;
	  float lecturaEnPeso4 ;
	  float pesoConocido   ;
	  float pendiente  ;
	  float pendiente1 ;
	  float pendiente2 ;
	  float pendiente3 ;
	  float pendiente4 ;

	int  CalculaPeso();
	int  CalculaPesoCalibrado();
	void CalculaAlarmas();
	int Redondea(int num);
	DX80 dx;

	int pma;
	int offsetpeso;
	int precisionpesada;

};

} /* namespace container */

#endif /* DX80ENLACE_H_ */
