/*
 * RS232Puerto.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: borja
 */

#include "RS232Puerto.h"

namespace container {
extern log4cpp::Category &log;


	RS232Puerto::RS232Puerto(string id,int baudRate) : Puerto(id,RS232)
	{
		configuraVelocidad(baudRate);
		this->charSize = SerialStreamBuf::CHAR_SIZE_8;
		this->parity =   SerialStreamBuf::PARITY_NONE;
		this->stopBits = 1;
	}

	RS232Puerto::RS232Puerto(string id,int baudRate,int charSize,int parity,int stopBits) : Puerto(id,RS232)
	{
		configuraVelocidad(baudRate);
		configuraBitsDatos(charSize);
		configuraParidad(parity);
		this->stopBits = stopBits;
	}

	RS232Puerto::~RS232Puerto() {

	}

	int RS232Puerto::abrir(){
		log.info("%s: %s %s",__FILE__, "Comienza abrir puerto..",this->getName().data());
		serial_port.Open( this->getName()) ;
		if ( ! serial_port.good() ) {
			return -1;
		}
		// Default configuration
		serial_port.SetBaudRate( this->baudRate ) ;
		if ( ! serial_port.good() ){
			return(-1) ;
		}
		//
		// Set the number of data bits.
		//
		serial_port.SetCharSize( this->charSize) ;
		if ( ! serial_port.good() ){
			return(-1) ;
		}
		//
		// Disable parity.
		//
		serial_port.SetParity( this->parity ) ;
		if ( ! serial_port.good() ){
			return(-1) ;
		}
		//
		// Set the number of stop bits.
		//
		serial_port.SetNumOfStopBits( 1 ) ;
		if ( ! serial_port.good() ){
			return(-1) ;
		}
		//
		// Turn on hardware flow control.
		//

		serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
		if ( ! serial_port.good() ){
			return(-1) ;
		}
		serial_port.SetVMin(0);
		serial_port.SetVTime(0);
		this->setIsOpen(true);
		log.info("%s: %s",__FILE__, "Puerto abierto!!");
		return 0 ;
	}

	void RS232Puerto::cerrar(){
		serial_port.Close();
		this->setIsOpen(false);
	}

	int RS232Puerto::reabrir(){
		this->cerrar();
		return ( this->abrir() );
	}
	/**
	 * Devuelve -1 si hay error.
	 */
	int RS232Puerto::leer (char buffer[]){
		int count = 0 ;
		int res = 0;

		while( (res = serial_port.rdbuf()->in_avail()) > 0  ){
			char next_byte;
			serial_port.get(next_byte);
			if (count < 255) buffer[count++] = next_byte;
			if (next_byte =='\n') break;
		}
		if (count >= 255 || res < 0 ) count = -1;
		buffer[count]= 0 ;
		return count;
	}

	int RS232Puerto::leerSimple (char &data){
			int res = 0;

			if( (res = serial_port.rdbuf()->in_avail()) > 0  ){
				serial_port.get(data);
			}

			return res;
	}
	int RS232Puerto::escribir (char buffer[], int count){
			serial_port.write( buffer, count ) ;
			return 0;
	}

	void RS232Puerto::configuraVelocidad (int a){
		switch (a){
			case 1200:
					this->baudRate = SerialStreamBuf::BAUD_1200 ;
			break;
			case 9600:
				this->baudRate = SerialStreamBuf::BAUD_9600 ;
			break;
			case 115200:
				this->baudRate = SerialStreamBuf::BAUD_115200 ;
			break;
			case 19200:
				this->baudRate = SerialStreamBuf::BAUD_19200 ;
			break;
			default:
				log.error("%s: %s %d %s",__FILE__, "Velocidad de puerto no definida: ", a , " usando 9600");
				this->baudRate = SerialStreamBuf::BAUD_9600 ;
			break;
		}
	}
	void RS232Puerto::configuraParidad (int a){
		switch (a){
			case 0:
				this->parity = SerialStreamBuf::PARITY_NONE;
			break;
			case 1:
				this->parity = SerialStreamBuf::PARITY_ODD;
			break;
			case 2:
				this->parity = SerialStreamBuf::PARITY_EVEN;
			break;
			default:
				log.error("%s: %s %d %s",__FILE__, "Paridad de puerto no definida: ", a , " usando PAR");
				this->parity = SerialStreamBuf::PARITY_NONE ;
			break;
		}
	}

	void RS232Puerto::configuraBitsDatos (int a){
		switch (a){
			case 7:
				this->charSize = SerialStreamBuf::CHAR_SIZE_7;
			break;
			case 8:
				this->charSize = SerialStreamBuf::CHAR_SIZE_8;
			break;
			default:log.error("%s: %s %d %s",__FILE__, "Bits de Datos de puerto no definida: ", a , " usando 8");
				this->charSize = SerialStreamBuf::CHAR_SIZE_8 ;
			break;
		}

	}

} /* namespace container */
