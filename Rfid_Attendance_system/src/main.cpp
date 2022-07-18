#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>  
#include<WiFiManager.h>
#include <SD.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = 19800;

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

#define Firebase_Auth "VmIUblY84GeW4lK1trBmn0evg1WgBtfNCEE17zQU"
#define Firebase_Host "iotatt-95af0-default-rtdb.firebaseio.com"

Servo servo;

FirebaseData firebaseData , firebasedata1;

constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

int blockNum = 2;

byte bufferLen = 18;

byte readBlockData[18];

MFRC522::StatusCode status;

EspClass esp;

int count = 0;   //counter for firbase key value
WiFiManager wifiManager;

void setup()
{

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.begin(9600);

  wifiManager.autoConnect("AutoConnectWIFI"); 
  


 //  WiFi.begin("Honey","0987654321");
  
  Serial.println("Connected.....");

  Firebase.begin(Firebase_Host, Firebase_Auth);    // Begin firebase cloud
  timeClient.begin();

  Serial.println("Scan a MIFARE 1K Tag ");

  servo.attach(D2, 540, 2400);
  servo.write(0);
  delay(2000);
}

void loop()
{

     Firebase.getString(firebasedata1 ,"/wifi/value");

       String reset = "reset";


     if(reset == firebasedata1.stringData())
     {
       if(Firebase.setString(firebasedata1,"/wifi/value","null"))
       {
          
         Firebase.setString(firebaseData,"/users","null");
       

       }

     }
   

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  /* Select one of the cards */

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("\n");
  Serial.println("**Card Detected**");

  /* Print UID of the Card */

  Serial.print(F("Card UID:"));

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i]);
  }

  

  char add[25];        // Name key Array for Firebase
  char timeadd[25];    // Time Key Array for Firebase

  sprintf(add, "%s%d%s", "/users/", count , "/name");
  sprintf(timeadd, "%s%d%s", "/users/" , count , "/time");

  count++;



    timeClient.update();

    String time = timeClient.getFormattedTime();  // getting current time from server

    char time12[8];   //Array That Store Current time Value

    time.toCharArray(time12,8,0);   


 

  if ((mfrc522.uid.uidByte[0] == 195) && (mfrc522.uid.uidByte[1] == 27) && (mfrc522.uid.uidByte[2] == 116) && (mfrc522.uid.uidByte[3] == 17))
  {

    // status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

    /* Reading data from the Block */

    // status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);

    /* if (status != MFRC522::STATUS_OK)
     {
       Serial.print("Reading failed: ");
       Serial.println(mfrc522.GetStatusCodeName(status));
       return;
     }
     else
     {
       Serial.println("");
       Serial.println("Block was read successfully");
     }

        Serial.print("Data in Block:");
          Serial.print(blockNum);
          Serial.print(" --> ");

          char data[15];

          for (int j=0 ; j<14 ; j++)
          {
             Serial.write(readBlockData[j]);
             data[j] = readBlockData[j];

          }

            Serial.println(data[3]);*/

    

    if (Firebase.setString(firebaseData, add, "Himanshu_Mehra"))
    {

      if (Firebase.setString(firebaseData, timeadd, time12))
      {
        Serial.println("");
        Serial.println("Data Transfered");
      }
    }

    Serial.print("\n");

    servo.write(150);

    delay(8000);

    servo.write(0);

    delay(1000);

  }

  else
  {

    Serial.println("Unregistered user");

    if (Firebase.setString(firebaseData, add, "Sudrshan_Mehra"))
    {

      if (Firebase.setString(firebaseData, timeadd, time12))
      {

        
        Serial.println("Data Transfered");
      }
    }
  }
   
 
  mfrc522.PICC_HaltA();

 



}
