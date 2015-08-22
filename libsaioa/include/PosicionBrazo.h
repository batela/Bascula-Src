/*
 * PosicionBrazo.h
 *
 *  Created on: Nov 15, 2014
 *      Author: root
 */

#ifndef POSICIONBRAZO_H_
#define POSICIONBRAZO_H_

namespace container {

class PosicionBrazo {
public:
	PosicionBrazo();
	virtual ~PosicionBrazo();

	float getAngulo() const {
		return angulo;
	}

	void setAngulo(float angulo) {
		this->angulo = angulo;
	}

	bool isLocks() const {
		return locks;
	}

	void setLocks(bool locks) {
		this->locks = locks;
	}

	float getLongitud() const {
		return longitud;
	}

	void setLongitud(float longitud) {
		this->longitud = longitud;
	}

private:
	float longitud;
	float angulo;
	bool locks;
};

} /* namespace container */

#endif /* POSICIONBRAZO_H_ */
