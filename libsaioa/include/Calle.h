/*
 * Calle.h
 *
 *  Created on: Oct 2, 2014
 *      Author: borja
 */

#ifndef CALLE_H_
#define CALLE_H_
#include "../include/Env.h"

namespace container {
class Calle {
public:
	Calle();
	virtual ~Calle();

	int getCalle() const 	{	return calle;	}
	void setCalle(int a1) 	{calle = a1;	}
	char getZona() const 	{	return zona;	}
	void setZona(char a2) 	{	zona = a2;	}
	float getA3() const 	{	return a3;	}
	void setA3(float a) 	{	a3 = a;	}
	float getA4() const 	{	return a4;	}
	void setA4(float a) 	{	a4 = a;	}
	float getEspacioContainer() const {		return espacioContainer;	}
	void setEspacioContainer(float a) {	espacioContainer = a;	}
	float getXf() const 	{	return Xf;	}
	void setXf(float xf) 	{	Xf = xf;	}
	float getXi() const 	{	return Xi;	}
	void setXi(float xi)	{	Xi = xi;	}
	float getYf() const 	{	return Yf;	}
	void setYf(float yf) 	{	Yf = yf;	}
	float getYi() const 	{	return Yi;	}
	void setYi(float yi) 	{	Yi = yi;	}
	int getId() {return id;};
	void setId(int a) {id = a;};
	float getPendiente() {return pendienteRecta ;}
	void setPendiente( float a) {pendienteRecta = a ;}

	int CrearRecta();
private:
	int id;
	float Xi;
	float Yi;
	float Xf;
	float Yf;
	float espacioContainer;
	int  	calle;
	char  zona;
	float a3;
	float a4;
	//Parametros para definir la recta
	float pendienteRecta;
	float offsetRecta;
};
}
#endif /* CALLE_H_ */
