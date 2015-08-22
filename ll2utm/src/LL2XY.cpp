/*
 * LL2XY.cpp
 *
 *  Created on: Sep 22, 2014
 *      Author: borja
 */

#include "../include/LL2XY.h"

namespace container {

LL2XY::LL2XY() {
	this->Olatitude 	= 0;
	this->Olongitude = 0;
	this->X = 0;
	this->Y = 0;
}

LL2XY::LL2XY(float lat, float lon) {
	this->Olatitude 	= lat;
	this->Olongitude = lon;
	this->X = 0;
	this->Y = 0;
}

LL2XY::~LL2XY() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * Pasamos las variable por referencia.
 */
/*
	Haversine
	formula: 	a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
	c = 2 ⋅ atan2( √a, √(1−a) )
	d = R ⋅ c
	where 	φ is latitude, λ is longitude, R is earth’s radius (mean radius = 6,371km);
	note that angles need to be in radians to pass to trig functions!
	JavaScript:

	var R = 6371; // km
	var φ1 = lat1.toRadians();
	var φ2 = lat2.toRadians();
	var Δφ = (lat2-lat1).toRadians();
	var Δλ = (lon2-lon1).toRadians();

	var a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
	        Math.cos(φ1) * Math.cos(φ2) *
	        Math.sin(Δλ/2) * Math.sin(Δλ/2);
	var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

	var d = R * c;
 */
/**
 * La llamada es:
 * 		norting inicio
 * 		easting inicio
 * 		norting final
 * 		easting final
 * 		norting punto
 * 		easting punto
 */
double LL2XY::findDistanceToSegment(double x1, double y1, double x2, double y2, double pointX, double pointY, double &linePX, double &linePY)
{
    double diffX = x2 - x1;
    float diffY = y2 - y1;
    if ((diffX == 0) && (diffY == 0))
    {
        diffX = pointX - x1;
        diffY = pointY - y1;
        return sqrt(diffX * diffX + diffY * diffY);
    }

    float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);

    if (t < 0)
    {
        //point is nearest to the first point i.e x1 and y1
        diffX = pointX - x1;
        diffY = pointY - y1;
        linePX = x1 ;
        linePY = y1 ;
    }
    else if (t > 1)
    {
        //point is nearest to the end point i.e x2 and y2
        diffX = pointX - x2;
        diffY = pointY - y2;
        linePX = x2 ;
        linePY = y2 ;
    }
    else
    {
        //if perpendicular line intersect the line segment.
        diffX = pointX - (x1 + t * diffX);
        diffY = pointY - (y1 + t * diffY);
        linePX = (x1 + t * diffX);
        linePY = (y1 + t * diffY);
    }

    //returning shortest distance
    return sqrt(diffX * diffX + diffY * diffY);
}
/**
 * Distancia entre dos puntos
 */
double LL2XY::findDistanceP2P(double x1, double y1, double x2, double y2)
{
    double 	diffX = x2 - x1;
    float 	diffY = y2 - y1;
    return sqrt(diffX * diffX + diffY * diffY);
}

/*
 *
 */
void LL2XY::getDistance (string lat, string lon, float &d){
	float a = 0;
	float c = 0;
	int R = 6371;

	float rLat;
	float rLon;
	CLongLatString strLat(lat,"HDDMM.mmm",LL_LATITUDE);
	CLongLatString strLon(lon,"HDDDMM.mmm",LL_LONGITUDE);


	float gLat = strLat.GetDecimalDegree();
	float gLon = strLon.GetDecimalDegree();

	rLat = gLat * 180 / 3.14159265;
	rLon = gLon * 180 / 3.14159265;
	float rOLat = Olatitude * 180 / 3.14159265;; //en radianes
	float rOLon = Olongitude* 180 / 3.14159265;; //en radianes

	float dLat = rLat - rOLat;
	float dLon = rLon - rOLon;
	a = sin (dLat/2) * sin (dLat/2) + cos (rOLat) * cos (rLat) * sin (dLon/2) * sin (dLon/2);
	c = 2* atan2(sqrt(a),sqrt(1-a));
	d =(R * c);
}

} /* namespace container */
