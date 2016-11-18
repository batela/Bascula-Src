/*
 * DX80.cpp
 *
 *  Created on: May 1, 2015
 *      Author: root
 */

#include "DX80.h"

namespace container {

DX80::DX80() {

}

DX80::~DX80() {

}


void DX80::Configure ()
{
  offSetPeso  =13107;
  valores[0] = valores[1] = valores[2] = valores[3] = 0;
  isOKInput1=isOKInput2=isOKInput3=isOKInput4=true;
  tolCM   = atoi(Env::getInstance()->GetValue("toleranciacm").data());
  tolPeso = atoi(Env::getInstance()->GetValue("toleranciapesoerror").data());
  tolDiffCelulas  = atoi(Env::getInstance()->GetValue("toleranciadiffcelulas").data());
  maxPesoTolerado = atoi(Env::getInstance()->GetValue("pesomaximotolerado").data());
  origenEscentri  = atoi(Env::getInstance()->GetValue("origenescentricidad").data());
  isEscentricidad = false;
  printf ("**********************Valores config %d %d %d %d %d\n",tolCM,tolPeso,tolDiffCelulas,maxPesoTolerado,origenEscentri);
}

void DX80::CalculaAlarmas()
{
  int negTol = (-1) * tolCM ;
  int differencias[6];
  int idx = 0;
  for (int i = 0 ; i < 4 ; i++){
    // 1.- Si uno de los valores es negativo o cero entonces alarma
    if (valores[i] <= offSetPeso)
      alarmas[i] = true;
    else
      alarmas[i] = false;

    for (int k = i+1 ; k < 4 ; k++){
      differencias[idx++]= abs(valoresPeso[i]- valoresPeso [k]);
    }
  }
  printf ("1.- Alarmas***************: %d %d %d %d: peso %d\n",alarmas[0],alarmas[1],alarmas[2],alarmas[3],tolPeso );
  // TW cerrados y una de las celulas marca menos de un umbral
  if ((int)this->getPeso1() <= tolPeso ) alarmas[0]=true;
  if ((int)this->getPeso2() <= tolPeso ) alarmas[1]=true;
  if ((int)this->getPeso3() <= tolPeso ) alarmas[2]=true;
  if ((int)this->getPeso4() <= tolPeso ) alarmas[3]=true;

  printf ("2.- Alarmas***************: %d %d %d %d\n",alarmas[0],alarmas[1],alarmas[2],alarmas[3] );
  if (!alarmas[0] && !alarmas[1] && !alarmas[2] && !alarmas[3]){

//    float razon = float (this->getPeso2() / this->getPeso3());
//    if ((razon > 1) && ((100*razon - 100) > this->tolDiffCelulas )) alarmas[1] = true;
//    else if ((razon < 1) && ((100*razon) < this->tolDiffCelulas )) alarmas[2] = true;
//
//    razon = float (this->getPeso1() / this->getPeso4());
//    if ((razon > 1) && ((100*razon - 100) > this->tolDiffCelulas )) alarmas[0] = true;
//    else if ((razon < 1) && ((100*razon) < this->tolDiffCelulas )) alarmas[3] = true;

    //printf ("Alarmas***************: %d %d %d %d",alarmas[0],alarmas[1],alarmas[2],alarmas[3] );
    //printf ("Los centros de masas: %d %d", cmX, cmY);
    if (cmX > tolCM && cmY > tolCM)  alarmas[1]=alarmas[2]=alarmas[3]= true;
    if (cmX < negTol && cmY > tolCM) alarmas[0]=alarmas[2]=alarmas[3]= true;
    if (cmX < negTol && cmY < negTol) alarmas[0]=alarmas[1]=alarmas[3]= true;
    if (cmX > tolCM  && cmY < negTol) alarmas[0]=alarmas[1]=alarmas[2]= true;

    if (cmX < tolCM && cmX > negTol && cmY > tolCM)   alarmas[2]=alarmas[3]= true;
    if (cmX < tolCM && cmX > negTol && cmY < negTol)  alarmas[0]=alarmas[1]= true;
    if (cmY < tolCM && cmY > negTol && cmX > tolCM)   alarmas[1]=alarmas[2]= true;
    if (cmY < tolCM && cmY > negTol && cmX < negTol)  alarmas[0]=alarmas[3]= true;

    if (this->getPeso() >= maxPesoTolerado) alarmas[0]=alarmas[1]=alarmas[2]=alarmas[3]= true;

  }
  printf ("3.- Alarmas***************: %d %d %d %d\n",alarmas[0],alarmas[1],alarmas[2],alarmas[3] );

  if (!alarmas[0] && !alarmas[1] && !alarmas[2] && !alarmas[3]){
    if ((-1)* origenEscentri >= cmX || origenEscentri <= cmX || (-1)* origenEscentri >= cmY || origenEscentri <= cmY )
      setEscentricidad(true);
    else
      setEscentricidad(false);
  }

  (alarmas[0]==false)?isOKInput1=true:isOKInput1=false;
  (alarmas[1]==false)?isOKInput2=true:isOKInput2=false;
  (alarmas[2]==false)?isOKInput3=true:isOKInput3=false;
  (alarmas[3]==false)?isOKInput4=true:isOKInput4=false;

  printf ("4.- Alarmas***************: %d %d %d %d\n",alarmas[0],alarmas[1],alarmas[2],alarmas[3] );
}
} /* namespace container */
