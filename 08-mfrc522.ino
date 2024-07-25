/*
  The code is designed for an Arduino system to interface with an RFID-RC522 Module 
  using the "MFRC522" library. It initializes the RFID module and continuously scans for 
  RFID cards. When a card is detected, the code prints the card type and its 4-byte 
  serial number to the serial monitor.
  
  Board: Arduino Uno R4
  Component: RFID-RC522 Module
*/

#include <SPI.h>         // Include SPI library
#include <MFRC522.h>     // Include MFRC522 library

#define SS_PIN 10        // Slave Select pin
#define RST_PIN 9        // Reset pin

MFRC522 rfid(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

void setup() {
  Serial.begin(9600);   // Initialize serial communication at 9600 baud
  SPI.begin();          // Initialize SPI bus
  rfid.PCD_Init();      // Initialize the RFID module
  Serial.println("Scan an RFID card...");
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print the card type
  Serial.print("Card type: ");
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Print the card's UID
  Serial.print("Card UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  delay(500);  // Delay for 500 milliseconds
}
