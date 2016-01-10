/*
 * Bascula.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: root
 */

#include "../include/Bascula.h"

dsfsfs
namespace container {
  /**
   *
   */
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
	/**
	 *
	 */
	void Bascula::SetIO(int a, int b, int c, int d)
	{
			pthread_mutex_lock(&mtxBascula);
			this->isCarro = a;
			this->isPalpa = b;
			this->isTwisl = c;
			this->isSubir = d;
			pthread_mutex_unlock(&mtxBascula);
	}
	/**
	 *
	 */
	void Bascula::GetIO(int &a, int &b, int &c, int &d)
	{
		pthread_mutex_lock(&mtxBascula);
		a = this->isCarro;
		b = this->isPalpa;
		c = this->isTwisl;
		d = this->isSubir;
		pthread_mutex_unlock(&mtxBascula);
	}
	/**
	 *
	 */
	void Bascula::SetGruaIO(int a, int b, int c, int d,int e, int f, int g, int h)
	{
	  pthread_mutex_lock(&mtxBascula);
	  this->isIOg0 = a;
	  this->isIOg1 = b;
	  this->isIOg2 = c;
	  this->isIOg3 = d;
	  this->isIOg4 = e;
	  this->isIOg5 = f;
	  this->isIOg6 = g;
	  this->isIOg7 = h;
	  pthread_mutex_unlock(&mtxBascula);
	}
	/**
	 *
	 */
	void Bascula::GetGruaIO(int &a, int &b, int &c, int &d,int &e, int &f, int &g, int &h)
	{
	  pthread_mutex_lock(&mtxBascula);
	  a = this->isIOg0;
	  b = this->isIOg1;
	  c = this->isIOg2;
	  d = this->isIOg3;
	  e = this->isIOg4;
	  f = this->isIOg5;
	  g = this->isIOg6;
	  h = this->isIOg7;
	  pthread_mutex_unlock(&mtxBascula);
	}
	/***
	 *
	 */
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
