/*
 * MODBUSPuerto.cpp
 *
 *  Created on: Nov 15, 2014
 *      Author: root
 */

#include "../include/MODBUSPuerto.h"
#include <stdio.h>
#include <string.h>
using namespace std;
namespace container {
	extern log4cpp::Category &log;
	MODBUSPuerto::MODBUSPuerto (string id, int br):Puerto(id,RS232) {
		this->baudrate = br ;
		ctx = NULL ;
	}

	MODBUSPuerto::~MODBUSPuerto() {
		if (ctx != NULL ) cerrar();
	}

	int MODBUSPuerto::abrir(){
		log.info("%s: %s %s",__FILE__, "Comienza abrir puerto..",this->getName().data());
		int res = 1 ;
		//struct timeval response_timeout;
		//response_timeout.tv_sec = 1;
		//response_timeout.tv_usec = 0;
		errno= 0;
		if ((ctx = modbus_new_rtu(this->getName().data(), baudrate, 'N', 8, 1)) != NULL){
			log.error("%s: %s %s",__FILE__, "Set slave error: ",modbus_strerror(errno));
			modbus_set_slave(ctx, 0x01);
			//log.error("%s: %s %s",__FILE__, "Set slave error: ",modbus_strerror(errno));
			//modbus_set_response_timeout(ctx, &response_timeout);
			log.error("%s: %s %s",__FILE__, "Set timeout error",modbus_strerror(errno));
			if (modbus_connect(ctx) == -1) {
				log.error("%s: %s %s",__FILE__, "Error al abrir puerto!",modbus_strerror(errno));
				modbus_free(ctx);
				res = 1;
				this->isOpen = false ;
			}
			else {
				this->isOpen = true ;
				res = 0 ;
				log.info("%s: %s",__FILE__, "Puerto abierto!!");
			}
		}
		return res ;
	}

	void MODBUSPuerto::cerrar(){
		modbus_close(ctx);
		modbus_free(ctx);
		this->isOpen = false;
	}

	int MODBUSPuerto::reabrir(){
		this->cerrar();
		return ( this->abrir() );
	}
	/**
	 * Devuelve -1 si hay error.
	 */
	int MODBUSPuerto::leer (int cod, int inicio, int tam,char buffer[]){
		log.debug("%s: %s codigo/inicio %d - %d",__FILE__, "Inicio funcion leer",cod, inicio);
		int count = 0 ;
		switch (cod){
			case 0x01:
				if ((count =  modbus_read_input_bits(ctx,inicio,tam, (unsigned char*) buffer)) == -1){
					log.error("%s: %s %s",__FILE__, "Error leyendo modbuss", modbus_strerror(errno));
				}
			break;
			case 0x02:
				log.warn("%s: %s",__FILE__, "Codigo modbus 0x02 no implementado");
			break;
			case 0x03:
				if ((count =  modbus_read_input_registers(ctx,inicio,tam, (unsigned short int*) buffer)) == -1){
					log.error("%s: %s %s",__FILE__, "Error leyendo modbuss", modbus_strerror(errno));
				}
			break;
			case 0x04:
				if ((count =  modbus_read_registers(ctx,inicio,tam, (unsigned short int*) buffer)) == -1){
					log.error("%s: %s %s",__FILE__, "Error leyendo modbuss", modbus_strerror(errno));
				}
			break;

		}

		log.debug("%s: %s %d",__FILE__, "Fin funcion leer modbus, resultado:", count);
		return count;
	}

	int MODBUSPuerto::escribir (char buffer[], int count){
		log.warn("%s: %s",__FILE__, "Funcion sin implementar");
		return 0;
	}

} /* namespace Container */
