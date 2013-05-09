/*
  pfodDevice command parser test for Arduino
 Parses commands of the form { cmd ... } 
 see www.pfod.com.au  for more details.
 
 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */

void setup() {
  parserSetup(); // initialize pfod Cmd parser
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("pfodCmdParser ready, enter messages of the form");
  Serial.println(" {cmd} ");
  Serial.println(" via the serial monitor (9600baud)");
}

void loop() // run over and over
{
  byte cmd = 0;
  byte in = 0;
  if (Serial.available()) {
    in = Serial.read();
    cmd = parse(in);
    if (cmd != 0) {
      Serial.print("Cmd:");
      Serial.write(cmd);
      Serial.println();
      // so clear it
      cmd = 0; // have processed this cmd now
      // so clear it and wait for next one
    } 
    // else keep looping
  }  
}

byte cmdByte;
byte parserByteCounter;
byte parserState;
static const byte pfodMaxMsgLen = 0xff; // == 255, if no closing } by now ignore msg 
static const byte pfodStartMsg = (byte)'{';
static const byte pfodEndMsg = (byte)'}';
static const byte pfodWaitingForStart = 0xff;
static const byte pfodInMsg = 0;

void parserSetup() {
  parserByteCounter = 0;
  cmdByte = 0; // not started yet
  parserState = pfodWaitingForStart;
  // switch to pfodInMsg when see {
}

//-------------------------------------
//PROCESS_RECEIVED_CHAR
// { char }
// save first char after {
// store in cmdByte 
//  
// before {  parserState == pfodWaitingForStart
// when { seen parserState == pfodInMsg
// after { first char ( != } ) is stored in cmdByte
// cmdByte returned when terminating } seen but
// not if no } seen for 254 bytes after starting { then
// ignore msg and start looking for { again
// 
//-------------------------------------
// return 0 if no cmd found yet
// else return cmd when see }
// or ignore if >254 bytes after {
//
// in is byte read from Serial port
byte parse(byte in) {
  parserByteCounter++;
  if (parserState == pfodWaitingForStart) {
    if (in == pfodStartMsg) { // found {
       parserSetup(); // clean out last cmd
       parserState = pfodInMsg;
    } // else ignore this char as waiting for start {
    // always reset counter if waiting for {
    parserByteCounter = 1;
    return 0;
  }    
  
  // else have seen { 
  if ((parserByteCounter == pfodMaxMsgLen) &&
        (in != pfodEndMsg)) {
    // ignore this msg and reset
    // should not happen as pfodApp should limit
    // msgs sent to pfodDevice to <=255 bytes
    parserSetup();
    return 0;
  }
  
  // else is this the end of the msg
  if (in == pfodEndMsg) {
    byte pfodMsgCmd = cmdByte;
    // this will return 0 when parsing {} msg
    // set up to wait for next {
    parserSetup();
    // return command byte found
    return pfodMsgCmd;
  // else normal byte
  } else if (cmdByte == 0) {
    // save first cmd byte (only the first)
    cmdByte = in;
  } 
  return 0;
 }


