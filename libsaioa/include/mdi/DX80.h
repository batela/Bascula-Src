/*
 * DX80.h
 *
 *  Created on: May 1, 2015
 *      Author: root
 */

#ifndef DX80_H_
#define DX80_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Env.h"
using namespace std;
namespace container {

class DX80 {
public:
	DX80();
	virtual ~DX80();
	int getInput1 (){ return valores[0];}
	int getInput2 (){ return valores[1];}
	int getInput3 (){ return valores[2];}
	int getInput4 (){ return valores[3];}

	int getPeso (){ return peso;}
	int getPesoValido (){ return pesoValido;}
	float getPeso1 (){ return peso1;}
	float getPeso2 (){ return peso2;}
	float getPeso3 (){ return peso3;}
	float getPeso4 (){ return peso4;}

	bool getIsOKInput1 (){ return isOKInput1;}
	bool getIsOKInput2 (){ return isOKInput2;}
	bool getIsOKInput3 (){ return isOKInput3;}
	bool getIsOKInput4 (){ return isOKInput4;}
	bool getIsOKRadio(){ return isOKRadio;}
	bool getIsOKMaster(){ return isOKMaster;}
	int  getCMX() {return cmX;}
	int  getCMY() {return cmY;}

	void setInput1(int a) {valores[0] = a;}
	void setInput2(int a) {valores[1] = a;}
	void setInput3(int a) {valores[2] = a;}
	void setInput4(int a) {valores[3] = a;}

	void setPeso(int a) {peso = a;}
	void setPesoValido(int a) {pesoValido = a;}
	void setPeso1(float a) {peso1 = a;}
	void setPeso2(float a) {peso2 = a;}
	void setPeso3(float a) {peso3 = a;}
	void setPeso4(float a) {peso4 = a;}

	void setIsOKInput1(bool a) {isOKInput1 = a;}
	void setIsOKInput2(bool a) {isOKInput2 = a;}
	void setIsOKInput3(bool a) {isOKInput3 = a;}
	void setIsOKInput4(bool a) {isOKInput4 = a;}
	void setIsOKRadio(bool a)  {isOKRadio = a;}
	void setIsOKMaster(bool a)  {isOKMaster = a;}

	int  getValorIdx (int i) {return valores[i];}
	char getSigno () {return signo;}
	void setSigno (char a) {signo = a;}
	void setCMX (int a ) {cmX = a;}
	void setCMY (int a ) {cmY = a;}

	void setIsFijo (bool a) {isFijo = a;}
	bool getIsFijo (){return isFijo;}

	void CalculaAlarmas();
	void Configure();
private:

	int   valores[4];
	bool  alarmas[4];
	int peso;
	int pesoValido;
	float peso1;
	float peso2;
	float peso3;
	float peso4;
	char signo;
	bool isOKInput1;
	bool isOKInput2;
	bool isOKInput3;
	bool isOKInput4;
	bool isOKRadio;
	bool isOKMaster;
	int cmX;
	int cmY;
	int tolCM;
	float tolPeso;
	bool isFijo;
	int offSetPeso;
};

} /* namespace container */

#endif /* DX80_H_ */
