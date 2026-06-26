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


//ENTRADAS
//Serial
#define CFG_SERIAL Serial.begin(9600)
#define HAY_DATO (Serial.available() > 0)
#define IMPRIMIR_SERIAL(X) Serial.println(X) 
#define LEER_DATO 	Serial.read()

//PULSADOR_ABRE
#define PIN_PULSADOR_ABRE         19
#define CFG_PULSADOR_ABRE         pinMode(PIN_PULSADOR_ABRE,INPUT)
#define PULSADOR_ABRE_ACTIVADO    digitalRead(PIN_PULSADOR_ABRE)

//control PULSADOR_CIERRA
#define PIN_PULSADOR_CIERRA       18
#define CFG_PULSADOR_CIERRA       pinMode(PIN_PULSADOR_CIERRA,INPUT)
#define PULSADOR_CIERRA_ACTIVADO  digitalRead(PIN_PULSADOR_CIERRA)

//FC_CORTINA_CERRADA 
#define PIN_FC_CORTINA_CERRADA    17
#define CFG_FC_CORTINA_CERRADA    pinMode(PIN_FC_CORTINA_CERRADA, INPUT)
#define ESTA_CERRADA              digitalRead(PIN_FC_CORTINA_CERRADA)

//FC_CORTINA_ABIERTA (Agustin)
#define PIN_FC_CORTINA_ABIERTA    16
#define CFG_FC_CORTINA_ABIERTA    pinMode (PIN_FC_CORTINA_ABIERTA, INPUT)
#define ESTA_ABIERTA              digitalRead(PIN_FC_CORTINA_ABIERTA)


//SALIDA 
//LED_TEST
#define PIN_LED_TEST              13
#define CFG_LED_TEST              pinMode(PIN_LED_TEST, OUTPUT)
#define PRENDER_LED_TEST          digitalWrite(PIN_LED_TEST, HIGH)
#define APAGAR_LED_TEST           digitalWrite(PIN_LED_TEST, LOW)
#define AJUSTAR_LED_TEST(x)       digitalWrite(PIN_LED_TEST, x) 

//MOTOR_MARCHA (Solange)
#define PIN_MOTOR_MARCHA          14
#define CFG_MOTOR_MARCHA          pinMode(PIN_MOTOR_MARCHA,OUTPUT)
#define ENCENDER_MOTOR            digitalWrite(PIN_MOTOR_MARCHA,HIGH)
#define DETENER_MOTOR             digitalWrite(PIN_MOTOR_MARCHA,LOW)

//MOTOR_ABRE
#define PIN_MOTOR_ABRE            15
#define CFG_MOTOR_ABRE            pinMode(PIN_MOTOR_ABRE,OUTPUT)
#define MOTOR_ABRIR               digitalWrite(PIN_MOTOR_ABRE,HIGH)
#define MOTOR_CERRAR              digitalWrite(PIN_MOTOR_ABRE,LOW)

#define MOTOR_APAGADO 0
#define MOTOR_ABRE 1
#define MOTOR_CIERRA 2
unsigned char motor = MOTOR_APAGADO;

#define CORTINA_ABRIR 0
#define CORTINA_ABRIENDO 1
#define CORTINA_ABIERTA 2
#define CORTINA_CERRAR 3
#define CORTINA_CERRANDO 4
#define CORTINA_CERRADA 5
#define CORTINA_DETENER 6
#define CORTINA_DETENIDA 7

unsigned char estado_cortina = CORTINA_DETENIDA;
unsigned long tiempo_inicio = 0; 
unsigned long tiempo_apertura_total = 0;
unsigned long tiempo_cierre_total = 0; 

bool se_presiono_pulsador_abrir=0, se_presiono_pulsador_cerrar=0;
//—---------------------------------


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



void RecepcionSerie(void)
{
	char comando;
	if(!HAY_DATO)return;
comando = LEER_DATO;


	switch (comando)
{
	case ‘a’: 
case ‘A’:
		estado_cortina=CORTINA_ABRIR;
		break;
case ‘c’: 
case ‘C’:
		estado_cortina=CORTINA_CERRAR;
		break;
case ‘d’: 
case ‘D’:		
		estado_cortina=CORTINA_DETENER;
break;
}

}

void TransmisionSerie(void)
{
//Para que se escriba el cambio de estado una vez por cambio.
static char estado_cortina_ant = CORTINA_DETENIDA;
	if(estado_cortina == estado_cortina_ant) return; //early return
	estado_cortina_ant = estado_cortina;

	switch(estado_cortina)
{
	case CORTINA_ABRIENDO:  IMPRIMIR_SERIAL("Abriendo");  break;
	case CORTINA_ABIERTA:   IMPRIMIR_SERIAL("Abierta");   break;
	case CORTINA_CERRANDO:  IMPRIMIR_SERIAL("Cerrando");  break;
	case CORTINA_CERRADA:   IMPRIMIR_SERIAL("Cerrada");   break;
	case CORTINA_DETENIDA:  IMPRIMIR_SERIAL("Detenida");	 break;
}
}


LedTest();

  CtrlPulsador();
  CtrlCortina();
  CtrlMotor();
  RecepcionSerie();

  TransmisionSerie();
  //TestFC(); 
  //TestPULSADORES();
