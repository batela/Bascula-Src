/*
 * GPS.h
 *
 *  Created on: Sep 26, 2014
 *      Author: borja
 */

#ifndef GPS_H_
#define GPS_H_
#include <string>
#include "time.h"
using namespace std;
namespace container {

class GPS {
public:
	GPS();
	virtual ~GPS();
	int getFecha ()			{return fecha;};
	float getLatitud ()	{return latitud;};
	float getLongitud (){return longitud;};
	float getAltitud ()	{return altitud;};
	char getCLatitud ()	{return clatitud;};
	char getCLongitud (){return clongitud;};
	int getSatelites ()	{return satelites;};
	int getCalidad ()		{return calidad;};
	//time getTFecha () 	{return tfecha;};

	void setFecha 		(int a) 	{ fecha = a;  };
	void setCalidad 	(int a) 	{ calidad = a;  };
	void setLatitud 	(float a) 	{ latitud = a; };
	void setLongitud 	(float a) 	{ longitud = a; };
	void setAltitud 	(float a) 	{ altitud = a; };
	void setCLatitud 	(char a) { clatitud = a; };
	void setCLongitud (char a) { clongitud = a; };
	void setSatelites (int a)  {satelites = a;} ;
private:
	int 	fecha;
	float latitud;
	float longitud;
	float altitud;
	char 	clatitud;
	char 	clongitud;
	int 	satelites;
	int 	calidad;
	/*
	0 = invalid  1 = GPS fix (SPS)  2 = DGPS fix  3 = PPS fix 4 = Real Time Kinematic
	5 = Float RTK  6 = estimated (dead reckoning) (2.3 feature) 7 = Manual input mode 8 = Simulation mode
	*/

};

} /* namespace container */
#endif /* GPS_H_ */
