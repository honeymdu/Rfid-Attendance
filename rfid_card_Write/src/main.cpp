#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SS_PIN = D4;    

MFRC522 mfrc522(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;        


int blockNum = 2;  

byte blockData [16] = {"HIMANSHU MEHRA"};

MFRC522::StatusCode status;

void WriteDataToBlock(int blockNum, byte blockData[]) 
{
 
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written into Block successfully");
  }
  
}

void setup() 
{
 
  Serial.begin(9600);

  /* Initialize SPI bus */
  SPI.begin();

  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();

  Serial.println("Scan a MIFARE 1K Tag to write data...");
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

   Serial.print("\n");
   Serial.println("Writing to Data Block...");
   WriteDataToBlock(blockNum, blockData);

   mfrc522.PICC_HaltA();
   
}






