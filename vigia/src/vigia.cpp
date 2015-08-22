//============================================================================
// Name        : vigia.cpp
// Author      : Batela Software
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <iostream>
#include <httpserver.hpp>
#include "restclient.h"
#include <curl/curl.h>
using namespace std;
using namespace httpserver;



int main() {
	int contador = 0 ;
	CURL *curl;
	CURLcode res;
	printf ("Iniciando Vigia esperamos 20 segundos\n");
	sleep (20);
	curl = curl_easy_init();
	if(!curl){
	  printf ("Error iniciando CURL\n");
	  exit(0);
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.24.121:9898/service?op=ultimo");
	while (true){
	  res = curl_easy_perform(curl);
	  if (res != CURLE_OK){
	    if (contador++ >=5){
	      printf ("Reiniciando....\n");
	      system("sudo shutdown -r 0");
	    }
	    printf ("Reintento...%d\n", contador);
	  }
	  else {
	    printf ("Conexion KEMEN correcta\n");
	    contador = 0 ;
	  }
	  if (contador == 0)
	    sleep (20);
	  else
	    sleep (2);

	}
	curl_easy_cleanup(curl);
}

