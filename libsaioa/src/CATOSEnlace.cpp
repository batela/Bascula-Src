/*
 * GPSEnlace.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#include "../include/CATOSEnlace.h"


namespace container {
extern log4cpp::Category &log;

CATOSEnlace::CATOSEnlace() {
	// TODO Auto-generated constructor stub
}

CATOSEnlace::~CATOSEnlace() {
	// TODO Auto-generated destructor stub
}
/**
 * Devuelve: -1 si la trama es desconcocida, 1 si eNOATUMEnlacel tratamiento de la trama es erroneo, 0 si va bine
 *
 */
int CATOSEnlace::analizaTrama(char buffer[]){
	log.error("%s: %s",__FILE__, "Funcion sin implementar..");
	return -1;
}

/**
 * '{'
	'0' Rotativo 0-1
	'2' 1 Cojer 2 Dejar
	'1' Fijo
	'B''8' Zona
	'6''5' Calle
	'0''2' Fila
	' ' Fijo
	'1' Altura
	'2' N.A
	'1'	'1'	'1'	'1'	'1'	'1'	'1' Fijo
	'A'	'D'	'P'	'R'	'a'	'a'	'w'	'u' N.a
	'}'
	'$r' CR
 *
 */
int CATOSEnlace::crearTrama(int accion, PosicionGrua pg, char buffer[]){
	log.error("%s: %s",__FILE__, "FuncCATOSion sin implementar..");
	return -1;
}

std::vector<std::string> & CATOSEnlace::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> CATOSEnlace::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
} /* namespace container */
