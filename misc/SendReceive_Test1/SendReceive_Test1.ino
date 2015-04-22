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

  Serial.print("Signal quality (0=nonexistent, 5=high) is ");
  Serial.println(signalQuality);



//////////////// send and recieve binary message ///////////////////////////////////////
// ==================== define binary message ============================= // 
//  uint8_t tx_buffer[200] = 
//  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

// process to communicate a string of 0 and 1:
//  for that we need to convert the object "String" into an array uint8_t
// 1) devide the string into chuncks of 8 characters (they will become the bits of the uint8_t)
// 2) convert each of the 8 character into an integer (0 or 1): i1 i2 i3 ... i8
// 3) build a unique uint8_t using: i7*2^7 + i6*2^6 + ... + i0*2^0
// 4) send data. 

//"01000110 10110001 010110";
String myText = "0100011010110001010110";

size_t size_mssg = myText.length(); 

    size_t tx_bufferSize  = size_mssg / 8;
    float tx_bufferSize_rest =  size_mssg % 8;
    if ( tx_bufferSize_rest != 0) { tx_bufferSize = tx_bufferSize + 1;
    } 
    Serial.println("numb of bytes in mssg:");
    Serial.println(tx_bufferSize);

uint8_t tx_buffer[tx_bufferSize];

/////////////// do chunks /////////////////////////////
for(int i=0; i<tx_bufferSize; i++){
    tx_buffer[i] = 0;
    for(int j=0; j<8; j++){
        int k = i*8 + j;
       char myChar = myText.charAt(k);
//       Serial.println("character:");
//       Serial.println(myChar);  
       //byte bit_extr = bitRead(myChar,0); // should be 0 or 1 
       int  bit_extr = max(myChar - '0', 0);
       // not working : nt  bit_extr = myChar.toInt();
       
//       Serial.println("bit:");
//       Serial.println(bit_extr);
       
// does not work       tx_buffer[i] = tx_buffer[i] + bit_extr * 2^(7-j);
       tx_buffer[i] = tx_buffer[i] + bit_extr * pow(2,7-j);
          
    }
    Serial.println(" ====================================== ");
    Serial.println("Dec number:");
    Serial.println(tx_buffer[i]);
    Serial.println(" ====================================== ");
  }
  /////////////// END do chunks /////////////////////////////


// gs tmp
//bool output[370 * 8] = {0};
bool output[3 * 8] = {0};
for (int i=0; i<sizeof(myText); i++){
  if (myText.charAt(i) == '0'){
    output[i] = 0;
  }else{
     output[i] = 1;
  }
}

uint8_t* output_as_int = (uint8_t*) output;


////All the integers indiv
//for (int i=0; i<3; i++){
//  uint8_t foo = output_as_int[i];
//  Serial.println("output_as_int= ");
//  Serial.println(output_as_int[i],DEC);
//  Serial.println(output_as_int[i],BIN);
//  //Serial.printf(F("value is:%3d\n"),output_as_int[i]);
//  Serial.println("foo= ");
//  Serial.println(foo);
//}
  

// ==================== end define binary message ========================= // 
// int sendReceiveSBDBinary(const uint8_t *txData, size_t txDataSize, uint8_t *rxBuffer, size_t &rxBufferSize);
//Description:   Transmits a binary message to the global satellite system and receives a message if one is available.
//Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
//Parameter:      txData – The buffer containing the binary data to be transmitted.
//Parameter:      txDataSize - The size of the outbound buffer in bytes.
//Parameter:      rxBuffer – The buffer to receive the inbound message.
//Parameter:      rxBufferSize - The size of the buffer in bytes.
// NOTE: uint8_t is shorthand for: a type of unsigned integer of length 8 bits
// NOTE: The maximum size of a transmitted packet (including header and checksum) is 340 bytes.
// NOTE: The maximum size of a received packet is 270 bytes.
//=========== real command =========================================== //
  uint8_t rx_buffer[400]; // max size of a message is 370 bytes so we are safe with 400. 
  size_t rx_bufferSize = sizeof(rx_buffer);
  
//  err = isbd.sendReceiveSBDBinary(tx_buffer, 20, rx_buffer, bufferSize);
  err = isbd.sendReceiveSBDBinary(tx_buffer, tx_bufferSize, rx_buffer, rx_bufferSize);
//  err = isbd.sendReceiveSBDBinary(output_as_int, tx_bufferSize, rx_buffer, rx_bufferSize);
//=========== end real command ======================================= //


//////////////// send and recieve text message ///////////////////////////////////////
////==================== define text message ============================= // 
//String myText = "saumon";
//    Serial.println("printing message to send:");
//    Serial.println(myText);
//    Serial.println("END printing message to send");
//
//size_t tx_bufferSize = myText.length();
//    Serial.println("number of characters in my mssg:");
//    Serial.println(tx_bufferSize);
//    Serial.println("");
//char tx_buffer[tx_bufferSize];
//
//for(int i=0; i<myText.length(); i++)
//  {
//    tx_buffer[i] = myText.charAt(i);
//  // char tx_buffer[i] = myText.charAt(i);
//     //  Serial.println(tx_buffer);
//  }
//  
//    Serial.println("printing message after convertion into char:");
//    Serial.println(tx_buffer);
//    Serial.println("");
//// ==================== end define binary message ========================= // 
////int sendReceiveSBDText(const char *message, uint8_t *rxBuffer, size_t &rxBufferSize);
////Description:   Transmits a text message to the global satellite system and receives a message if one is available.
////Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
////Parameter:      message – A 0-terminated string message.
////Parameter:      rxBuffer – The buffer to receive the inbound message.
////Parameter:      rxBufferSize - The size of the buffer in bytes.
//// NOTE: when the char="truite" is sent we read on the web -rockblock interface "747275697465" which is the hex for "truite"
//// NOTE: an object of type "String" is a string of characters with no specified length (dynamic allocation) whereas a 
////       type "char" is a string of characters but with a well defined length (specified at declaration). 
//// NOTE: the sendReceiveSBDText cannot deal with String but char which are well defined in size.
////=========== real command =========================================== //
//uint8_t rx_buffer[200];
//size_t rx_bufferSize = sizeof(rx_buffer);
//    err = isbd.sendReceiveSBDText(tx_buffer, rx_buffer, rx_bufferSize);
//    
//  // you can also do that, that we know works:
//   //err = isbd.sendSBDText("truite");
////=========== end real command ======================================= //


  Serial.print("message sent");
  if (err != 0)
  {
    Serial.print("sendReceiveSBDText failed: error ");
    Serial.println(err);
    return;
  }
// ================ Print inbound message ================================= //
  Serial.print("Inbound buffer size is ");
  Serial.println(rx_bufferSize);
  for (int i=0; i<rx_bufferSize; ++i)
  {
 //   Serial.write(rx_buffer[i]);
    Serial.print("(");
    //Serial.print(rx_buffer[i], HEX);
    Serial.print(rx_buffer[i]);
    Serial.print(") ");
  }
// ================ END Print inbound message ============================== //

  
  Serial.print("Number messages left: ");
//  int getWaitingMessageCount();
//  Description:   Returns the number of waiting messages on the Iridium servers.
//  Returns:            The number of messages waiting.
  Serial.println(isbd.getWaitingMessageCount());
}

//     // from rockblock module :
//    String combine(int bin_size, long input_data, String dataword)
//  {
//      int zeros;
//      String temp_str;
//      temp_str = String(input_data,BIN);
//      zeros = bin_size - temp_str.length();
//   
//      for (int i=0; i<zeros; i++) {
//        temp_str = "0"+temp_str;
//      }
//      
//      dataword = dataword + temp_str;
//      
//      return dataword;     
//  }
//  // end from rockblock module :
  

void loop()
{
}

