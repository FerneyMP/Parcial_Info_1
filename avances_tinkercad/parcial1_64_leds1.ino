const int SER = 13;
const int RCLK = 12;
const int SRCLK = 11;


//memoria dinámica
int *arreglo_LEDS= new int[64];

void setup(){  
  // configuración de puertos digitales como salida
  for (unsigned int i = 13; i>10; i--) pinMode(i, OUTPUT);
  for (unsigned int j = 13; j>10; j--) digitalWrite(j, 0);  
  Serial.begin(9600);
  
 }

//Funcion #1 ---- Almacena datos (1 / 0)
void CLOCK1(){
  // Para activar el reloj de la primera etapa de los registros (registros de entrada)
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1);
    digitalWrite(SRCLK, 0);
}
//Funcion #2 ---- Conecta los datos almacenados en CLOCK1 a los pines del integrado
void CLOCK2(){  
 // Activación del reloj de segunda etapa de registros (registros de salida)    
    digitalWrite(RCLK, 0);
    digitalWrite(RCLK, 1);
    digitalWrite(RCLK, 0);
}
//Funcion #3 --- revisa que todos los leds enciendan
void verificacion()
{
  int contador=0;
  while (contador <64){  
    digitalWrite(SER, 1);
    CLOCK1();
    //delay(50);
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
    //Serial.setTimeout(5000);
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
    // ciclo while lo lee y enciende los leds.
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

/* La función minimenu gestiona las diferentes funciones desarrolladas
 * en el código, por lo que, le brinda al usuario distintos tipos
 * de entrada, como chequeo del funcionamiento de leds, ingreso de patrones.*/

int *reset(int *_arreglo_LEDS)
{
  for (int r = 0; r < 64; r++)
  {
    arreglo_LEDS[r]=0;
  }
  return _arreglo_LEDS;
}

void publik()
{
  bool bandera=true;
  int C_segundos,c=1,c2;
  int patrones; 
  
  Serial.println("Ingrese la cantidad de patrones que desee formar: ");
  while (!Serial.available()>0);
  patrones = Serial.parseInt();
  
  Serial.println("frecuencia (en milisegundos)");
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
  
 c = 0;
  c2 = 0;
  Serial.println("Presione un caracter para volver al menu");
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
      //delete[] arreglo_LEDS;
      
      
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
 
void loop()
{  
  minimenu();
}

