/*
   Copyright 2015-2016 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/  

/****
 *  AllThingsTalk Developer Cloud IoT experiment for LoRa
 *  Version 1.0 dd 09/11/2015
 *  Original author: Jan Bogaerts 2015
 *
 *  This sketch is part of the AllThingsTalk LoRa rapid development kit
 *  -> http://www.allthingstalk.com/lora-rapid-development-kit
 *
 *  This example sketch is based on the Proxilmus IoT network in Belgium
 *  The sketch and libs included support the
 *  - MicroChip RN2483 LoRa module
 *  - Embit LoRa modem EMB-LR1272
 *  
 *  For more information, please check our documentation
 *  -> http://docs.smartliving.io/kits/lora
 *  
 **/

#include <Wire.h>
#include <ATT_LoRa_IOT.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>

#define SERIAL_BAUD 57600

#define AnalogSensor A4


MicrochipLoRaModem Modem(&Serial1, &Serial);
ATTDevice Device(&Modem, &Serial);

void setup() 
{
  pinMode(AnalogSensor,INPUT);
  while((!Serial) && (millis()) < 2000){}						//wait until serial bus is available, so we get the correct logging on screen. If no serial, then blocks for 2 seconds before run
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(Modem.getDefaultBaudRate());					// init the baud rate of the serial connection so that it's ok for the modem
  Device.Connect(DEV_ADDR, APPSKEY, NWKSKEY);
  Serial.println("Ready to send data");
}

float value = 0;

void loop() 
{
  int sensorValue = analogRead(AnalogSensor); 
  float Rsensor= sensorValue * 3.3 / 1023;
  Rsensor = pow(10, Rsensor);
  Serial.println(Rsensor);
  if((int)value != Rsensor){			// we round the value so we don't send too many value changes
    value = Rsensor;
    SendValue();
  }
  delay(3000);
}

void SendValue()
{
  Serial.print("Sending value: ");
  Serial.println(value);
  Device.Send(value, LIGHT_SENSOR);
}


void serialEvent1()
{
  Device.Process();														// for future use of actuators
}


