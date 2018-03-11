#include <SoftwareSerial.h>
char inChar;  //used to decode gps
int index;
char inData[150];
char ttime[11];
char longitude[10];
char nsid[2];
char latitude[11];
char ewid[2];
int8_t answer;

char frame[150];

SoftwareSerial GPRS(7, 8);

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout) {
  uint8_t answer = 0;
  char response[150];
  unsigned long previous;

  memset(response, '\0', 150);    // Initialize the string
  while ( GPRS.available() > 0) GPRS.read();   // Clean the input buffer
  GPRS.println(ATcommand);    // Send the AT command
  index = 0;
  previous = millis();
  do {
    if (GPRS.available() != 0) {
      response[index] = GPRS.read();
      index++;
      if (strstr(response, expected_answer) != NULL) {
        answer = 1;
      }
    }
  }
  while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(response);
  return answer;
}



void setup() {
  GPRS.begin(19200);
  Serial.begin(19200);
  Serial.println("Starting...");
  sendATcommand("AT", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"internet.beeline.ru\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"USER\",\"beeline\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"PWD\",\"beeline\"", "OK", 2000);
  //sendATcommand("AT+SAPBR=1,1", "OK", 2000);
  sendATcommand("AT+SAPBR=2,1", "OK", 2000);
  //sendATcommand("AT CGPSIPR=19200", "OK", 2000);
  sendATcommand("AT+CGPSPWR=1", "OK", 2000);
  delay(30000);
  //while( (sendATcommand("AT+CGPSSTATUS?", "2D Fix", 5000) || sendATcommand("AT+CGPSSTATUS?", "3D Fix", 5000)) == 0 );
  
  sendATcommand("AT+CGPSSTATUS?", "OK", 2000);
}

void Send2SRV() {
  char tempstr[200];
  char aux_str[30];
  char url[] = "kdsystem.noip.me";
  
  //tempstr = String("AT+HTTPPARA=\"URL\",\"alielectronics.ru/gps/write.php?ttime=" + ttime + "&longitude=" + longitude + "&nsid=" + nsid +"&latitude=" + latitude +"&ewid=" + ewid +"\"");
  //tempstr = "AT+HTTPPARA=\"URL\",\"alielectronics.ru/gps/write.php?ttime="+ttime+"\"";
  //GPRS.println(temstr);
  // Sets url 
//  Serial.print("url = ".url);
  sprintf(aux_str, "AT+HTTPPARA=\"URL\",\"http://%s/keeneye/write.php?", url);
  Serial.print(aux_str);
  sprintf(frame, "ttime=%s&latitude=%s&longitude=%s", ttime, latitude, longitude);
  Serial.print(frame);
  answer = sendATcommand("\"", "OK", 5000);
  //Serial.println(answer);
}

void read_GPS() {
  GPRS.println("AT+CGPSINF=2\r\n\r\n");  //ask SIM908 GPS info
  read_String();  //read and store serial answer
  Serial.println(inData);
  
  strtok(inData, ",");
  strcpy(ttime, strtok(NULL, ",")); // Gets date
  strcpy(longitude, strtok(NULL, ",")); // Gets longitude
  strcpy(nsid, strtok(NULL, ",")); // Gets date
  strcpy(latitude, strtok(NULL, ",")); // Gets latitude
  strcpy(ewid, strtok(NULL, ",")); // Gets date

  // convert2Degrees(latitude);  //fix value
  // convert2Degrees(longitude); //fix value

  //Serial.println("received data:");
  Serial.println(nsid);
  Serial.println(longitude);
  Serial.println(ewid);
  Serial.println(latitude);
  Serial.println(ttime);
  Serial.println("---------------------------------");
  //Send2SRV();
}

void read_String () {
  long previous;
  index = 0;
  answer = 0;
  memset(inData, '\0', 150);    // Initialize the string
  previous = millis();
  do {
    if (GPRS.available() != 0) {
      inData[index] = GPRS.read();
      index++;
      if (strstr(inData, "OK") != NULL) { answer = 1;
      }
    }
  }
  while ((answer == 0) && ((millis() - previous) < 2000));
  inData[index - 3] = '\0';
}

void loop() {
  read_GPS();
  delay(5000);
}
