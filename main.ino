#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <Servo.h>

Servo topLeft;
Servo topRight;
Servo bottomLeft;
Servo bottomRight;

// Experimentally defined servo positions
#define TOP_LEFT_OPEN 25
#define TOP_RIGHT_OPEN 133
#define TOP_LEFT_CLOSE 76
#define TOP_RIGHT_CLOSE 83
#define BOTTOM_LEFT_OPEN 80
#define BOTTOM_RIGHT_OPEN 80
#define BOTTOM_LEFT_CLOSE 180
#define BOTTOM_RIGHT_CLOSE 0

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);

uint8_t records[7]; // save record
uint8_t buf[64];

#define onRecord    (0)
#define offRecord   (1)
#define attackRecord (2)

void setup()
{
  /** initialize */
  myVR.begin(9600);
  Serial.begin(115200);

  topLeft.attach(5);
  topRight.attach(7);
  bottomLeft.attach(9);
  bottomRight.attach(11);
  myVR.clear();
  myVR.load((uint8_t)onRecord);
  myVR.load((uint8_t)offRecord);
  myVR.load((uint8_t)onRecord);
  myVR.load((uint8_t)attackRecord);

  openMask();
  closeMask();
}

void closeMask() {
  topLeft.write(TOP_LEFT_CLOSE);
  topRight.write(TOP_RIGHT_CLOSE);
  delay(100);
  bottomLeft.write(BOTTOM_LEFT_CLOSE);
  bottomRight.write(BOTTOM_RIGHT_CLOSE);
  delay(2000);
}

void slowCloseMask() {
  for (int leftPos = TOP_LEFT_OPEN; leftPos <= TOP_LEFT_CLOSE; leftPos++) {
    int rightPos = (TOP_LEFT_OPEN + TOP_RIGHT_OPEN) - leftPos;
    topLeft.write(leftPos);
    topRight.write(rightPos);
    delay(5);
  }
  delay(100);
  bottomLeft.write(BOTTOM_LEFT_CLOSE);
  bottomRight.write(BOTTOM_RIGHT_CLOSE);
  delay(2000);
}

void openMask() {
  bottomLeft.write(BOTTOM_LEFT_OPEN);
  bottomRight.write(BOTTOM_RIGHT_OPEN);
  delay(100);
  topLeft.write(TOP_LEFT_OPEN);
  topRight.write(TOP_RIGHT_OPEN);
  delay(2000);
}

void attack() {
  bottomLeft.write(BOTTOM_LEFT_OPEN);
  bottomRight.write(BOTTOM_RIGHT_OPEN);
  delay(100);
  topLeft.write(TOP_LEFT_OPEN);
  topRight.write(TOP_RIGHT_OPEN);
  delay(500);

  for (int i = 0; i < 5; i++) {
    topLeft.write(60);
    topRight.write(100);
    delay(200);
    topLeft.write(TOP_LEFT_OPEN);
    topRight.write(TOP_RIGHT_OPEN);
    delay(200);
  }
  closeMask();
}

void loop()
{
  // Serial.println("Begin voice recognition");
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case onRecord:
        // Open
        // Serial.println("Open");
        openMask();
        break;
      case offRecord:
        // Close
        // Serial.println("Close");
        closeMask();
        break;
      case attackRecord:
        attack();
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
  }
}
