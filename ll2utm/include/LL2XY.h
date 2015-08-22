/*
 * LL2XY.h
 *
 *  Created on: Sep 22, 2014
 *      Author: borja
 */

#ifndef LL2XY_H_
#define LL2XY_H_
#include <string>
#include <string.h>
#include <math.h>

#include "llstr.h"

using namespace std;
namespace container {



class LL2XY {
public:
	LL2XY();
	LL2XY(float lat, float lon);
	virtual ~LL2XY();

	float getLatitude()	{return Olatitude;}
	float getLongitude(){return Olongitude;}
	float getX(){return X;}
	float getY(){return Y;}

	void setLatitude (float a) 	{Olatitude = a;}
	void setLongitude (float a) {Olongitude = a ;}
	void setX (float a) {X = a ;}
	void setY (float a) {Y = a ;}
	double findDistanceToSegment(double x1, double y1, double x2, double y2, double pointX, double pointY,double &linePX, double &linePY);
	void getDistance (string lat, string lon, float &d);
	double findDistanceP2P(double x1, double y1, double x2, double y2) ;
private:
	float Olatitude; //en grados decimales
	float Olongitude; //en grados decimales
	float X;
	float Y;
};

} /* namespace container */
#endif /* LL2XY_H_ */
