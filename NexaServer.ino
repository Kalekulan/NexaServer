#include <NexaTransmitter.h>
#include <stdlib.h>

//#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
/*

Sender:
14424562
110111000001100111110010

14424562,1,2,3
14424562,0,1,0
0123456789101112
--------------------------------

Byte0: 11110010
Byte1: 00011001
Byte2: 11011100

*/
const int txPin = 2;

const int SENDER = 1;
//const int UNIT = 2;
const int STATE = 2;
const int DIMLEVEL = 3;

const unsigned int MAXLENGTH = 20;


//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity

//NexaTransmitter remote(txPin,  14424562); // Create the nexa remote on pin12 with remote id
// 63161283 works :D:D


void setup() {
	Serial.begin(9600);
	Serial.println("Start of NEXA Transmitter");
	Serial.println("");
	Serial.println("");
	Serial.println("");
 	//lcd.begin(16,2); 
}



void ReadData(const byte incomingByte) {
	static char buf [MAXLENGTH];
	static unsigned int inputPos = 0;
	
	switch (incomingByte) {

		case '\n':   // end of text
			buf [inputPos] = 0;  // terminating null byte
			Serial.print("buf = ");
			Serial.println(buf);
			  
			// terminator reached! process input_line here ...
			ProcessData(buf, inputPos);
			  
			// reset buffer for next time
			inputPos = 0;  
			break;

		case '\r':   // discard carriage return
			break;

		default:
		  // keep adding if not full ... allow for terminating null byte
			if (inputPos < (MAXLENGTH - 1)) buf [inputPos++] = incomingByte;
			break;

    }  // end of switch

}


void ProcessData(const char* data, int length) {
	String senderTemp = "";
	uint32_t sender = 0;  
	int	state = 0;
	int	firstBreak = 0;
	int	secondBreak = 0;
	int thirdBreak = 0;
	int	dimLevel = 0;
	String dimTemp = "";
	int unit = 0;
	int separatorCounter = 0;

	int i = 0;

	//Serial.print("data: ");
	//Serial.println(data);
	
	//while(i < strlen(data)) {
	while(i < length) {
	//for(i = 0; i < strlen(data); i++) {
	
		if(data[i] == ',') {
			separatorCounter++;
			switch (separatorCounter) {

				case SENDER:
					for(int x = 0; x < i; x++) senderTemp += data[x];
					sender = senderTemp.toInt();
					Serial.print("sender = ");
					Serial.println(sender);

					unit = data[i+1] - '0';
					Serial.print("unit = ");
					Serial.println(unit);

					break;

				case STATE:
					state = data[i+1] - '0';
					Serial.print("state = ");
					Serial.println(state);

					break;

				case DIMLEVEL:
					if(i == strlen(data)-2) dimLevel = data[i+1] - '0';
					else if(i == strlen(data)-3) {
						dimTemp += data[i+1];
						dimTemp += data[i+2];
						dimLevel = dimTemp.toInt();
					}

					Serial.print("dimLevel = ");
					Serial.println(dimLevel);

					if(dimLevel > 15) Serial.println("dimLevel > 15 not allowed. Set to 15 instead.");

					break;

			}
			i++;
		}
		else i++;


	}

	//if(senderTemp.length() > 0 && dimTemp.length() > 0) {

	
	NexaTransmitter remote(txPin, sender);
		

	for(int i = 0; i < 5; i++) remote.setSwitch(state, unit, dimLevel); // switch on/down the unit 0 to dim level 0 (out of 15)
	Serial.println("Message transmitted.");
	Serial.println("");
	//for(int i = 0; i < 5; i++) 
	//remote.setSwitch(state, unit, dimLevel); // switch on/down the unit 0 to dim level 0 (out of 15)

	//remote.setSwitch(1, 0, dimLevel);
	//remote.setSwitch(1, 1, dimLevel);
	//for(int i = 0; i < 5; i++) remote.setSwitch(true, 0); // switch on/down the unit 0 to dim level 0 (out of 15)


}

void loop() {

	//int i = 0;
	//int incomingByte = 0; 

	//int numBytes;

	//byte incomingByte[24];
	//char buf[16];
	//NexaTransmitter remote(2, 14424562);
	while (Serial.available() > 0) ReadData(Serial.read());
	//for(int i = 0; i < 5; i++) remote.setSwitch(true, 0);
	//Serial.println("sent");
	//delay(1000);
	//memset

}





