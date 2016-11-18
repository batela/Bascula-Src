/*
 * Puerto.h
 *
 *  Created on: Sep 21, 2014
 *      Author: borja
 */

#ifndef PUERTO_H_
#define PUERTO_H_

#include <iostream>
using namespace std;

namespace container {

class Puerto {

public:
	enum PORT_TYPE {RS232, I2C,TCPIP,UARTCAN} ;
	Puerto(string a, PORT_TYPE b );
	virtual ~Puerto();

	bool getIsOpen ()				{return isOpen;}
	void setIsOpen (bool a) {isOpen = a;}
	string getName ()				{return name ;}
	void setName (string a)	{name = a;}
	virtual int 	abrir ()	{return 0 ;}
	virtual void cerrar ()	{};
	virtual int escribir (char buffer[], int count){return 0;}
	virtual int leer (char buffer[]){return 0 ;}
protected:
	bool 			isOpen;
	PORT_TYPE type;
	string 		name ;

};

} /* namespace container */
#endif /* PUERTO_H_ */
