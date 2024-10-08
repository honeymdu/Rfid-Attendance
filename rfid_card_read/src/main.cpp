#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SS_PIN = D4;     

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;        

int blockNum = 2;  

byte bufferLen = 18;

byte readBlockData[18];

MFRC522::StatusCode status;



void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);

  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
  
}


void setup() 
{
  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  Serial.println("Scan a MIFARE 1K Tag to read data...");
}

void loop()
{
 
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) 
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
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
 
 
   Serial.print("\n");

   Serial.println("Reading from Data Block...");
   ReadDataFromBlock(blockNum, readBlockData);
  
   Serial.print("\n");
   Serial.print("Data in Block:");
   Serial.print(blockNum);
   Serial.print(" --> ");
   
   for (int j=0 ; j<14 ; j++)
   {
     Serial.write(readBlockData[j]);
   }
   Serial.print("\n");

   mfrc522.PICC_HaltA();

}



