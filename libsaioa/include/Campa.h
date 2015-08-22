/*
 * Campa.h
 *
 *  Created on: Oct 2, 2014
 *      Author: borja
 */

#ifndef CAMPA_H_
#define CAMPA_H_
#include 	<iostream>
#include 	<string>
#include 	<stdlib.h>
#include	<fstream>
#include 	<sstream>
#include	<vector>
#include 	<unistd.h>
#include <iomanip>

#include "LatLong-UTMconversion.h"
#include "../include/Calle.h"
#include "../include/Env.h"
#include "../include/PosicionGrua.h"
#include "LL2XY.h"

using namespace std;

namespace container {
class Campa {
public:
	Campa();
	virtual ~Campa();
	int CargarCalles (string a);
	Calle * GetCalle (int i ) {return callesReales[i];}
	void CalcularPosicion (float lat, float lon, PosicionGrua &pg) ;
	int BuscaCalleReferencia (vector <Calle *> calles, double X,double Y,double &linePX,double &linePY,double &distancia);

private:
	int 		elipsoide;
	float 	umbralCalle;
	double	UTMNorthingBase;
	double 	UTMEastingBase;
	char 		UTMZoneBase[4];


	void Inicializa ();
	vector <Calle *>	callesReales;
	vector <Calle *>	callesVirtuales;
	int CargarCallesReales 		(string a);
	int CargarCallesVirtuales ();
	int CalculaCoordenadasVirtuales (Calle * cc,Calle *cv);

	int CrearCalle (vector<string> items);
	vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	vector<std::string> split(const std::string &s, char delim);
};
}
#endif /* CAMPA_H_ */
