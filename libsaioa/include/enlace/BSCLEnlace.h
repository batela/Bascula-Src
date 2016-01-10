/*
 * BSCLEnlace.h
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#ifndef BSCLENLACE_H_
#define BSCLENLACE_H_

#include "../Bascula.h"
#include "Enlace.h"
namespace container {

class BSCLEnlace: public Enlace {
public:
	BSCLEnlace(int a , int b);
	virtual ~BSCLEnlace();
	int analizaTrama (char *buffer);
	int VerificaTrama (char *buffer);
	Bascula * getBSCL () {return &bascula;} ;
	void Configure (string a, string b);
private:
	int completaBSCL (vector<std::string> items, Bascula &gps);
	int completaBSCL (int peso, char signo, Bascula &bsc);
	int numeroPesaje ;
	int margenPesaje;
	int pesajesCorrectos;

	Bascula bascula;

};

} /* namespace container */

#endif /* BSCLENLACE_H_ */
