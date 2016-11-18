/*
     This file is part of libhttpserver
     Copyright (C) 2014 Sebastiano Merlino

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
     USA
*/


#include "../include/service.h"

#include <Bascula.h>
#include <DB.h>
#include <DX80.h>
#include <Env.h>
#include <httpserver/http_request.hpp>
#include <httpserver/http_response.hpp>
#include <httpserver/http_response_builder.hpp>
#include <httpserver/http_utils.hpp>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>


bool verbose=true;
using namespace std;
using namespace std;
extern BSCLEnlace *bscl;
extern DX80Enlace *dx80;
extern vector<int> gPesos;
extern vector<int> gPesosC1;
extern vector<int> gPesosC2;
extern vector<int> gPesosC3;
extern vector<int> gPesosC4;

service_resource::service_resource()
{
	pthread_mutex_init(&mtxService, NULL);
	db = new DBPesaje("/home/batela/bascula/db/kemen.db");
	db->Open();
}

service_resource::~service_resource()
{
	db->Close();
}

void service_resource::render_GET(const http_request &req, http_response** res)
{
	//std::cout << "service_resource::render_GET()" << std::endl;
	pthread_mutex_lock(&mtxService);
	map <string , string,arg_comparator> queryitems;

	if (db->isOpen()==false) db->Open();

  string response= "";
  string operation = req.get_arg("op");
  req.get_args(queryitems);

  if (operation.compare("historico")==0)
      this->getDBHistoricData(queryitems.find("startdate")->second,queryitems.find("enddate")->second,response);
  else if (operation.compare("max") == 0)
  	  this->getDBMaxDayData(queryitems.find("startdate")->second,queryitems.find("enddate")->second,queryitems.find("count")->second,response);
  else if(operation.compare("ultimo") == 0)
  	  this->getLastData(response);
  else if(operation.compare("ultimosdiez") == 0)
    	this->getLastTenData(response);
  else if(operation.compare("reiniciar") == 0)
      this->reiniciar();
  else if(operation.compare("grafica") == 0)
      this->getGraficaData(response);

  else std::cout << "Operacion: " << req.get_arg("op") << " no localizada."<<"\n";

  *res = new http_response(http_response_builder(response, 200).string_response());

  pthread_mutex_unlock(&mtxService);
  //if (verbose) std::cout << **res;
}



void service_resource::render_PUT(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_PUT()" << std::endl;	

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("PUT response", 200).string_response());

    if (verbose) std::cout << **res;
}


void service_resource::render_POST(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_POST()" << std::endl;	

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("POST response", 200).string_response());
    if (verbose) std::cout << **res;    
}
void service_resource::render(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render()" << std::endl;	

    if (verbose) std::cout << req;

	*res = new http_response(http_response_builder("generic response", 200).string_response());

    if (verbose) std::cout << **res;    
}


void service_resource::render_HEAD(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_HEAD()" << std::endl;

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("HEAD response", 200).string_response());

    if (verbose) std::cout << **res;    
}

void service_resource::render_OPTIONS(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_OPTIONS()" << std::endl;

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("OPTIONS response", 200).string_response());

    if (verbose) std::cout << **res;    
}

void service_resource::render_CONNECT(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_CONNECT()" << std::endl;	

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("CONNECT response", 200).string_response());

    if (verbose) std::cout << **res;    
}

void service_resource::render_DELETE(const http_request &req, http_response** res)
{
	std::cout << "service_resource::render_DELETE()" << std::endl;	

    if (verbose) std::cout << req;
    
	*res = new http_response(http_response_builder("DELETE response", 200).string_response());

    if (verbose) std::cout << **res;    
}
/*
 * http://192.168.24.109:9898/service?op=max&startdate=2014-12-26&enddate=2014-12-27&count=limit
 */
void service_resource::getDBMaxDayData(string startdate, string enddate,string count,string &data)
{
	std::cout << "getDBMaxDayData today:" << startdate << " end " << enddate << " count " << count << std::endl;

	DBPesaje db("/home/batela/bascula/db/kemen.db");
	db.Open();
	db.ReadMaxDayData(startdate,enddate,count,data);
	//std::cout << data;
	db.Close();
}
/**
 * http://192.168.24.109:9898/service?op=historico&startdate=2014-12-26&enddate=2014-12-27
 */
void service_resource::getDBHistoricData(string startdate, string enddate,string &data)
{
	std::cout << "getDBHistoricData start:" << startdate << " end " << enddate << std::endl;

	DBPesaje db("/home/batela/bascula/db/kemen.db");
	db.Open();
	db.ReadHistoricData(startdate,enddate,data);
	//std::cout << data;
	db.Close();
}

/**
 * http://192.168.24.109:9898/service?op=ultimosdiez
 */
void service_resource::getLastTenData(string &data)
{
	std::cout << "getLastTenData start:" << std::endl;
	if (db->ReadLastTenData(data) != 0) db->Close();
}

/**
 * http://192.168.24.109:9898/service?op=grafica
 */
void service_resource::getGraficaData(string &data)
{
  std::cout << "getGraficaData start:" << std::endl;
  std::stringstream ss;

  int esperaPeso = atoi(Env::getInstance()->GetValue("esperapeso").data());
  int pesajesCorrectos = atoi(Env::getInstance()->GetValue("pesajescorrectos").data());
  int numeroDatos = gPesos.size();
  ss << "A;"<< esperaPeso << ";" << pesajesCorrectos << ";" << numeroDatos << "\n";
  for(std::vector<int>::iterator it = gPesos.begin(); it != gPesos.end(); ++it) {
    ss << *it << ";";
  }
  ss <<"\n";

  for(std::vector<int>::iterator it = gPesosC1.begin(); it != gPesosC1.end(); ++it) {
    ss << *it << ";";
  }
  ss <<"\n";

  for(std::vector<int>::iterator it = gPesosC2.begin(); it != gPesosC2.end(); ++it) {
    ss << *it << ";";
  }
  ss <<"\n";

  for(std::vector<int>::iterator it = gPesosC3.begin(); it != gPesosC3.end(); ++it) {
    ss << *it << ";";
  }
  ss <<"\n";

  for(std::vector<int>::iterator it = gPesosC4.begin(); it != gPesosC4.end(); ++it) {
    ss << *it << ";";
  }
  ss <<"\n";
  data = ss.str();

  std::cout << "getGraficaData end:" << std::endl;
}
void service_resource::reiniciar()
{
	system("sudo shutdown -r 0");
}
/**
 * http://192.168.24.109:9898/service?op=ultimo
 */
void service_resource::getLastData(string &data)
{
	char raw[256];
	char now [20];
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (now,20,"%F %T",timeinfo);

	int isCarro, isPalpa, isTwisl, isSubir;
	bscl->getBSCL()->GetIO(isCarro,isPalpa,isTwisl,isSubir);

	int isIOg0,isIOg1,isIOg2,isIOg3,isIOg4,isIOg5,isIOg6,isIOg7;
	bscl->getBSCL()->GetGruaIO(isIOg0,isIOg1,isIOg2,isIOg3,isIOg4,isIOg5,isIOg6,isIOg7);

	char okM = ((dx80->getDX()->getIsOKMaster()==true)?'F':'V');
	char okR = ((dx80->getDX()->getIsOKRadio()==true)?'F':'V');
	char okTW1 = ((dx80->getDX()->getIsOKInput1())==true)?'F':'V';
	char okTW2 = ((dx80->getDX()->getIsOKInput2())==true)?'F':'V';
	char okTW3 = ((dx80->getDX()->getIsOKInput3())==true)?'F':'V';
	char okTW4 = ((dx80->getDX()->getIsOKInput4())==true)?'F':'V';
  char fijo =((dx80->getDX()->getIsFijo())==true)?'F':'V';
  int pesoValido = dx80->getDX()->getPesoValido();
  int cmX = dx80->getDX()->getCMX() ;
  int cmY = dx80->getDX()->getCMY();
  char okEdad=((dx80->getDX()->getEscentricidad()==false)?'F':'V');

  if (isTwisl == false)  cmX=cmY=0;

  char iog0 = ((isIOg0)==0)?'F':'V';
  char iog1 = ((isIOg1)==0)?'F':'V';
  char iog2 = ((isIOg2)==0)?'F':'V';
  char iog3 = ((isIOg3)==0)?'F':'V';
  char iog4 = ((isIOg4)==0)?'F':'V';
  char iog5 = ((isIOg5)==0)?'F':'V';
  char iog6 = ((isIOg6)==0)?'F':'V';
  char iog7 = ((isIOg7)==0)?'F':'V';
  char cal = 'R';
	sprintf(raw,"%d;%c;%c;%d;%s;%d;%d;%d;%d;%.0f;%.0f;%.0f;%.0f;%d;%d;%c;%c;%c;%c;%c;%c;%c;%c;%c;%d;%c;%c;%c;%c;%c;%c;%c;%c;%c\n",1,fijo,'+',dx80->getDX()->getPeso(),now,isCarro,isPalpa,isTwisl,isSubir,dx80->getDX()->getPeso1(),dx80->getDX()->getPeso2(),dx80->getDX()->getPeso3(),dx80->getDX()->getPeso4(),cmX,cmY,okTW1,okTW2,okTW3,okTW4,okR,okEdad,'G','H',okM,pesoValido,iog0,iog1,iog2,iog3,iog4,iog5,iog6,iog7,cal);
	data =  raw;
	//std::cout << "Enviado: " << data << std::endl;
}

/*
void usage()
{
    std::cout << "Usage:" << std::end,l
              << "service [-p <port>][-s [-k <keyFileName>][-c <dx80certFileName>]][-v]" << std::endl;
}
*/
