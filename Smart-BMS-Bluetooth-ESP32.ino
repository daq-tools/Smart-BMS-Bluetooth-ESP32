/**
Program to read out and display 
data from xiaoxiang Smart BMS 
over Bluetooth Low Energy
https://www.lithiumbatterypcb.com/
Tested with original BLE module provided.
Might work with generic BLE module when UUIDs are modified

Needs ESP32 and graphic display.
Tested on TTGO TS https://github.com/LilyGO/TTGO-TS

(c) Miroslav Kolinsky 2019
https://www.kolins.cz

thanks to Petr Jenik for big parts of code
thanks to Milan Petrzilka

known bugs:
-if BLE server is not available during startup, program hangs
-reconnection sort of works, sometimes ESP reboots
*/

#define TRACE
#include <Arduino.h>
#include "BLEDevice.h"
#include "mydatatypes.h"
#include <Wire.h>

HardwareSerial commSerial(0);

//---- global variables ----

static boolean doConnect = false;
static boolean BLE_client_connected = false;
static boolean doScan = false;

packBasicInfoStruct packBasicInfo; //here shall be the latest data got from BMS
packEepromStruct packEeprom;	   //here shall be the latest data got from BMS
packCellInfoStruct packCellInfo;   //here shall be the latest data got from BMS

const byte cBasicInfo3 = 3; //type of packet 3= basic info
const byte cCellInfo4 = 4;  //type of packet 4= individual cell info

unsigned long previousMillis = 0;
const long interval = 2000;

bool toggle = false;
bool newPacketReceived = false;

void setup()
{

	commSerial.begin(115200, SERIAL_8N1, 3, 1);
	delay(3000);
	commSerial.println("Starting ebike dashboard application...");
	bleStartup();
}
//---------------------main loop------------------
void loop()
{
	bleRequestData();
	//server.handleClient();
	if (newPacketReceived == true)
	{
		printBasicInfo();
		printCellInfo();
	}
}
//---------------------/ main loop------------------
