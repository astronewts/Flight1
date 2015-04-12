#include <IridiumSBD.h>
//#include <SoftwareSerial.h>

//SoftwareSerial nss(18, 19);

//  Creates an IridiumSBD library object
//IridiumSBD(Stream &stream, int sleepPinNo = -1);
// stream – The serial port that the RockBLOCK is connected to.
// sleepPin - The number of the Arduino pin connected to the RockBLOCK SLEEP line.
IridiumSBD isbd(Serial3, 50);

static const int ledPin = 13; 

void setup()
{
  

  
  int signalQuality = -1;
  
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  Serial3.begin(19200);

  isbd.attachConsole(Serial); // see http://arduiniana.org/libraries/iridiumsbd/ for details 
  isbd.attachDiags(Serial);   // see http://arduiniana.org/libraries/iridiumsbd/ for details 
    pinMode(ledPin, OUTPUT);
    
    isbd.setPowerProfile(1); // This is a low power application
    
  // begin =  Starts (or wakes) the RockBLOCK modem and prepare it to communicate.
  isbd.begin(); 

  
    isbd.useMSSTMWorkaround(false);  // see http://arduiniana.org/libraries/iridiumsbd/ for details 

//int getSignalQuality(int &quality);
//Description:   Queries the signal strength and visibility of satellites
//Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
//Parameter:      quality – Return value: the strength of the signal (0=nonexistent, 5=high)
  int err = isbd.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("Signal quality (0=nonexistent, 5=high) is");
  Serial.println(signalQuality);


////////////////// code to transform a string into a binary: ////////////////////////////
//String myText = "Hello World";
//
//for(int i=0; i<myText.length(); i++){
//
//   char myChar = myText.charAt(i);
//  
//    for(int i=7; i>=0; i--){
//      byte bytes = bitRead(myChar,i);
//      Serial.print(bytes, BIN);
//    }
////////////////// END code to transform a string into a binary ////////////////////////////


////////////////// send and recieve binary message ///////////////////////////////////////
//// ==================== define binary message ============================= // 
////  uint8_t tx_buffer[200] = 
////  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
//
//// let try that: 
//int i;
//char tmp[3];
//char buf[] = "truite";
//tmp[2] = '\0';
//uint8_t tx_buffer[20];
//uint8_t len_buffer=0;
//
//for(i=0;i<strlen(buf);i+=2) {
//  tmp[0] = buf[i];
//  tmp[1] = buf[i+1];
//  tx_buffer[len_buffer] = strtol(tmp,NULL,16);
//  len_buffer++;}
//  
////// you can also do that:   
////// this version is actually not working yet, rrrrrrr!!!! 
////    // Define 
////String str = "This is my string"; 
////// Length (with one extra character for the null terminator)
////int str_len = str.length() + 1; 
////// Prepare the character array (the buffer) 
//////char tx_buffer[str_len];
////uint8_t tx_buffer[str_len];
////// Copy it over 
////str.toCharArray(tx_buffer, str_len);
////////str.StringToCharArray(tx_buffer, str_len);
////    Serial.print("printing message");
////    Serial.println(tx_buffer);
////    Serial.print("END printing message");
//// ==================== end define binary message ========================= // 
//// int sendReceiveSBDBinary(const uint8_t *txData, size_t txDataSize, uint8_t *rxBuffer, size_t &rxBufferSize);
////Description:   Transmits a binary message to the global satellite system and receives a message if one is available.
////Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
////Parameter:      txData – The buffer containing the binary data to be transmitted.
////Parameter:      txDataSize - The size of the outbound buffer in bytes.
////Parameter:      rxBuffer – The buffer to receive the inbound message.
////Parameter:      rxBufferSize - The size of the buffer in bytes.
//// NOTE: uint8_t is shorthand for: a type of unsigned integer of length 8 bits
//// NOTE: The maximum size of a transmitted packet (including header and checksum) is 340 bytes.
//// NOTE: The maximum size of a received packet is 270 bytes.
////=========== real command =========================================== //
//  size_t tx_bufferSize = sizeof(tx_buffer);
//  uint8_t rx_buffer[200];
//  size_t rx_bufferSize = sizeof(rx_buffer);
//  
////  err = isbd.sendReceiveSBDBinary(tx_buffer, 20, rx_buffer, bufferSize);
//  err = isbd.sendReceiveSBDBinary(tx_buffer, tx_bufferSize, rx_buffer, rx_bufferSize);
////=========== end real command ======================================= //


////////////// send and recieve text message ///////////////////////////////////////
//==================== define text message ============================= // 
// this define text mssg has not been tested!
String myText = "Hello World";
for(int i=0; i<myText.length(); i++)
  {
   char tx_buffer = myText.charAt(i);
  }
// ==================== end define binary message ========================= // 
//int sendReceiveSBDText(const char *message, uint8_t *rxBuffer, size_t &rxBufferSize);
//Description:   Transmits a text message to the global satellite system and receives a message if one is available.
//Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
//Parameter:      message – A 0-terminated string message.
//Parameter:      rxBuffer – The buffer to receive the inbound message.
//Parameter:      rxBufferSize - The size of the buffer in bytes.
//=========== real command =========================================== //
    uint8_t rx_buffer[200];
    size_t rx_bufferSize = sizeof(rx_buffer);
    err = isbd.sendReceiveSBDText(tx_buffer, rx_buffer, rx_bufferSize);
  // you can also do that:
//   err = isbd.sendSBDText("Hello, world!");
//=========== end real command ======================================= //


  Serial.print("message sent");
  if (err != 0)
  {
    Serial.print("sendReceiveSBDText failed: error ");
    Serial.println(err);
    return;
  }
//// ================ Print inbound message ================================= //
//  Serial.print("Inbound buffer size is ");
//  Serial.println(rx_bufferSize);
//  for (int i=0; i<rx_bufferSize; ++i)
//  {
//    Serial.write(rx_buffer[i]);
//    Serial.print("(");
//    Serial.print(rx_buffer[i], HEX);
//    Serial.print(") ");
//  }
//// ================ END Print inbound message ============================== //

  
  Serial.print("Number messages left: ");
//  int getWaitingMessageCount();
//  Description:   Returns the number of waiting messages on the Iridium servers.
//  Returns:            The number of messages waiting.
  Serial.println(isbd.getWaitingMessageCount());
}

     // from rockblock module :
    String combine(int bin_size, long input_data, String dataword)
  {
      int zeros;
      String temp_str;
      temp_str = String(input_data,BIN);
      zeros = bin_size - temp_str.length();
   
      for (int i=0; i<zeros; i++) {
        temp_str = "0"+temp_str;
      }
      
      dataword = dataword + temp_str;
      
      return dataword;     
  }
  // end from rockblock module :
  

void loop()
{
}

