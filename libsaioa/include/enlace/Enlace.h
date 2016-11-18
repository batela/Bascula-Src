/*
 * Enlace.h
 *
 *  Created on: Sep 25, 2014
 *      Author: borja
 */

#ifndef ENLACE_H_
#define ENLACE_H_
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "../Env.h"
#include "configini.h"
using namespace std;
namespace container {

class Enlace {
public:
	Enlace();
	virtual ~Enlace();
	virtual int analizaTrama(char * buffer,int tipo = 0){return 0;}
	virtual int trataError(){return 0;};
	string getItemCfg (string section , string a) ;
	int getItemCnt (string section) {return (ConfigGetKeyCount(this->cfg,section.data()));}

	virtual int VerificaTrama (char buffer[]){return 0;};
	void SetStartByte	(char a) { startByte = a;} ;
	void SetEndByte		(char a) { endByte = a;} ;
	char GetStartByte	() { return startByte ;} ;
	char GetEndByte		() { return endByte ;} ;

	char txbuffer[256];
	char rxbuffer[256];
	bool GetIsFalloCom ()        {return isFalloCom ;};
	void SetIsFalloCom (int a)        {isFalloCom = a ;};
protected:

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;
	std::vector<std::string> split(const std::string &s, char delim);
	Config *cfg;
private:
	char startByte;
	char endByte;
	bool isFalloCom ;

};

} /* namespace container */
#endif /* ENLACE_H_ */
