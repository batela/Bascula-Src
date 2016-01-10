/*
 * Puerto.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: borja
 */

#include "Puerto.h"

namespace container {

Puerto::Puerto(string a , PORT_TYPE b) {
	this->name = a;
	this->type = b;
	this->isOpen = false;

}

Puerto::~Puerto() {
	// TODO Auto-generated destructor stub
}

} /* namespace container */
