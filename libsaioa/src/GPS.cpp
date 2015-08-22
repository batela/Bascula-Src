/*
 * GPS.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: borja
 */

#include "../include/GPS.h"

namespace container {

GPS::GPS() {
	fecha 		= 0;
	latitud 	= 0;
	longitud	= 0;
	altitud		= 0;
	clatitud	= 0;
	clongitud = 0;
	satelites	= 0;
	calidad 	= 0;

}

GPS::~GPS() {
}

} /* namespace container */
