#define INTERVALSEND 30000
#define MINCHANGE 100
 
#include "SoftwareSerial.h"
SoftwareSerial GPRS(7, 8);
//int onModulePin= 9;
char aux_str[150];
char aux;
char data[512];
int data_size;
uint8_t answer=0;
 
 
#include "TinyGPS.h"
TinyGPS gps;
//Tx, Rx 
SoftwareSerial gpsSerial(7, 8);
long lat, lon;
long endlatsend=0; 
long endlonsend;
unsigned long time1,date1;
unsigned long age;
bool newdata = false;
unsigned long millis1=0;
unsigned long millissend=0;
unsigned long millisdata=0;
 
 
char apn[]="internet.beeline.ru";
char url[150];
String surl="kdsystem.noip.me/keeneye/write.php?car_id=";
int id_avto=1;
 
void setup()
  {
  GPRS.begin(19200);               // the GPRS baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  //gpsSerial.begin(9600);
  Serial.println("Starting...");
  //pinMode(onModulePin,OUTPUT);
   
  //pinMode(12,OUTPUT);
  //digitalWrite(12,LOW);
   
  power_on();
  delay(3000);
  //sendATcommand("AT", "OK", 2000);
  //delay(3000);
  // sets APN , user name and password
  sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);
  snprintf(aux_str, sizeof(aux_str), "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);
  while (sendATcommand("AT+SAPBR=1,1", "OK", 2000) == 0)
    {
    delay(2000);
    }
  delay(1000);
  }
 
void loop() 
  {
    //digitalWrite(12,HIGH);
    GPRS.end();gpsSerial.begin(9600);
    while (millis() - millis1 < 2000) 
      {
      if (readgps())
        newdata = true; 
      }   
    if (newdata) {
      gps.get_position(&lat, &lon, &age);
      millisdata=millis();
      Serial.print("lat=");Serial.print(lat);
      Serial.print("  lon=");Serial.print(lon);     
      gps.get_datetime(&date1, &time1, &age);
      Serial.print(" date=");Serial.print(date1);
      Serial.print("  time=");Serial.println(time1);     
      newdata=false;
    }
   //if(millis()-millissend>INTERVALSEND && millis()-millisdata<INTERVALSEND
      //&& abs(lat-endlatsend)>MINCHANGE && abs(lon-endlonsend)>MINCHANGE)
  if(millis()-millissend>INTERVALSEND && millis()-millisdata<INTERVALSEND
      && abs(lat-endlatsend)>MINCHANGE && abs(lon-endlonsend)>MINCHANGE
      )
     {
     GPRS.begin(19200);gpsSerial.end();
     // Initializes HTTP service
     answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
     if (answer == 1)
       {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        if (answer == 1)
          {// Sets url 
           endlatsend=lat;endlonsend=lon;
           String surl1=set_url_avto()+"&lat="+String(lat)+"&lon="+String(lon);
           surl1+="&date="+String(date1)+"&time="+String(time1);
           surl1.toCharArray(url,surl1.length()+1);
           snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\"", url);
           answer = sendATcommand(aux_str, "OK", 5000);
           if (answer == 1)
           {// Starts GET action
           answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 10000);
           if (answer == 1)
             {
             sprintf(aux_str, "AT+HTTPREAD");
             sendATcommand(aux_str, "OK", 5000);
             }
           else
             {
             Serial.println("Error getting the url");
             }
           }
         else
           {
           Serial.println("Error setting the url");
           }
         }
       else
         {
         Serial.println("Error setting the CID");
         }    
       }
    else
       {
       Serial.println("Error initializating");
       }
    sendATcommand("AT+HTTPTERM", "OK", 5000);
    millissend=millis();
    }
  else
    Serial.println("data not change!!!");
  millis1=millis();
  GPRS.begin(19200);gpsSerial.end();
  }
// отправка AT-команд
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
  {
   uint8_t x=0,  answer=0;
   char response[150];
   unsigned long previous;
 
   memset(response, '\0', 150);    // Initialize the string
   delay(100);
   while( GPRS.available() > 0) GPRS.read();    // Clean the input buffer
   GPRS.println(ATcommand);    // Send the AT command 
   x = 0;
   previous = millis();
   // this loop waits for the answer
   do{
     if(GPRS.available() != 0)
       {    
       // if there are data in the UART input buffer, reads it and checks for the asnwer
       response[x] = GPRS.read();
       //Serial.print(response[x]);
       x++;
       // check if the desired answer  is in the response of the module
       if (strstr(response, expected_answer) != NULL)    
         {
         answer = 1;
         }
       }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    
    Serial.println(response);
        return answer;
  }
// программное включение питания
void power_on()
  {
  uint8_t answer=0;
  //answer = sendATcommand("AT+CPOWD=1", "OK", 2000);
  //pinMode(onModulePin,OUTPUT);
  // checks if the module is started
  //digitalWrite(onModulePin,LOW);
  //delay(1000);
  //digitalWrite(onModulePin,HIGH);
  //delay(2000);
  //digitalWrite(onModulePin,LOW);
  //delay(3000);
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
    {
    //digitalWrite(onModulePin,LOW);
    //delay(1000);
    //digitalWrite(onModulePin,HIGH);
    //delay(2000);
    //digitalWrite(onModulePin,LOW);
    //delay(3000);
    Serial.println("POWER!!!!");
     
       
    // power on pulse
    //digitalWrite(onModulePin,HIGH);
    //delay(3000);
    //digitalWrite(onModulePin,LOW);
    // waits for an answer from the module
    while(answer == 0)
      {  
      // Send AT every two seconds and wait for the answer   
      answer = sendATcommand("AT", "OK", 2000);    
      }
    }
  }
// проверка наличия данных gps
bool readgps() 
  {
  while (gpsSerial.available())
    {
    int b = gpsSerial.read();
    //в TinyGPS есть баг, когда не обрабатываются данные с \r и \n
    if('\r' != b) 
      {
      if (gps.encode(b))
          return true;
      }
    }
  return false;
  }
   
 String set_url_avto()
  {
  String surl1;
  id_avto=1;
  //if(digitalRead(14)==1)
  //  id_avto=id_avto+1;
  //if(digitalRead(15)==1)
  //  id_avto=id_avto+2;
  //if(digitalRead(16)==1)
  //  id_avto=id_avto+4;
  surl1=surl+String(id_avto);
  Serial.print("surl1=");Serial.println(surl1); 
  id_avto=0;
  return surl1; 
  }
