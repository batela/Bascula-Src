/*
 * DX80Enlace.cpp
 *
 *  Created on: May 1, 2015
 *      Author: root
 */

#include "../include/DX80Enlace.h"

namespace container {
extern log4cpp::Category &log;
/***
 *
 */
DX80Enlace::DX80Enlace() {
	cfg = NULL;

}
/***
 *
 */
DX80Enlace::~DX80Enlace() {
}
void DX80Enlace::Configure (string a){

	dx.Configure();
  pma = atoi(Env::getInstance()->GetValue("pesomaximo").data());
	offsetpeso = atoi(Env::getInstance()->GetValue("offsetpeso").data());
}
/**
 * res = 0 correcto res != error
 */
int DX80Enlace::analizaTrama (char *buffer,int tipo){
	log.debug("%s: %s",__FILE__, "Comienza funcion AnalizaTrama");

	int res = 1;
	if (tipo == 0){
	  int peso = VerificaTrama(buffer) ;
	  if (peso != -1028) res = 0;
	    dx.setPeso(peso);
	}
	else if (tipo == 1) {
	  VerificaEstadoRadio(buffer);
	}

	log.debug("%s: %s",__FILE__, "Fin de funcion AnalizaTrama");
	return res;

}

int DX80Enlace::trataError (){
  log.debug("%s: %s",__FILE__, "Comienza funcion TrataError");

  int res = 1;
  dx.setPeso1(0);
  dx.setPeso2(0);
  dx.setPeso3(0);
  dx.setPeso4(0);
  dx.setPeso(0);
  dx.setIsOKMaster(false);
  log.debug("%s: %s",__FILE__, "Fin de funcion TrataError");
  return res;

}

void DX80Enlace::VerificaEstadoRadio (char* buffer){
  log.debug("%s: %s",__FILE__, "Comienza funcion VerificaEstadoRadio");

  dx.setIsOKMaster(true);

  int isOKRadio = (unsigned short)(256* (unsigned char) buffer[1] + (unsigned char) buffer[0]);
  if (isOKRadio > 0)
    dx.setIsOKRadio(true);
  else
    dx.setIsOKRadio(false);

  log.debug("%s: %s",__FILE__, "Fin de funcion VerificaEstadoRadio");
}

/***
 * res = 0, todos los pesos correctos, res =1 un peso erroneo...
 */
int DX80Enlace::VerificaTrama (char *buffer){
	log.debug("%s: %s",__FILE__, "Comienza funcion VerificaTrama");

	dx.setIsOKMaster(true);

	dx.setInput1((unsigned short)(256* (unsigned char) buffer[1] + (unsigned char) buffer[0]));
	dx.setInput2((unsigned short)(256* (unsigned char) buffer[3] + (unsigned char) buffer[2]));
	dx.setInput3((unsigned short)(256* (unsigned char) buffer[5] + (unsigned char) buffer[4]));
	dx.setInput4((unsigned short)(256* (unsigned char) buffer[7] + (unsigned char) buffer[6]));
	log.info("%s: Entradas: %d - %d - %d - %d",__FILE__, dx.getInput1() ,dx.getInput2() , dx.getInput3() ,dx.getInput4());
	int res = CalculaPeso();

	float cmX = 0;
	float cmY = 0;
	if (res != 0){
	  cmX = (dx.getPeso1() + (-1)* dx.getPeso2() + (-1)*dx.getPeso3() + dx.getPeso4()) / (dx.getPeso1() + dx.getPeso2() + dx.getPeso3() + dx.getPeso4());
	  cmY = (dx.getPeso1() + dx.getPeso2() + (-1)*dx.getPeso3() + (-1)*dx.getPeso4()) / (dx.getPeso1() + dx.getPeso2() + dx.getPeso3() + dx.getPeso4());
	}
	dx.setPeso(res);
	dx.setCMX(cmX*100);
	dx.setCMY(cmY*100);

	log.debug("%s: %s %d Pesos individuales : %.1f - %.1f - %.1f - %.1f",__FILE__, "Peso calculado: " , res, dx.getPeso1() , dx.getPeso2() , dx.getPeso3() , dx.getPeso4());
	log.debug("%s: %s %.1f %s %.1f",__FILE__, "Centro de Masas X: " , cmX, " Y: ", cmY);
	log.debug("%s: %s",__FILE__, "Fin de funcion VerificaTrama");
	return res;
}



/**
 *
 */
int DX80Enlace::CalculaPeso(){
	//En esta fase asignamos a cada celula la que corresponda por configuracion
	int indiceCelula = atoi(Env::getInstance()->GetValue("activacelula1").data()) - 1;
	if (indiceCelula >= 0 && indiceCelula <= 3) dx.setInput1(dx.getValorIdx(indiceCelula));

	indiceCelula = atoi(Env::getInstance()->GetValue("activacelula2").data()) - 1;
	if (indiceCelula >= 0 && indiceCelula <= 3) dx.setInput2(dx.getValorIdx(indiceCelula));

	indiceCelula = atoi(Env::getInstance()->GetValue("activacelula3").data()) - 1;
	if (indiceCelula >= 0 && indiceCelula <= 3) dx.setInput3(dx.getValorIdx(indiceCelula));

	indiceCelula = atoi(Env::getInstance()->GetValue("activacelula4").data()) - 1;
	if (indiceCelula >= 0 && indiceCelula <= 3) dx.setInput4(dx.getValorIdx(indiceCelula));

	//Finalmente se hace el escalado
	float aux =  ((float)dx.getInput1() * (float)20)/ (float)65535 ; //pasar a Amperios
  dx.setPeso1((((float)pma * aux)/16.0) - (float)offsetpeso);

  aux =  ((float)dx.getInput2() * (float)20)/ (float)65535 ;
  dx.setPeso2((((float)pma * aux)/16.0) - (float)offsetpeso);

  aux =  ((float)dx.getInput3() * (float)20)/ (float)65535 ;
  dx.setPeso3((((float)pma * aux)/16.0) - (float)offsetpeso);

  aux =  ((float)dx.getInput4() * (float)20)/ (float)65535 ;
  dx.setPeso4((((float)pma * aux)/16.0) - (float)offsetpeso);

	return (dx.getPeso1() + dx.getPeso2() +dx.getPeso3() + dx.getPeso4());
}

int DX80Enlace::Redondea(int num)
{
     int rem = num % 10;
     return rem >= 5 ? (num - rem + 10) : (num - rem);
}
} /* namespace container */
