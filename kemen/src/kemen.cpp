#include "../include/kemen.h"
#include <Bascula.h>
#include <DX80.h>
#include <Env.h>
#include <GruaIO.h>
#include <httpserver/create_webserver.hpp>
#include <httpserver/webserver.hpp>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <Locks.h>
#include <unistd.h>
#include <iterator>
#include <string>
#include <vector>

//extern bool verbose;

BSCLEnlace *bscl = new BSCLEnlace (0,0);
DX80Enlace *dx80 = new DX80Enlace ();
vector <Enlace*> mbEnlacesP1 ;
vector <Enlace*> mbEnlacesP2 ;

log4cpp::Category &log  = log4cpp::Category::getRoot();

bool pesando      = false ;
bool pesajeHecho  = false ;
vector<int> gPesos(100,0);
vector<int> gPesosC1(100,0);
vector<int> gPesosC2(100,0);
vector<int> gPesosC3(100,0);
vector<int> gPesosC4(100,0);

void * httpservermanager(void * p)
{
	//uint16_t port=8080;
  uint16_t port=9898;
	//BSCLEnlace *bscl = (BSCLEnlace *)p;
	const char *key	="key.pem";
	const char *cert="cert.pem";
	bool secure=false;
	std::cout << "Using port " << port << std::endl;
  if (secure) {
    std::cout << "Key: " << key << " Certificate: " << cert << std::endl;
  }
  // Use builder to define webserver configuration options
  //
  create_webserver cw = create_webserver(port).max_threads(5);
  if (secure) {
    cw.use_ssl().https_mem_key(key).https_mem_cert(cert);
  }
  // Create webserver using the configured options
  webserver ws = cw;
  // Create and register service resource available at /service
  //
  service_resource res;
  ws.register_resource("/service",&res,true);
 // Start and block the webserver
 //
 ws.start(true);
 return 0;
}
/**
 *
 */
int CalcularPeso (vector <int> pesos, float pesoMedio){
	int res = -1 ;

	return res;
}
/**
 *
 */
void * PesaContainerRadio (void * enlace){

  DBPesaje db("/home/batela/bascula/db/kemen.db");
  int pesajesCorrectos = atoi(Env::getInstance()->GetValue("pesajescorrectos").data());
  int pesajes     = 0 ;
  float pesoMedio =0;
  int pesoMaximo  =0;
  int esperaPesada = atoi(Env::getInstance()->GetValue("esperapeso").data());
  int pesoMinimo = atoi(Env::getInstance()->GetValue("pesominimo").data());
  DX80Enlace* ex = (DX80Enlace *) enlace;
  vector<int> pesos;
  struct timespec tim, tim2;
  tim.tv_sec  = 0;
  tim.tv_nsec = 100 * 1000000L; //en milisegundos
  bool delayHecho = false;

  while (true){
    tim.tv_nsec = 100 * 1000000L;
    pesos.clear();
    pesajes = pesoMedio = 0;

    log.debug("%s: %s",__FILE__, "Esperando señal de pesado");
    delayHecho = false;
    while (pesando==true && pesajes< pesajesCorrectos && pesajeHecho == false){
      if (delayHecho == false) {
        sleep (esperaPesada);
        delayHecho = true;
      }
      tim.tv_nsec = 200 * 1000000L;
      int peso = ((DX80Enlace*)ex)->getDX()->getPeso();
      //((DX80Enlace*)ex->getEnlace())->getDX()->setPesoValido(pesoMedio); Cuando se pasaba el explorador
      if (((DX80Enlace*)ex)->getDX()->getSigno() !='-'){
        pesos.push_back(peso);
        if (pesajes++<= pesajesCorrectos)
          nanosleep(&tim , &tim2);
      }
    }
    //Actualizamos la base de datos
    if (pesajes>= pesajesCorrectos){
      for(std::vector<int>::iterator it = pesos.begin(); it != pesos.end(); ++it) {
        if (pesoMaximo < *it) pesoMaximo= *it;
        pesoMedio = pesoMedio + *it;
        log.info("%s: %s %d",__FILE__, " >>>Peso leido...",*it);
      }
      pesoMedio = pesoMedio / pesos.size();
      ((DX80Enlace*)ex)->getDX()->setPesoValido(pesoMedio);

      log.debug("%s: %s %d",__FILE__, "Peso Maximo container calculado...",pesoMaximo);
      log.info("%s: %s %f",__FILE__, "Peso-Medio container calculado...",pesoMedio);

      pesajeHecho = true;
      ((DX80Enlace*)ex)->getDX()->setIsFijo(true);
      ((DX80Enlace*)ex)->getDX()->CalculaAlarmas();
      //Si el peso medido es inferior a pesominimo se entiende que es una operación
      // en vacio. No se guarda
      if (pesoMedio > pesoMinimo){
        db.Open();
        db.InsertData(1,pesoMedio);
        db.Close();
      }
    }

    //Esperamos por defecto 100ms
    nanosleep(&tim , &tim2);
  }
  return 0;
}
/**
 *
 */
void * CalculaOffSetPesada (void * exBascula)
{
  //Desactivado de momento
   MODBUSExplorador* ex = (MODBUSExplorador *) exBascula;

   struct timespec tim, tim2;
   tim.tv_sec  = 0;
   tim.tv_nsec = 500 * 1000000L; //en milisegundos
   while (true){
     tim.tv_nsec = 100 * 1000000L;
     log.debug("%s: %s",__FILE__, "Esperando final de pesada");
     while (pesando!=true ){
       sleep(2); //Evaluamos el offset cada dos segundos
     }
     //Esperamos por defecto 100ms
     nanosleep(&tim , &tim2);
   }
   return 0;
}
/**
 *
 */
void * CalibradoCelulas (void * e)
{
    log.debug("%s: %s",__FILE__, "Comenzando calibrado de celulas");
    MODBUSExplorador* ex = (MODBUSExplorador *) e;

    int pesajesCalibrado  = atoi(Env::getInstance()->GetValue("pesajescalibrado").data());
    int margenCalibrado   = atoi(Env::getInstance()->GetValue("margencalibrado").data());
    int contaje = 0;

    struct timespec tim, tim2,tim3;

    tim.tv_sec  = atoi(Env::getInstance()->GetValue("tesperacalibrado").data());
    tim.tv_nsec = 0; //Damos una espera de dos segundo y luego mantenemos dos segundos la señal activa
    //Tiempo entre pesajes a contabilizar
    tim3.tv_sec  = 0;
    tim3.tv_nsec = 500 * 1000000L;
    vector<int> pesosC1;
    vector<int> pesosC2;
    vector<int> pesosC3;
    vector<int> pesosC4;

    //Esperamos un par de segundos y almacenamos los valores durante un tiempo
    nanosleep(&tim , &tim2);
    while (contaje++ < pesajesCalibrado){
      pesosC1.push_back(dx80->getDX()->getPeso1());
      pesosC2.push_back(dx80->getDX()->getPeso2());
      pesosC3.push_back(dx80->getDX()->getPeso3());
      pesosC4.push_back(dx80->getDX()->getPeso4());
      nanosleep(&tim3 , &tim2);
    }
    //Hacemos la media de todos los pesajes
    int pC1= 0, pC2=0, pC3=0,pC4=0;
    for(std::size_t i = 0; i < pesosC1.size(); ++i) {
      pC1=pC1 + ((pesosC1[i]<0)?0:pesosC1[i]);
      pC2=pC2 + ((pesosC2[i]<0)?0:pesosC2[i]);
      pC3=pC3 + ((pesosC3[i]<0)?0:pesosC3[i]);
      pC4=pC4 + ((pesosC4[i]<0)?0:pesosC4[i]);
    }
    pC1 = pC1 / pesosC1.size();
    pC2 = pC2 / pesosC2.size();
    pC3 = pC3 / pesosC3.size();
    pC4 = pC4 / pesosC4.size();

    //log.info(">>>>>>>>>>>>>>>>Calculo margen calibrado %d total %d",pC1 + pC2 + pC3 + pC4 , margenCalibrado);
    //Si la suma de los pesajes supera el margen de calibrado activamos la señal de calibrado
    if ((pC1 + pC2 + pC3 + pC4) >= margenCalibrado){
      int dirMB = atoi (mbEnlacesP1[0]->getItemCfg("equipo","dir").data());
      log.info("%s: %s",__FILE__, "Señal de calibrado ON");
      int ioCalib = atoi(Env::getInstance()->GetValue("iocalibrado").data());
      ex->EscribeCoil(dirMB,ioCalib,1);
      tim.tv_sec  = atoi(Env::getInstance()->GetValue("tcalibrado").data());
      nanosleep(&tim , &tim2);
      //Ponemos una seguridad para que en caso de error vuelva a escribir
      log.info("%s: %s",__FILE__, "Señal de calibrado OFF");
      if (ex->EscribeCoil(dirMB,ioCalib,0) != 0){
        ex->EscribeCoil(dirMB,ioCalib,0);
       }
    }

    log.debug("%s: %s",__FILE__, "Finalizado calibrado de celulas");

   return 0;
}
/**
 *
 */
void * AlmacenaPesada (void * enlace){
  //Desactivado de momento
  int pesajesCorrectos = 100;
  int pesajes     = 0 ;
  DX80Enlace* ex = (DX80Enlace *) enlace;
  struct timespec tim, tim2;
  tim.tv_sec  = 0;
  tim.tv_nsec = 100 * 1000000L; //en milisegundos
  while (true){
    tim.tv_nsec = 200 * 1000000L;
    if (pesando==false) pesajes = 0;
    log.debug("%s: %s",__FILE__, "Esperando señal de pesado");
    //while (pesando==true && pesajes< pesajesCorrectos && pesajeHecho == false){
    while (pesando==true && pesajes< pesajesCorrectos){
      if (pesajes++ == 0){
        tim.tv_nsec = 200 * 1000000L;
        gPesos.clear();
        gPesosC1.clear();
        gPesosC2.clear();
        gPesosC3.clear();
        gPesosC4.clear();
      }
      gPesos.push_back(((DX80Enlace*)ex)->getDX()->getPeso());
      gPesosC1.push_back(((DX80Enlace*)ex)->getDX()->getPeso1());
      gPesosC2.push_back(((DX80Enlace*)ex)->getDX()->getPeso2());
      gPesosC3.push_back(((DX80Enlace*)ex)->getDX()->getPeso3());
      gPesosC4.push_back(((DX80Enlace*)ex)->getDX()->getPeso4());
      nanosleep(&tim , &tim2);
    }
    //Esperamos por defecto 100ms
    log.debug("%s: %s %d",__FILE__, "Numero de pesajes guardados: " , pesajes);
    nanosleep(&tim , &tim2);
  }
  return 0;
}
/**
 *
 */
int main(int argc, char **argv) {

	pthread_t idThLector;
	pthread_t idThPesaje;
	pthread_t idThAlmacenaPesada;
  pthread_t idThCalibrado;

	//log4cpp::PropertyConfigurator::configure( Env::getInstance("/home/batela/bascula/cnf/bascula.cnf")->GetValue("logproperties") );
	log4cpp::PropertyConfigurator::configure( Env::getInstance()->GetValue("logproperties") );
	ESTADO estado, estadoAnterior;
	DBPesaje db("/home/batela/bascula/db/kemen.db");
	log.info("%s: %s",__FILE__, "Iniciando aplicacion de gruas...");

	bscl->Configure(Env::getInstance()->GetValue("pesajescorrectos"), Env::getInstance()->GetValue("margenpesajes"));
	//Configuramos el lecto IO
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = atoi(Env::getInstance()->GetValue("ioperiod").data()) * 1000000L;

  int isCarro = 0;
	int isPalpa = 0;
	int isTwisl = 0;
	int isSubir = 0;
	int isIOg0 = 0;
	int isIOg1 = 0;
	int isIOg2 = 0;
	int isIOg3 = 0;
	int isIOg4 = 0;
	int isIOg5 = 0;
	int isIOg6 = 0;
	int isIOg7 = 0;

	int ioCarro  = atoi(Env::getInstance()->GetValue("iocarroenvia").data()); //restamos 1 para referencia en 0
	int ioPalpa  = atoi(Env::getInstance()->GetValue("iopalpadores").data());
	int ioTwisl  = atoi(Env::getInstance()->GetValue("iotwislock").data());
	int ioSubir  = atoi(Env::getInstance()->GetValue("iomandosubir").data());

	int ioG0  = atoi(Env::getInstance()->GetValue("iogrua0").data());
	int ioG1  = atoi(Env::getInstance()->GetValue("iogrua1").data());
	int ioG2  = atoi(Env::getInstance()->GetValue("iogrua2").data());
	int ioG3  = atoi(Env::getInstance()->GetValue("iogrua3").data());
	int ioG4  = atoi(Env::getInstance()->GetValue("iogrua4").data());
	int ioG5  = atoi(Env::getInstance()->GetValue("iogrua5").data());
	int ioG6  = atoi(Env::getInstance()->GetValue("iogrua6").data());
	int ioG7  = atoi(Env::getInstance()->GetValue("iogrua7").data());

	IOEnlace *io = new IOEnlace(); //Posicion enlace 0
	io->Configure("/home/batela/bascula/cnf/ibd1.cnf");

	IOEnlaceGrua *ioGrua = new IOEnlaceGrua();
	ioGrua->Configure("/home/batela/bascula/cnf/ibd2.cnf");

	MODBUSPuerto *moxaPort = new MODBUSPuerto(Env::getInstance()->GetValue("puertomoxa"), 9600);
	//MODBUSExplorador *exGarra = new MODBUSExplorador (io,moxaPort);
	//Creamos la lista de enlace modbus del puerto #1
	mbEnlacesP1.push_back(io);
	mbEnlacesP1.push_back(ioGrua);
	MODBUSExplorador *exGarra = new MODBUSExplorador (mbEnlacesP1,moxaPort,"/home/batela/bascula/cnf/mbp1.cnf");

	if (atoi(Env::getInstance()->GetValue("usadisplay").data())>0){
		Env::getInstance()->GetValue("puertobascula");
		int baudios 		= atoi(Env::getInstance()->GetValue("baudiosbascula").data());
		int bitsdatos 	= atoi(Env::getInstance()->GetValue("bitsbascula").data());
		int bitsparada 	= atoi(Env::getInstance()->GetValue("bitsparadabascula").data());
		RS232Puerto *bsclPort = new RS232Puerto(Env::getInstance()->GetValue("puertobascula"), baudios,bitsdatos,0,bitsparada);
		Explorador 	*exBSCL		= new Explorador (bscl,bsclPort,true);
		//pthread_create( &idThPesaje, NULL, PesaContainer,exBSCL);
	}
	else{
		dx80->Configure("/home/batela/bascula/cnf/dx80modbus.cnf"); //Necesario par cargar parametros
		mbEnlacesP2.push_back(dx80);
		MODBUSPuerto *dxPort = new MODBUSPuerto(Env::getInstance()->GetValue("puertodx80"), 19200);

		//MODBUSExplorador 	*exBSCL		= new MODBUSExplorador (dx80,dxPort,"/home/batela/bascula/cnf/radiocom.cnf");
		MODBUSExplorador  *exBSCL   = new MODBUSExplorador (mbEnlacesP2,dxPort,"/home/batela/bascula/cnf/radiocom.cnf");
		pthread_create( &idThPesaje, NULL, PesaContainerRadio,dx80);
	}
	//Finalmente lanzamos el thread http
	pthread_create( &idThLector, NULL, httpservermanager,NULL);
  pthread_create( &idThAlmacenaPesada, NULL, AlmacenaPesada,dx80);
  //pthread_create( &idThCalibrado, NULL, CalibradoCelulas,exGarra);

	estado = estadoAnterior = ESPERA_CARRO_ENVIA;
	while (true){
		log.debug("%s: %s",__FILE__, "Lanzando lectura de módulo IO");
		if (exGarra->Explora() == 0){
			isCarro = io->GetLocks()->GetLock(ioCarro);
			isPalpa = io->GetLocks()->GetLock(ioPalpa);
			isTwisl = io->GetLocks()->GetLock(ioTwisl);
			isSubir = io->GetLocks()->GetLock(ioSubir);

			isIOg0 = ioGrua->GetIOGrua()->GetGruaIO(ioG0);
			isIOg1 = ioGrua->GetIOGrua()->GetGruaIO(ioG1);
			isIOg2 = ioGrua->GetIOGrua()->GetGruaIO(ioG2);
			isIOg3 = ioGrua->GetIOGrua()->GetGruaIO(ioG3);
			isIOg4 = ioGrua->GetIOGrua()->GetGruaIO(ioG4);
			isIOg5 = ioGrua->GetIOGrua()->GetGruaIO(ioG5);
			isIOg6 = ioGrua->GetIOGrua()->GetGruaIO(ioG6);
			isIOg7 = ioGrua->GetIOGrua()->GetGruaIO(ioG7);

			bscl->getBSCL()->SetIO(isCarro,isPalpa,isTwisl,isSubir);
			bscl->getBSCL()->SetGruaIO(isIOg0, isIOg1, isIOg2, isIOg3, isIOg4, isIOg5, isIOg6, isIOg7);
			log.info("%s: %s: %d-%d-%d-%d",__FILE__, "Leido: ", isCarro, isPalpa,isTwisl,isSubir);
			log.info("%s: %s: %d-%d-%d-%d-%d-%d-%d-%d",__FILE__, "IO en Grua: ", isIOg0, isIOg1, isIOg2, isIOg3, isIOg4, isIOg5, isIOg6, isIOg7);

			switch (estado){
				case ESPERA_CARRO_ENVIA:
					log.info("%s: %s",__FILE__, "Proceso de pesaje en: ESPERA_CARRO_VIA");
					pesando 		= false;
					pesajeHecho = false ;
					if (isCarro && isPalpa && isTwisl) estado = ESPERA_PALPADORES_NO_APOYO;
					else estado = ESPERA_CARRO_ENVIA;
					//liberamos la condicion de pesado
					dx80->getDX()->setIsFijo(false);
					dx80->getDX()->setPesoValido(0);
					if (estadoAnterior == ESPERA_SOLTAR) {
					  pthread_create( &idThCalibrado, NULL, CalibradoCelulas,exGarra);
					}
					estadoAnterior = estado;
				break;

				case ESPERA_PALPADORES_NO_APOYO:
					log.info("%s: %s",__FILE__, "Proceso de pesaje en: ESPERA_PALPADORES_NO_APOYO");
					estadoAnterior = estado;
					if (isCarro && !isPalpa && isTwisl && isSubir ){
						if ( pesando != true) pesando = true;
						estado = ESPERA_SOLTAR;
					}
					else if (!isTwisl) estado = ESPERA_CARRO_ENVIA;
					else estado = ESPERA_PALPADORES_NO_APOYO;

				break;
				case ESPERA_SOLTAR:
					log.info("%s: %s",__FILE__, "Proceso de pesaje en: ESPERA_SOLTAR");
					estadoAnterior = estado;
					if (!isTwisl) estado = ESPERA_CARRO_ENVIA;
					else estado = ESPERA_SOLTAR;
				break;
				default:
					estado = estadoAnterior= ESPERA_CARRO_ENVIA;
				break;
			}
		}
		nanosleep(&tim , &tim2);
	}
	return 0;
}
