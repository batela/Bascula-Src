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


#include <string>
#include "../include/service.h"


bool verbose=true;
using namespace std;
using namespace std;
extern BSCLEnlace *bscl;
extern DX80Enlace *dx80;
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
	/*
	DBPesaje db("/home/batela/bascula/db/kemen.db");
	db.Open();
	db.ReadLastTenData(data);
	db.Close();
	*/
}

void service_resource::reiniciar()
{
	system("sudo shutdown -r 0");
}

void service_resource::getLastData(string &data)
{

	std::cout << "getLastData.... "<< std::endl;


	char raw[256];
	char now [20];
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (now,20,"%F %T",timeinfo);
	//char isValido = (bscl->getBSCL()->GetEstable()==true)?'V':'N';
	//sprintf(raw,"%d;%c;%c;%d;%s\n",1,isValido,bscl->getBSCL()->GetSigno(),bscl->getBSCL()->GetPeso(),now);
	int isCarro, isPalpa, isTwisl, isSubir;
	bscl->getBSCL()->GetIO(isCarro,isPalpa,isTwisl,isSubir);

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

  /*
  if (cmX > 100) cmX = 100;
  if (cmY > 100) cmY = 100;
  if (cmX < -100) cmX = -100;
  if (cmY < -100) cmY = -100;
  printf (">>>>>>>>>>>>>>>>>>>Centros de masa %d %d\n",dx80->getDX()->getCMX(),dx80->getDX()->getCMY());
	*/

	sprintf(raw,"%d;%c;%c;%d;%s;%d;%d;%d;%d;%.0f;%.0f;%.0f;%.0f;%d;%d;%c;%c;%c;%c;%c;%c;%c;%c;%c;%d\n",1,fijo,'+',dx80->getDX()->getPeso(),now,isCarro,isPalpa,isTwisl,isSubir,dx80->getDX()->getPeso1(),dx80->getDX()->getPeso2(),dx80->getDX()->getPeso3(),dx80->getDX()->getPeso4(),cmX,cmY,okTW1,okTW2,okTW3,okTW4,okR,'F','G','H',okM,pesoValido);
	data =  raw;
	std::cout << "Enviado: " << data << std::endl;
}

/*
void usage()
{
    std::cout << "Usage:" << std::end,l
              << "service [-p <port>][-s [-k <keyFileName>][-c <dx80certFileName>]][-v]" << std::endl;
}
*/
