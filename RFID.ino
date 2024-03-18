#include <MFRC522.h>
#include <SPI.h>

#define SAD 10
#define RST 5
MFRC522 nfc(SAD, RST);

#define ledPinOpen 2//Điều khiển led tại chân 2 sáng khi thẻ từ đưa vào được nhận diện
#define ledPinClosed 3//Điều khiển led tại chân 3 sáng khi thẻ từ đưa vào chưa được nhận điện
#define ledPinWrong 4//Điều khiển led tại chân 4 sáng khi thẻ từ đưa vào sai nhận diện

void setup() {
pinMode(ledPinOpen , OUTPUT); 
pinMode(ledPinClosed, OUTPUT); 
pinMode(ledPinWrong , OUTPUT);
SPI.begin();
Serial.begin(9600); //baurate là 115200 hoặc 9600
Serial.println("Dang tim chip MFRC522.");
nfc.begin();
byte version = nfc.getFirmwareVersion();
if (! version) {
Serial.print("Khong thay chip MFRC522.");
while(1); //halt
}
Serial.print("Da thay chip MFRC522 ");
Serial.print("Firmware phien ban. 0x");
Serial.print(version, HEX);
Serial.println(".");
}
#define AUTHORIZED_COUNT 2 /*Xác định tổng số lượng thẻ mà ta đã lập trình nhận diện, nếu ta có 2 thì nhập 2, */
byte Authorized[AUTHORIZED_COUNT][6] = {
{0x5, 0xF1, 0x64, 0x76, 0xFF, 0xFF}//Mã thẻ đã lập trình để nhận diện
,{0x1A, 0xD2, 0x46, 0xD6, 0xFF, 0xFF} 
};
void printSerial(byte *serial);
boolean isSame(byte *key, byte *serial);
boolean isAuthorized(byte *serial);

void loop() {
byte status;
byte data[MAX_LEN];
byte serial[5];
boolean Opening = false;
digitalWrite(ledPinOpen, Opening);
digitalWrite(ledPinClosed, !Opening);
digitalWrite(ledPinWrong, Opening);
status = nfc.requestTag(MF1_REQIDL, data);

if (status == MI_OK) {
status = nfc.antiCollision(data);
memcpy(serial, data, 5);

if(isAuthorized(serial))
{ 
printSerial(serial);
Serial.println("Ma the dung");
Opening = true;
}
else
{ 
printSerial(serial);
Serial.println("Ma the KHONG dung");
Opening = false;
}
nfc.haltTag();
digitalWrite(ledPinOpen, Opening);
digitalWrite(ledPinClosed, !Opening);
digitalWrite(ledPinWrong, !Opening);
delay(6000);
nfc.haltTag();

}//if (status == MI_OK)

delay(500);

}//void loop()

boolean isSame(byte *key, byte *serial)
{
for (int i = 0; i < 4; i++) {
if (key[i] != serial[i])
{ 
return false; 
}
}

return true;

}

boolean isAuthorized(byte *serial)
{
for(int i = 0; i<AUTHORIZED_COUNT; i++)//vòng lập tương ứng với số thẻ.
{
if(isSame(serial, Authorized[i]))
return true;
}
return false;
}
//
void printSerial(byte *serial)
{
Serial.print("The:");
for (int i = 0; i < 4; i++) {
Serial.print(serial[i], HEX);
Serial.print(" ");
} 
}
