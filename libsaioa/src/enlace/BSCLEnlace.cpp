/*
 * BSCLEnlace.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#include "../../include/enlace/BSCLEnlace.h"

namespace container {
extern log4cpp::Category &log;

BSCLEnlace::BSCLEnlace( int pesajescorrectos, int margenpesaje) {
	SetStartByte(0x02);
	SetEndByte(0x03);
	numeroPesaje  		= 0 ;
	pesajesCorrectos 	= pesajescorrectos;
	margenPesaje 			= margenpesaje;
}

BSCLEnlace::~BSCLEnlace() {
}

void BSCLEnlace::Configure(string a , string b) {
	pesajesCorrectos 	= atoi(a.data());
	margenPesaje 		= atoi(b.data());
}

int BSCLEnlace::analizaTrama(char *buffer){
	int res = -1;
	char peso[10];
	if (VerificaTrama(buffer) == 0){
		memset (peso,0,10);
		memcpy (peso, &buffer[3],5);
		completaBSCL(atoi(peso),buffer[2],this->bascula);
	}
	else {
		log.error("%s: %s",__FILE__, "Trama Bascula incorrecta");
	}
	return res;
}

int BSCLEnlace::VerificaTrama (char *buffer)
{
	int res = 0 ;
	//log.info("%s: %s",__FILE__, "Comenzando funcion..");
	if ((buffer[0] != 0x02) || strlen(buffer) != 9 || (buffer[8] != 0x03)) {
		log.warn("%s: %s",__FILE__, "Trama recibida erronea");
		res = 1;
	}
	//log.info("%s: %s",__FILE__, "Terminando funcion!!");
	return res;
}

int BSCLEnlace::completaBSCL (int peso, char signo, Bascula &bsc){
	log.debug("%s: %s",__FILE__, "Comenzando funcion.. completar bascula");
	static int ultimoPeso = 0 ;
	if (peso != ultimoPeso){
		log.info("%s: %s %c%d",__FILE__, "***************Actualiza bascula: " , signo,peso);
		bsc.Actualiza(peso,signo);
		ultimoPeso = peso;
	}

	log.debug("%s: %s",__FILE__, "Terminando funcion!!");
	return 0;
}

int BSCLEnlace::completaBSCL (vector<std::string> items, Bascula &bsc){
	log.debug("%s: %s",__FILE__, "Comenzando funcion.. completar bascula");
	static int ultimoPeso = 0 ;
	int peso = atoi(items[2].data());
	if (peso != ultimoPeso){

		bsc.Actualiza(peso,items[0].at(0));
	}

	log.debug("%s: %s",__FILE__, "Terminando funcion!!");
	return 0;
}
/*
int BSCLEnlace::completaBSCL (vector<std::string> items, Bascula &bsc){
	log.debug("%s: %s",__FILE__, "Comenzando funcion.. completar bascula");

	int peso = atoi(items[2].data());
	bsc.SetSigno(items[0].at(0));
	if ((peso > 0) && (abs(peso - bsc.GetPeso()) < margenPesaje)) {
		if (numeroPesaje++ >= pesajesCorrectos){
			bsc.SetEstable(true);
			bsc.SetPeso (peso);
			log.info("%s: %s %d",__FILE__, "Identificado pesaje correcto::>>" , peso);
			numeroPesaje = 0;
		}
	}
	else{
		numeroPesaje = 0 ;
		bsc.SetEstable(false);
		bsc.SetPeso (peso);
	}
	//bsc.SetSigno((char)items[1].data());
	log.debug("%s: %s",__FILE__, "Terminando funcion!!");
	return 0;
}
*/
} /* namespace container */
