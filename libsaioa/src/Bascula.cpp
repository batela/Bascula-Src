/*
 * Bascula.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#include "../include/Bascula.h"

namespace container {

	Bascula::Bascula() {
	 pthread_mutex_init(&mtxBascula, NULL);
	}

	Bascula::~Bascula() {
	}
	/***
	 *
	 */
	int Bascula::GetPeso()
	{
		pthread_mutex_lock(&mtxBascula);
		int valor = 0 ;
		valor = this->peso ;
		pthread_mutex_unlock(&mtxBascula);
		return valor;
	}

	void Bascula::SetIO(int a, int b, int c, int d)
	{
			pthread_mutex_lock(&mtxBascula);
			this->isCarro = a;
			this->isPalpa = b;
			this->isTwisl = c;
			this->isSubir = d;
			pthread_mutex_unlock(&mtxBascula);
	}
	void Bascula::GetIO(int &a, int &b, int &c, int &d)
	{
		pthread_mutex_lock(&mtxBascula);
		a = this->isCarro;
		b = this->isPalpa;
		c = this->isTwisl;
		d = this->isSubir;
		pthread_mutex_unlock(&mtxBascula);
	}

	int Bascula::Actualiza(int peso, char signo)
	{
		pthread_mutex_lock(&mtxBascula);

		this->peso = peso ;
		this->signo = signo;

		pthread_mutex_unlock(&mtxBascula);
	}
	/***
	 *
	 */
	char Bascula::GetSigno()
	{
		pthread_mutex_lock(&mtxBascula);
		char valor = 0 ;
		valor = this->signo ;
		pthread_mutex_unlock(&mtxBascula);
		return valor;
	}
	/**
	 *
	 */
	void Bascula::SetPeso(int peso)
	{
		pthread_mutex_lock(&mtxBascula);
		this->peso = peso;
		pthread_mutex_unlock(&mtxBascula);
	}
	/***
	 *
	 */
	void Bascula::SetSigno(char signo)
	{
		pthread_mutex_lock(&mtxBascula);
		this->signo = signo;
		pthread_mutex_unlock(&mtxBascula);
	}

} /* namespace container */
