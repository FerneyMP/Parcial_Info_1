//   ************************ Manual ************************
/* ¡Bienvenido al programa que administra un circuito de LEDS 8x8!

 ° A continuación se describe un pequeño manual de uso para este programa:
 
 * Se mostrará en pantalla un menú con diferentes opciones.
 * El usuario tendrá habilitado la opción de revisar si las conexiones del Hardware están
 * correctamente conectadas.
 * El usuario puede optar por ingresar un patrón o declinarse. 
 * El usuario podrá generar un patrón cualquiera a partir del ingreso de datos en el rango de
 * 1 a 64 por el monitor serial del Arduino, estos números a ingresar representan el número del
 * LED que se desea encender.
 * La matriz se empieza a mover de izquierda a derecha y de arriba hacia abajo.
 * El usuario podrá volver al menú principal para seguir navegando por las opciones, como lo es 
 * ingresar una secuencia de patrones, indicando el tiempo de visualización entre dichos patrones.
  
 ° Conexión con el Arduino:
 
 * En los pines 13, 12, 11 del Arduino, se hace el enlazamiento con el integrado 74HC595.
 * El pin 13 del Arduino se conecta a la entrada (SER) del integrado, el pin 12 va conectado al
 * reloj de registro de salida (RCLK) y el pin 11 va conectado directamente al reloj de registro
 * de desplazamiento (SRCLK). El votaje debe de ir conectado en el pin 16 del integrado (Vcc)
 * y la activación de salida (OE) junto con el pin 8 del integrado (GND) deben ir a tierra.
 * Las salidas 1, 2, 3, 4, 5, 6, 7, 8 van conectados a resistencias de 560 Ohms y a su vez
 * van conectadas a los correspondientes LEDS.
 * Ahora bien, para hacer la multiplexación con los demás integrados, se deben enlazar la salida
 * invertida (QH') con la entrada (SER) del integrado contiguo, luego los relojes de registro
 * de salida (RCLK-RCLK) y los relojes de registro de desplazamiento (SRCLK-SRCLK).
 
 ° Ejemplos:

 1) 
 
 Ingrese una opcion:
  1. Desea comprobar que todos los LEDS funcionan correctamente 
  2. Desea mostrar un patron 
  3. Desea mostrar una secuencia de patrones 
  4. Apagar LEDS 
 
 R/ Si se ingresa la opción 1, el resultado esperado es que se enciendan los LEDS durandte
   un segundo y luego se apagan automáticamente.
   
 2) Si se ingresa la opción 2, el usuario podrá ingresar números enteros del 1 al 64
   para representar un patrón deseado. Luego aparecerá en pantalla una opción que le permite
   volver al menú principal y al instante en que marque otra opción se apagarán los LEDS.
   
 3) Si se ingresa la opción 3, el usuario podrá ingresar una secuencia de patrones, teniendo
   en cuenta las consideraciones explicadas en el ejemplo 2.
   
   ¡Precaución!, se sugiere no crear más de 7 patrones debido a la limitación de memoria del
    Arduino.

 * ¡Esperamos que pueda disfrutar de ingresar los patrones y encender los LEDS a gusto propio! */

/* INICIO DEL PROGRAMA */

const int SER = 13;
const int RCLK = 12;
const int SRCLK = 11;
int *arreglo_LEDS= new int[64];

void setup(){  
  // configuración de puertos digitales como salida
  for (unsigned int i = 13; i>10; i--) pinMode(i, OUTPUT);
  for (unsigned int j = 13; j>10; j--) digitalWrite(j, 0);  
  Serial.begin(9600);  
 }

//Funcion CLOCK1 ---- Almacena datos (1 / 0) en el integrado 74HC595
void CLOCK1(){
  // Para activar el reloj de la primera etapa de los registros (registros de entrada)
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1);
    digitalWrite(SRCLK, 0);
}

 //Funcion CLOCK2 ---- Conecta los datos almacenados en CLOCK1 a los pines del integrado
void CLOCK2(){  
 // Activación del reloj de segunda etapa de registros (registros de salida)    
    digitalWrite(RCLK, 0);
    digitalWrite(RCLK, 1);
    digitalWrite(RCLK, 0);
}

//Funcion verificación --- revisa que todos los leds enciendan y no tiene un valor de retorno.
void verificacion()
{
  int contador=0;
  while (contador <64){  
    digitalWrite(SER, 1);
    CLOCK1();
    CLOCK2();
    delay(50);
    contador++;
  }
}

/*La funcion imagen le permite al usuario ingresar datos
 * por el puerto serial mediante el monitor en serie, además
 * captura dichas entradas y forma un arreglo con los valores
 * ingresados y retorna dicho arreglo */
int *imagen()
{ 
  int captura_LED;
  char captura=0;
  bool bandera = false;
  
  while (bandera == false)
  {
    Serial.println("Desea encender un LED  [S/n]");
    while (!Serial.available()>0);
    captura = Serial.read();
    
    if (captura == 's'||captura == 'S')
    {
      do 
      {
        Serial.println("Ingrese el numero del LED que desea encender (ENTRE 1 Y 64)");
		while (!Serial.available()>0);
        captura_LED = Serial.parseInt();
        
        if (captura_LED <1 || captura_LED>64)
        {
          Serial.println("El valore no esta permitido, intente nuevamente.");          
        }
        else break;
      }
      while (true);
      
      arreglo_LEDS[65-captura_LED]= 1;     
    }
    else if (captura == 'n'||captura == 'N')
    {
      bandera = true;
    }
  }
  return arreglo_LEDS;  
}

/* La función leer arreglo recibe como parámetro de entrada
 * un arreglo. Su funcionalidad se enfoca en el encendido de 
 * los leds mediante la indexación de un contador en el arreglo
 * recibido. */
void *leerarreglo(int arreglo[]){
    // ciclo while que lee y enciende los leds.
  	int c = 0;
    while(c<=64)
  {
    if (arreglo_LEDS[c]==1)
    {
      digitalWrite(SER,1);
        CLOCK1();
        CLOCK2();
        c++;
    }
    else
    {
      digitalWrite(SER,0);
          CLOCK1();
          CLOCK2();
          c++;
    }      
  }
}

/* La función reset es tipo puntero, recibe un puntero y le asigna el valor de 0
 * a todas las entradas de un arreglo. Retorna un arreglo.
 */
int *reset(int *_arreglo_LEDS)
{
  for (int r = 0; r <= 64; r++)
  {
    arreglo_LEDS[r]=0;
  }
  return _arreglo_LEDS;
}

/* La función publik no tiene argumentos, recibe la cantidad de patrones en secuencia que
 * se van a presenciar, crea un arreglo multidimensional e inicia la secuencia de encendido
 * y apagado de los patrones ingresados mediante la reutilización de la función imagen().
 * No retorna nada.
 */
void publik()
{
  bool bandera=true;
  int C_segundos,c=1,c2;
  int patrones; 
  
  Serial.println("Ingrese la cantidad de patrones que desea formar: ");
  while (!Serial.available()>0);
  patrones = Serial.parseInt();
  
  Serial.println("Ingrese el tiempo de visualizacion entre cada patron ... (en milisegundos)");
  while (!Serial.available()>0);
  C_segundos = Serial.parseInt();
 
  int **arreglo_2D = new int *[patrones];
  
  for (int profundidad=0; profundidad<patrones; profundidad++) *(arreglo_2D+profundidad) = new int[64];
  
  for (int i=0; i < patrones; i++)
  {
    
    Serial.print("Inicia Patron: ");Serial.println(c); Serial.println("n para terminar el patron");
    imagen();
    c++;
    for (int posiciones = 0; posiciones <= 64; posiciones++){
      arreglo_2D[i][posiciones]= *(arreglo_LEDS+posiciones);
    }
    reset(arreglo_LEDS);
  } 
  
  // Ciclo que desplaza los ( 1 / 0 ) en la multiplexación de los integrados (ON - OFF)
  c = 0;
  c2 = 0;
  Serial.println("Presione un caracter para volver al menu principal");
  while (bandera==true)
  {
  while(c2<=patrones)
    {
    while(c<=64)
    {
      if (arreglo_2D[c2][c]==1)
      {
        digitalWrite(SER,1);
          CLOCK1();
          CLOCK2();
          c++;
      }
      else 
      {
        digitalWrite(SER,0);
            CLOCK1();
            CLOCK2();
            c++;
      }
    }
    delay(C_segundos);
    c2++;
    c=0;
    }
    c2=0;
    
    if (Serial.available() > 0)
    {
      char temp=Serial.read();
      apagarLEDS2d(patrones);
      bandera=false;
    }  
  } 
  delete[] arreglo_2D;  
}

/* La función apagarLEDS no tiene argumento, su funcionalidad es que coge un arreglo unidimensinal
 * y "apaga" la matriz de LEDS, mediante el envío de ceros (0) a los integrados.
 * No retorna nada.
 */
void apagarLEDS()
{
  int arreglo[64]={0};
  int c = 0;
    while(c<=64)
  {
    if (arreglo[c]==1)
    {
      digitalWrite(SER,0);
        CLOCK1();
        CLOCK2();
        c++;
    }
    else
    {
      digitalWrite(SER,0);
          CLOCK1();
          CLOCK2();
          c++;
    }      
  }  
}

/* La función apagarLEDS2d recibe como argumento un entero (#de patrones), 
 * su funcionalidad es que indexa el arreglo multidimensional y "apaga" 
 * la matriz de LEDS, mediante el envío de ceros (0) a los integrados.
 * No retorna nada.
 */

void apagarLEDS2d(int cPatrones)
{
  int arreglo2de0[cPatrones][64]={0};
  
  int c = 0; int c2 = 0;

  while(c2<=cPatrones)
    {
    while(c<=64)
    {
      if (arreglo2de0[c2][c]==1)
      {
        digitalWrite(SER,0);
          CLOCK1();
          CLOCK2();
          c++;
      }
      else 
      {
        digitalWrite(SER,0);
            CLOCK1();
            CLOCK2();
            c++;
      }
    }    
    c2++;
    c=0;    
    }    
    c2=0; 
}

/* La función minimenu gestiona las diferentes funciones desarrolladas
 * en el código, por lo que, le brinda al usuario distintos tipos
 * de entrada, como chequeo del funcionamiento de leds, ingreso de patrones, ingreso
 * de secuencia de patrones. No tiene argumento y no retorna nada.
 */
void minimenu()
{  
  int dato;
  bool bandera=false;
  while (bandera!=true)
  {
    Serial.println("Ingrese una opcion:");
  	Serial.println("1. Desea comprobar que todos los LEDS funcionan correctamente ");
  	Serial.println("2. Desea mostrar un patron ");  
  	Serial.println("3. Desea mostrar una secuencia de patrones ");
    while (!Serial.available()>0);
    dato=Serial.parseInt();
  
    switch (dato)
    {    
      case 1:
      verificacion();
      delay(1000);
      apagarLEDS();
      break;

      case 2:
      char temp;
      int *x;
      x = imagen();
      leerarreglo(x);
      reset(arreglo_LEDS);
      
      Serial.println("Presione un caracter para volver al menu");
      while (!Serial.available()>0);
      temp=Serial.read();
      apagarLEDS();      
      break;
      
      case 3:      
      publik();
      break;
      
      default:
      Serial.println("Valor no permitido, intente nuevamente");
      break;
    }
  }
}

/* Función que ejecuta una invocación de una función de manera repetitiva.
*/
void loop()
{  
  minimenu();
}

/* FIN DEL PROGRAMA */