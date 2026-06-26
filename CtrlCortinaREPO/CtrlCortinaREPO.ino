//Nombre de cada estudiante:
// Ing. Santiago Giles 1 2 3 4
// Bascal Francisco Emanuel; 
// Buss Agustin; 
// Fanjul Luca
// Exequiel Bustos
// Elias Schiel
// Facundo Neveu
// Solange Gerbuez
// Fuentes Facundo
// Manuel Ponce
// Elizondo Julian

/*
Asignaciones para completar el código:

Definiciones --> BASCAL, Francisco Emanuel
Setup --> PONCE, Manuel Andres
loop() --> BUSTOS, Exequiel Yoel
TestFC() --> BUSS, Agustin
TestPULSADORES() --> BASCAL, Francisco Emanuel
LedTest () --> GERBUEZ SOLANGE, Abigail
CtrlMotor(void) --> FANJUL, Luca Miguel
CtrlCortina() --> SCHIEL, Elias Martin
CtrlPulsador(void) --> BUSS, Agustin
RecepcionSerie(void) --> BASCAL, Francisco Emanuel
TransmisionSerie(void) --> BUSTOS, Exequiel Yoel
*/
void setup() {
  CFG_SERIAL;
  CFG_PULSADOR_ABRE;
  CFG_PULSADOR_CIERRA;
  
  CFG_FC_CORTINA_CERRADA;
  CFG_FC_CORTINA_ABIERTA;
  CFG_MOTOR_MARCHA;
  CFG_MOTOR_ABRE; 
  CFG_LED_TEST;
 
  if (ESTA_ABIERTA) estado_cortina = CORTINA_ABIERTA;
  if (ESTA_CERRADA) estado_cortina = CORTINA_CERRADA;
  else {estado_cortina = CORTINA_DETENIDA;}
  
  DETENER_MOTOR;
}

void loop() {
  // put your main code here, to run repeatedly:

}

void LedTest ()
  {
  static unsigned long millis_ant = 0;
  static bool blink = 0;
  static unsigned int tiempo_destello=1000;

  if (millis() - millis_ant < tiempo_destello) return;
  millis_ant = millis();
   
  tiempo_destello = (motor == MOTOR_ABRE || motor == MOTOR_CIERRA) ? 100 : 1000;
    
  blink = !blink;
  AJUSTAR_LED_TEST(blink);

  }



void CtrlCortina()
  {
  	int tiempo_maximo = 10000;
switch(estado_cortina)    
      {
      case CORTINA_ABRIR:
        if(ESTA_ABIERTA) 
          {
          estado_cortina=CORTINA_ABIERTA;
          break;
          }          
        motor = MOTOR_ABRE;
        estado_cortina++;
        tiempo_inicio = millis(); 
        break; 
      case CORTINA_ABRIENDO:
if(millis()-tiempo_inicio > tiempo_maximo) {estado_cortina = CORTINA_DETENER; break; }        
   if(se_presiono_pulsador_abrir || se_presiono_pulsador_cerrar)    {estado_cortina=CORTINA_DETENER;break;}
        if(!ESTA_ABIERTA) break;
        motor = MOTOR_APAGADO;
        estado_cortina++;
	tiempo_apertura_total = (millis() - tiempo_inicio)/1000;
      	

        break; 
      case CORTINA_ABIERTA: 
        if(se_presiono_pulsador_cerrar) estado_cortina=CORTINA_CERRAR;
        break;
      case CORTINA_CERRAR:
        if(ESTA_CERRADA)
          {
          estado_cortina=CORTINA_CERRADA;
          break;
          }
        motor = MOTOR_CIERRA;
        estado_cortina++;
	tiempo_inicio = millis();

        break; 
      case CORTINA_CERRANDO:
if(millis()-tiempo_inicio > tiempo_maximo) {estado_cortina = CORTINA_DETENER; break; }        
if(se_presiono_pulsador_abrir || se_presiono_pulsador_cerrar)   
 	{estado_cortina=CORTINA_DETENER; break;}
        if(!ESTA_CERRADA) break;
        motor = MOTOR_APAGADO;
        estado_cortina++;
tiempo_cierre_total = (millis() - tiempo_inicio)/1000;
      	

        break; 
      case CORTINA_CERRADA: 
        if(se_presiono_pulsador_abrir) estado_cortina=CORTINA_ABRIR;
        break; 
      case CORTINA_DETENER:
        motor = MOTOR_APAGADO;
        estado_cortina++;
        break; 
      case CORTINA_DETENIDA:  
        if(se_presiono_pulsador_abrir)   {estado_cortina=CORTINA_ABRIR; break;}
        if(se_presiono_pulsador_cerrar)  {estado_cortina=CORTINA_CERRAR; break;}
        break;
      }
  }


