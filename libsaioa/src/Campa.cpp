/*
 * Campa.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: borja
 */


#include "../include/Campa.h"
#include <cmath>

namespace container {
extern log4cpp::Category &log;

Campa::Campa() {
	Inicializa();
}
Campa::~Campa() {
	// TODO Auto-generated destructor stub
}

void Campa::Inicializa (){
	elipsoide= atoi (Env::getInstance()->GetValue("elipsoide").data());
	float latbase = atof (Env::getInstance()->GetValue("latbase").data());
	float lonbase = atof (Env::getInstance()->GetValue("lonbase").data());
	umbralCalle = atof (Env::getInstance()->GetValue("umbralcalle").data());
	LLtoUTM(elipsoide, latbase,lonbase, UTMNorthingBase, UTMEastingBase, UTMZoneBase);
}
/**
 * @return: Devuelve el numero de calles cargadas con exito. En caso de error devuelve -1;
 *
 */
int Campa::CargarCalles(string a)
{
	log.info("%s: %s %s",__FILE__, "Comienza carga de calles..",a.data());

	int callesReales = CargarCallesReales(a);
	int callesVirtuales = CargarCallesVirtuales();
	log.debug("%s: %s %d %s %d",__FILE__, "Numero de calles Reales: ",callesReales,"Numero de calles Virtuales: ", callesVirtuales);

	if (callesReales != callesVirtuales){
		log.error("%s: %s",__FILE__, "Error en carga de calles");
		callesReales = -1;
	}

	log.info("%s: %s %s",__FILE__, "Finaliza cargad de calles.",a.data());
	return callesReales;
}
/**
 *
 */
int Campa::CargarCallesReales(string a){
	log.info("%s: %s %s",__FILE__, "Comienza carga de calles..",a.data());
	string id, eq, val,line;
	ifstream cfgfile (a.data());
	int count = 0 ;
	while(getline((std::istream&)cfgfile, line)){
		log.info("%s: %s %s",__FILE__, "Leida linea: ",line.data());
	  if ((line[0] == '#') || (line[0] == '0')) continue;  // skip comments and lline with 0
	  	vector<string> items = this->split (line,' ');
	    if (items.size() == 11 )
	    	if (CrearCalle (items) == 0 ) count++;
	}
	log.info("%s: %s %d",__FILE__, "Finalizada carga de calles con: ",count);
	return count ;
}
/**
 *
 */
int Campa::CargarCallesVirtuales(){
	log.info("%s: %s %s",__FILE__, "Comienza carga de calles virtuales..");
	int count = 0 ;
	for(std::vector<Calle*>::iterator it = callesReales.begin(); it != callesReales.end(); ++it) {
		Calle *cc = *it;
		Calle *cv= new Calle();
		CalculaCoordenadasVirtuales (cc,cv);
		cv->setId(cc->getId());
		cv->setZona	(cc->getZona());
		cc->setCalle(cc->getCalle());
		cv->CrearRecta();
		callesVirtuales.push_back(cv);
	}
	log.info("%s: %s %d",__FILE__, "Finalizada carga de calles virtuales con : ",count);
	return count ;
}
/**
 * @return: res = 0 si va bien
 */
int Campa::CalculaCoordenadasVirtuales (Calle * cc,Calle *cv){
	log.info("%s: %s",__FILE__, "Comienza calculo de coordenadas de calles virtuales..");
	float PI=3.14159265;
	int res = 0 ;

	float cosval = 1 / (sqrt ( (cc->getPendiente()*cc->getPendiente() + 1)/(cc->getPendiente()*cc->getPendiente()) ));
	float alpha = acos (cosval);
	//float angulo = alpha * 180 / PI;

  int factor = 0;
  float segmento = 16;
  (cc->getPendiente()>0)?factor = -1 : factor = 1;
	cv->setYi(cc->getYi() - factor * segmento * sin (alpha));
	cv->setXi(cc->getXi() -  segmento * cos (alpha));
	cv->setYf(cc->getYf() - factor * segmento * sin (alpha));
	cv->setXf(cc->getXf() -  segmento * cos (alpha));
	log.debug("%s: %s %f : %f : %f : %f",__FILE__,"Coordenada calculadas: Yi Xi Yf Xf",cv->getYi(),cv->getXi(),cv->getYf(),cv->getXf() );

	/*
	float mvirtual = (-1 /cc->getPendiente());
	float cosval2 = 1 / (sqrt ( (mvirtual*mvirtual + 1)/(mvirtual*mvirtual) ));
	float alpha2 = acos (cosval2);
	float angulo2 = alpha2 * 180 / PI;
	float jjj = std::abs (sin (alpha));
	cv->setYi(cc->getYi() - factor * segmento * sin (alpha2));
	cv->setXi(cc->getXi() - segmento * cos (alpha2));
	cv->setYf(cc->getYf() - factor * segmento * sin (alpha2));
	cv->setXf(cc->getXf() - segmento * cos (alpha2));
	log.debug("%s: %s %f : %f : %f : %f",__FILE__,"Coordenada calculadas: Yi Xi Yf Xf",cv->getYi(),cv->getXi(),cv->getYf(),cv->getXf() );
 */
	log.info("%s: %s",__FILE__, "Finaliza calculo de coordenas virtuales");
	return res ;
}

/**
 *
 */
int Campa::CrearCalle (vector<string> items){
	int res = 0 ;
	Calle *cc = new Calle();
	cc->setId(atoi(items[0].data()));
	cc->setYi((-1)*atof(items[1].data()));
	cc->setXi(atof(items[2].data()));
	cc->setYf((-1)*atof(items[3].data()));
	cc->setXf(atof(items[4].data()));
	cc->setZona(atoi(items[6].data()));
	cc->setCalle(atoi(items[7].data()));
	/// TODO flata el resto
	cc->CrearRecta();
	callesReales.push_back(cc);
	return res;
}

vector<std::string> &Campa::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::istringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
  	//cout << "Config Item .." << item << "\n";
  	if (!item.empty() )elems.push_back(item);
  }
    return elems;
}


vector<std::string> Campa::split(const std::string &s, char delim) {
	vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

void Campa::CalcularPosicion(float lat, float lon, PosicionGrua &pg) {

	log.info("%s: %s %f %f",__FILE__, "Inicia CalcularPosicion con latitud y longitud: ", lat,lon);

	double UTMNorthing;
	double UTMEasting;
	char UTMZone[4];
	double Y; 	//posiciones relativas a la referencia
	double X; 	//posiciones relativa a la referenciaLL2XY::
	LL2XY ll2xy;

	// 1.-Pasar lat y lon a UTM.
	log.info("%s: %s %f %f",__FILE__, "Calculando Posicion para: ",lat ,lon);
	LLtoUTM(elipsoide , lat, lon, UTMNorthing, UTMEasting, UTMZone);

	// 2.-Calcular posicion relativa a 0-0 en UTM.
  Y = UTMNorthingBase - UTMNorthing ;
  X = UTMEastingBase - UTMEasting;

  //Inicializamos los parametros de PosicionGrua
  pg.setGlat(lat);
  pg.setGlon(lon);
  pg.setEasting(UTMEasting);
  pg.setNorting(UTMNorthing);
  pg.setYrelativo(Y);
  pg.setXrelativo(X);
  pg.setCalle	('-');
  pg.setBloque('-');

  /* SOLO PARA PRUEBAS
  Y=lat;
  X=lon;
  */
  // 3.-Con el norting/easting relativos calcular la distanci a las rectas
  double linePY= 0; 						// Proyeccion de la coordenada Y sobre la calle
  double linePX= 0; 						// Proyeccion de la coordenada X sobre la calle
  double linePYVirtual= 0; 			// Proyección de la coodernada Y sobre la calle virtual
  double linePXVirtual= 0; 			// Proyección de la coordenada X sobre la calle virtual
  double distanciaReal = -1;		// Distancia a la calle real
  double distanciaVirtual = -1; // Distancia a la calle virtual
  int indice= -1 ;

  int indiceReales		= BuscaCalleReferencia (callesReales, X,Y,linePX,linePY,distanciaReal);
  int indiceVirtuales	= BuscaCalleReferencia (callesVirtuales, X,Y,linePXVirtual,linePYVirtual,distanciaVirtual);
  log.debug("%s: %s: %d : %f",__FILE__, "Indices en calles Reales : Distancia:" , indiceReales, distanciaReal);
  log.debug("%s: %s: %d : %f",__FILE__, "Indices en calles Virtuales :Distancia:" , indiceVirtuales, distanciaVirtual);
  if ((distanciaReal < distanciaVirtual) && (indiceReales != -1)){
  	indice = indiceReales;
  	pg.setYsobreCalle(linePY);
  	pg.setXsobreCalle(linePX);
  	pg.setDistanciaCalle(distanciaReal);
  	pg.setFila((int)linePY/(3.045+0.09));
  }
  else if ((distanciaReal > distanciaVirtual) && (indiceVirtuales != -1)){
  	indice = indiceVirtuales;
  	pg.setYsobreCalle(linePYVirtual);
  	pg.setXsobreCalle(linePXVirtual);
  	pg.setDistanciaCalle(distanciaVirtual);
  	pg.setFila((int)linePYVirtual/(3.045+0.09));
  }
  log.debug("%s: %s: %d",__FILE__, "Indice asignado: ", indice);
  if (indice >= 0){
  	pg.setCalle	(callesReales[indice-1]->getCalle());
  	pg.setBloque(callesReales[indice-1]->getZona());
  }

  // 4.-Una vez que tenemos la recta situarnos en el origen y ver cuantos contendore hay

  log.debug("%s: %s: %f : %f %c %c",__FILE__, "Posicionado de grua:" , pg.getXNOATUM(),pg.getYNOATUM(),pg.getCalle(),pg.getBloque());
  log.info("%s: %s",__FILE__, "Termina CalcularPosicion");
}
/**
 *
 */
int Campa::BuscaCalleReferencia (vector <Calle *> calles, double X,double Y,double &linePX,double &linePY,double &distancia)
{
	LL2XY ll2xy;
	int indice = 0 ;
	int res = -1 ;
  distancia = 99999999;
	for(std::vector<Calle*>::iterator it = calles.begin(); it != calles.end(); ++it) {
		Calle *cc = *it;
	  float aux = ll2xy.findDistanceToSegment(cc->getXi(),cc->getYi(),cc->getXf(),cc->getYf(),X,Y,linePX,linePY);
	  log.debug("%s: %s %d %s %f",__FILE__, "Analizada calle: ",++indice," distancia: ",aux);
	  if (aux < distancia){
	  	distancia = aux;
	  	if (aux <= umbralCalle) res = indice;
	  }
	}
	return res ;
}


}//namespace
