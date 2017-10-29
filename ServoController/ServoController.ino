#include <CurieBLE.h>
#include <Servo.h>

Servo myServo;
 
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService servoService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Servo Service
 
// BLE Servo Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
 
void setup() {
  Serial.begin(9600);

  myServo.attach(9);
 
  // set advertised local name and service UUID:
  blePeripheral.setAdvertisedServiceUuid(servoService.uuid());
 
  // add service and characteristic:
  blePeripheral.addAttribute(servoService);
  blePeripheral.addAttribute(switchCharacteristic);
 
  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);
 
  // begin advertising BLE service:
  blePeripheral.begin();

  myServo.write(0);
}
 
void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();
 
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
 
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the Servo:
      if (switchCharacteristic.written()) {
        //ココらへんを編集
        if (switchCharacteristic.value() == 48) { //iPhone側は"0"を送信している
          myServo.write(90);
          delay(1000);
        } 
        else if(switchCharacteristic.value() == 49) {//iPhone側は"1"を送信している
          myServo.write(0);
          delay(1000);
        }
      }
    }
 
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
