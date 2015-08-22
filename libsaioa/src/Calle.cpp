/*
 * Calle.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: borja
 */


#include "../include/Calle.h"
namespace container {
extern log4cpp::Category &log;

Calle::Calle()
{
}

Calle::~Calle()
{
}

int Calle::CrearRecta()
{
	int res = 0 ;
	float dividendo = Yi-Yf;
	float divisor = Xi-Xf;
	if (divisor == 0) res = 0;
	else {
		pendienteRecta = dividendo/divisor;
		offsetRecta = Yi - pendienteRecta * Xi;
		res = 1;
	}
	return res  ;
}
}
