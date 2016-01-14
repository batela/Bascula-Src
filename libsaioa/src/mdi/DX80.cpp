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
  valores[0] = valores[1] = valores[2] = valores[3] = 0;
  isOKInput1=isOKInput2=isOKInput3=isOKInput4=true;
  tolCM=atoi(Env::getInstance()->GetValue("toleranciacm").data());
  tolPeso=(float)(atof(Env::getInstance()->GetValue("toleranciapesoerror").data())/(float)100);
  offSetPeso=13107;
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
      //differencias[idx++]= Abs(valores[i]- valores [k]);
      differencias[idx++]= (valores[i]- valores [k]);
    }
  }
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

  (alarmas[0]==false)?isOKInput1=true:isOKInput1=false;
  (alarmas[1]==false)?isOKInput2=true:isOKInput2=false;
  (alarmas[2]==false)?isOKInput3=true:isOKInput3=false;
  (alarmas[3]==false)?isOKInput4=true:isOKInput4=false;
  //printf ("Alarmas***************: %d %d %d %d",alarmas[0],alarmas[1],alarmas[2],alarmas[3] );
}
} /* namespace container */
