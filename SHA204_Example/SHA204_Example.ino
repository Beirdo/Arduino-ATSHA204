/*
Copyright 2013 Nusku Networks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <SHA204.h>
#include <SHA204SWI.h>
#include <SHA204Definitions.h>

const int sha204Pin = 10;

SHA204SWI sha204swi(sha204Pin);

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Sending a Wakup Command. Response should be:\r\n4 11 33 43:");
  Serial.println("Response is:");
  wakeupExample();
  Serial.println();
  Serial.println("Asking the SHA204's serial number. Response should be:");
  Serial.println("1 23 x x x x x x x EE");
  Serial.println("Response is:");
  serialNumberExample();
  Serial.println();
  Serial.println("Sending a MAC Challenge. Response should be:");
  Serial.println("23 6 67 0 4F 28 4D 6E 98 62 4 F4 60 A3 E8 75 8A 59 85 A6 79 96 C4 8A 88 46 43 4E B3 DB 58 A4 FB E5 73");
  Serial.println("Response is:");
  macChallengeExample();
}

void loop()
{
}

byte wakeupExample()
{
  uint8_t response[SHA204_RSP_SIZE_MIN];
  byte returnValue;
  
  returnValue = sha204swi.wakeup(&response[0]);
  for (int i=0; i<SHA204_RSP_SIZE_MIN; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  return returnValue;
}

byte serialNumberExample()
{
  uint8_t serialNumber[9];
  byte returnValue;
  
  returnValue = sha204swi.serialNumber(serialNumber);
  for (int i=0; i<9; i++)
  {
    Serial.print(serialNumber[i], HEX);
    Serial.print(" ");
  }
  Serial.println(); 
  
  return returnValue;
}

byte macChallengeExample()
{
  uint8_t command[MAC_COUNT_LONG];
  uint8_t response[MAC_RSP_SIZE];

  const uint8_t challenge[MAC_CHALLENGE_SIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
  };

  uint8_t ret_code = sha204swi.execute(SHA204_MAC, 0, 0, MAC_CHALLENGE_SIZE, 
    (uint8_t *) challenge, 0, NULL, 0, NULL, sizeof(command), &command[0], 
    sizeof(response), &response[0]);

  for (int i=0; i<SHA204_RSP_SIZE_MAX; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
  
  return ret_code;
}
