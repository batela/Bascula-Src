#ifndef HTTPSERVICE_H_
#define HTTPSERVICE_H_

#include <httpserver.hpp>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "DBPesaje.h"
#include "BSCLEnlace.h"
#include "DX80Enlace.h"
using namespace std;
using namespace httpserver;
using namespace container;
class service_resource: public http_resource<service_resource> {
public:
	service_resource();

	~service_resource();

	void render_GET(const http_request &req, http_response** res);

	void render_PUT(const http_request &req, http_response** res);

	void render_POST(const http_request &req, http_response** res);

	void render(const http_request &req, http_response** res);

	void render_HEAD(const http_request &req, http_response** res);

	void render_OPTIONS(const http_request &req, http_response** res);

	void render_CONNECT(const http_request &req, http_response** res);

	void render_DELETE(const http_request &req, http_response** res);
	//User defined
	void getDBMaxDayData(string startdate,string enddate,string limit,string &data);
	void getDBHistoricData(string startdate, string enddate,string &data);
	void getLastData(string &data);
	void getLastTenData(string &data);
	void getGraficaData(string &data);
	void reiniciar();

private:

	pthread_mutex_t mtxService;
	DBPesaje *db;

};

#endif /* X86MONITOR_H_ */
