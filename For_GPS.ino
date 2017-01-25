 
 #include <LiquidCrystal.h>
 #include <string.h>
 #include <ctype.h>
 
 LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
 int ledPin = 9;                  // LED test pin
 int rxPin = 0;                    // RX PIN 
 int txPin = 1;                    // TX TX
 int byteGPS=-1;            //incoming temp variable assign as -1 to check if incoming byte replaced it
 char linea[300] = "";                                //receiving buffer for proccess
 char comandoGPR[7] = "$GPRMC";    //string looking for
 int cont=0;
 int bien=0;
 int conta=0;                                          //index for receiving buffer
 int indices[13];
 int z=0;
 char lat[10];            //As latitude is of length 10
 char longi[11];        // As longitude is of length 11
 void setup() 
{
 pinMode(ledPin, OUTPUT);       // Initialize LED pin
 pinMode(rxPin, INPUT);
 pinMode(txPin, OUTPUT);
 Serial.begin(4800);            //baud rate for serial monitor
 Serial1.begin(4800);          //baud rate for GPS module
 lcd.begin(16, 2);                  //LCD initilization
 lcd.print("Waiting for GPS");
 for (int i=0;i<300;i++)
 {    
 // Initialize a buffer for received data
 linea[i]=' ';
 }   
 }
 void loop() 
 {
 digitalWrite(ledPin, HIGH);
 byteGPS=Serial1.read();         // Read a byte of the serial port
 if (byteGPS == -1) 
 {           // See if the port is empty yet
 delay(100); 
 } 
 else 
 {
 // note: there is a potential buffer overflow here!
 linea[conta]=byteGPS;        // If there is serial port data, it is put in the buffer
 conta++;                      
 if (byteGPS==13)
 {            // If the received byte is = to 13, end of transmission
 digitalWrite(ledPin, LOW); 
 cont=0;
 bien=0;
 for (int i=1;i<7;i++)
 {     // Verifies if the received command starts with $GPR
 if (linea[i]==comandoGPR[i-1])
 {
 bien++;
 }
 }
 if(bien==6)
 {               // If yes, continue and process the data
 for (int i=0;i<300;i++)
 {
 if (linea[i]==',')
 {    // check for the position of the  "," separator
 indices[cont]=i;
 cont++;
 }
 if (linea[i]=='*')
 {    // ... and the "*"
 indices[12]=i;
 cont++;
 }
 }
 Serial.println("");      // ... and write to the serial port
 Serial.println("");
 Serial.println("---------------");
 for (int i=0;i<12;i++)
{
 switch(i)
 {
 case 0 :Serial.print("Time in UTC (HhMmSs): ");break;
 case 1 :Serial.print("Status (A=OK,V=KO): ");break;
 case 2 :Serial.print("Latitude: ");break;
 case 3 :Serial.print("Direction (N/S): ");break;
 case 4 :Serial.print("Longitude: ");break;
 case 5 :Serial.print("Direction (E/W): ");break;
 case 6 :Serial.print("Velocity in knots: ");break;
 case 7 :Serial.print("Heading in degrees: ");break;
 case 8 :Serial.print("Date UTC (DdMmAa): ");break;
 case 9 :Serial.print("Magnetic degrees: ");break;
 case 10 :Serial.print("(E/W): ");break;
 case 11 :Serial.print("Mode: ");break;
 case 12 :Serial.print("Checksum: ");break;
 }
//           lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Lat:  ");
lcd.setCursor(0, 1);
lcd.print("Longi:");


//        
 for (int j=indices[i];j<(indices[i+1]-1);j++)
 {

 Serial.print(linea[j+1]); 
 if(i==2)
 {
z=indices[i];
lat[j-z]=linea[j+1];
 lcd.setCursor(6, 0);
 lcd.print(lat);
 }
 
 if(i==4)
 {
z=indices[i];
longi[j-z]=linea[j+1];
 lcd.setCursor(6, 1);
 lcd.print(longi);
 }
 }
 Serial.println("");
 }
//                       Serial.println("---------------");
 }
 conta=0;                    // Reset the buffer
 for (int i=0;i<300;i++)
 {    //  
 linea[i]=' ';             
 }                    
 }
 }
 }
