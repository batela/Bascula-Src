#include "../include/kemen.h"
extern bool verbose;

BSCLEnlace *bscl = new BSCLEnlace (0,0);
DX80Enlace *dx80 = new DX80Enlace ();
log4cpp::Category &log  = log4cpp::Category::getRoot();
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

    //
    // Use builder to define webserver configuration options
    //
    create_webserver cw = create_webserver(port).max_threads(5);

    if (secure) {
        cw.use_ssl().https_mem_key(key).https_mem_cert(cert);
    }

    //

    // Create webserver using the configured options
    //
	webserver ws = cw;

    //
    // Create and register service resource available at /service
    //
	service_resource res;
	ws.register_resource("/service",&res,true);

    //
    // Start and block the webserver
    //
	ws.start(true);

	return 0;
}
/*
int PesaContainer (Explorador * exBascula){
	int res = 0;
	int contador = 0;
	log.info("%s: %s",__FILE__, "Comienza pesaje");
	((BSCLEnlace*)exBascula->getEnlace())->getBSCL()->SetEstable(false);
	sleep (atoi(Env::getInstance()->GetValue("esperapeso").data()));
	while (((BSCLEnlace*)exBascula->getEnlace())->getBSCL()->GetEstable()==false){
		sleep (1);
		if (contador++ >= 10) break;
	}
	if (((BSCLEnlace*)exBascula->getEnlace())->getBSCL()->GetEstable()==false) res = 1;
	return res;
}
*/
bool pesando 			= false ;
bool pesajeHecho 	= false ;


int CalcularPeso (vector <int> pesos, float pesoMedio){
	int res = -1 ;

	return res;
}

void * PesaContainer (void * exBascula){

	DBPesaje db("/home/batela/bascula/db/kemen.db");
	int pesajesCorrectos = atoi(Env::getInstance()->GetValue("pesajescorrectos").data());
	int pesajes 		= 0 ;
	float pesoMedio	=0;
	int pesoMaximo	=0;
	int esperaPesada = atoi(Env::getInstance()->GetValue("esperapeso").data());
	Explorador* ex = (Explorador *) exBascula;
	vector<int> pesos;
	struct timespec tim, tim2;
	tim.tv_sec 	= 0;
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
			int peso = ((BSCLEnlace*)ex->getEnlace())->getBSCL()->GetPeso();
			if (((BSCLEnlace*)ex->getEnlace())->getBSCL()->GetSigno() !='-'){
				pesos.push_back(peso);
				if (pesajes++<= pesajesCorrectos) nanosleep(&tim , &tim2);
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
			log.debug("%s: %s %d",__FILE__, "Peso Maximo container calculado...",pesoMaximo);
			log.info("%s: %s %f",__FILE__, "Peso-Medio container calculado...",pesoMedio);
			pesajeHecho = true;
			db.Open();
			db.InsertData(1,pesoMedio);
			db.Close();
		}
		nanosleep(&tim , &tim2);
	}
	return 0;
}

void * PesaContainerRadio (void * exBascula){

	DBPesaje db("/home/batela/bascula/db/kemen.db");
	int pesajesCorrectos = atoi(Env::getInstance()->GetValue("pesajescorrectos").data());
	int pesajes 		= 0 ;
	float pesoMedio	=0;
	int pesoMaximo	=0;
	int esperaPesada = atoi(Env::getInstance()->GetValue("esperapeso").data());
	MODBUSExplorador* ex = (MODBUSExplorador *) exBascula;
	vector<int> pesos;
	struct timespec tim, tim2;
	tim.tv_sec 	= 0;
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
			int peso = ((DX80Enlace*)ex->getEnlace())->getDX()->getPeso();
			if (((DX80Enlace*)ex->getEnlace())->getDX()->getSigno() !='-'){
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
			((DX80Enlace*)ex->getEnlace())->getDX()->setPesoValido(pesoMedio);

			log.debug("%s: %s %d",__FILE__, "Peso Maximo container calculado...",pesoMaximo);
			log.info("%s: %s %f",__FILE__, "Peso-Medio container calculado...",pesoMedio);

			pesajeHecho = true;
			((DX80Enlace*)ex->getEnlace())->getDX()->setIsFijo(true);
			((DX80Enlace*)ex->getEnlace())->getDX()->CalculaAlarmas();
			db.Open();
			db.InsertData(1,pesoMedio);
			db.Close();
		}
		//Esperamos por defecto 100ms
		nanosleep(&tim , &tim2);
	}
	return 0;
}


void * AlmacenaPesada (void * exBascula){
  //Desactivado de momento
  int pesajesCorrectos = 100;
  int pesajes     = 0 ;
  float pesoMedio = 0;
  int pesoMaximo  = 0;
  MODBUSExplorador* ex = (MODBUSExplorador *) exBascula;
  vector<int> pesos;
  vector<int> pesosC1;
  vector<int> pesosC2;
  vector<int> pesosC3;
  vector<int> pesosC4;
  struct timespec tim, tim2;
  tim.tv_sec  = 0;
  tim.tv_nsec = 100 * 1000000L; //en milisegundos
  while (true){
    tim.tv_nsec = 100 * 1000000L;
    pesos.clear();
    pesosC1.clear();
    pesajes = 0;

    log.debug("%s: %s",__FILE__, "Esperando señal de pesado");
    while (pesando==true && pesajes< pesajesCorrectos && pesajeHecho == false){
      if (pesajes++ == 0){
        tim.tv_nsec = 200 * 1000000L;
        pesos.clear();
        pesosC1.clear();
        pesosC2.clear();
        pesosC3.clear();
        pesosC4.clear();
      }
      pesos.push_back(((DX80Enlace*)ex->getEnlace())->getDX()->getPeso());
      pesosC1.push_back(((DX80Enlace*)ex->getEnlace())->getDX()->getPeso1());
      pesosC2.push_back(((DX80Enlace*)ex->getEnlace())->getDX()->getPeso2());
      pesosC3.push_back(((DX80Enlace*)ex->getEnlace())->getDX()->getPeso3());
      pesosC4.push_back(((DX80Enlace*)ex->getEnlace())->getDX()->getPeso4());
      nanosleep(&tim , &tim2);
    }
    //Esperamos por defecto 100ms
    nanosleep(&tim , &tim2);
  }
  return 0;
}

int main(int argc, char **argv) {

	pthread_t idThLector;
	pthread_t idThPesaje;

	//log4cpp::PropertyConfigurator::configure( Env::getInstance("/home/batela/bascula/cnf/bascula.cnf")->GetValue("logproperties") );

	log4cpp::PropertyConfigurator::configure( Env::getInstance()->GetValue("logproperties") );
	ESTADO estado;
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
	int ioCarro  = atoi(Env::getInstance()->GetValue("iocarroenvia").data()); //restamos 1 para referencia en 0
	int ioPalpa  = atoi(Env::getInstance()->GetValue("iopalpadores").data());
	int ioTwisl  = atoi(Env::getInstance()->GetValue("iotwislock").data());
	int ioSubir  = atoi(Env::getInstance()->GetValue("iomandosubir").data());

	IOEnlace *io = new IOEnlace();
	MODBUSPuerto *moxaPort = new MODBUSPuerto(Env::getInstance()->GetValue("puertomoxa"), 9600);
	MODBUSExplorador *exGarra = new MODBUSExplorador (io,moxaPort);

	if (atoi(Env::getInstance()->GetValue("usadisplay").data())>0){
		Env::getInstance()->GetValue("puertobascula");
		int baudios 		= atoi(Env::getInstance()->GetValue("baudiosbascula").data());
		int bitsdatos 	= atoi(Env::getInstance()->GetValue("bitsbascula").data());
		int bitsparada 	= atoi(Env::getInstance()->GetValue("bitsparadabascula").data());
		RS232Puerto *bsclPort = new RS232Puerto(Env::getInstance()->GetValue("puertobascula"), baudios,bitsdatos,0,bitsparada);
		Explorador 	*exBSCL		= new Explorador (bscl,bsclPort,true);
		pthread_create( &idThPesaje, NULL, PesaContainer,exBSCL);
	}
	else{
		dx80->Configure(""); //Necesario par cargar parametros
		MODBUSPuerto *dxPort = new MODBUSPuerto(Env::getInstance()->GetValue("puertodx80"), 19200);
		MODBUSExplorador 	*exBSCL		= new MODBUSExplorador (dx80,dxPort,"/home/batela/bascula/cnf/radiocom.cnf");
		pthread_create( &idThPesaje, NULL, PesaContainerRadio,exBSCL);
	}
	//Finalmente lanzamos el thread http
	pthread_create( &idThLector, NULL, httpservermanager,NULL);

	estado = ESPERA_CARRO_ENVIA;
	while (true){
		log.debug("%s: %s",__FILE__, "Lanzando lectura de módulo IO");
		if (exGarra->Explora() == 0){
			isCarro = io->GetLocks()->GetLock(ioCarro);
			isPalpa = io->GetLocks()->GetLock(ioPalpa);
			isTwisl = io->GetLocks()->GetLock(ioTwisl);
			isSubir = io->GetLocks()->GetLock(ioSubir);
			bscl->getBSCL()->SetIO(isCarro,isPalpa,isTwisl,isSubir);
			log.info("%s: %s: %d-%d-%d-%d",__FILE__, "Leido: ", isCarro, isPalpa,isTwisl,isSubir);

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

				break;

				case ESPERA_PALPADORES_NO_APOYO:
					log.info("%s: %s",__FILE__, "Proceso de pesaje en: ESPERA_PALPADORES_NO_APOYO");
					if (isCarro && !isPalpa && isTwisl && isSubir ){
						if ( pesando != true) pesando = true;
						estado = ESPERA_SOLTAR;
					}
					else if (!isTwisl) estado = ESPERA_CARRO_ENVIA;
					else estado = ESPERA_PALPADORES_NO_APOYO;
				break;
				case ESPERA_SOLTAR:
					log.info("%s: %s",__FILE__, "Proceso de pesaje en: ESPERA_SOLTAR");

					if (!isTwisl) estado = ESPERA_CARRO_ENVIA;
					else estado = ESPERA_SOLTAR;
				break;
				default:
					estado = ESPERA_CARRO_ENVIA;
				break;
			}
		}
		nanosleep(&tim , &tim2);
	}
	return 0;
}
