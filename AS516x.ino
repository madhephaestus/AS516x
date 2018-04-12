#define SERIAL1_RXPIN 12 
#define SERIAL1_TXPIN 13
HardwareSerial Serial1(1);
#define AS5162KEY 0b0101000101100010
#define FuseRegister 0x41
#define Pass2Func 0x60 

#define NUM_DATA_BYTES 16

uint8_t stream[NUM_DATA_BYTES];

#define  CUSTID 0
#define  CLH 0
#define  CLL 0
#define  OFFSET 0
#define  GAIN 0
#define  BP 0
#define  ANGLERNG 0
#define  DIAG_HIGH 0
#define  QUADEN 0b01
#define  AIRGAPSEL 0
#define  HYSTSEL 0

#define  RED_ADD 0
#define  RED_BIT 0
#define  CUST_LOCK 1


void setup() {
  Serial1.begin(9600, SERIAL_8E1, SERIAL1_RXPIN, SERIAL1_TXPIN);
  Serial.begin(115200);

}

void send(bool rw ,uint8_t address,uint8_t data1,uint8_t data2){
  Serial1.write( (address&0x7f)+(rw?(1<<7):0));// address
  if(!rw){
    delay(10);
    return;
  }
  Serial1.write( data1);// data two
  Serial1.write( data2);// data one
  Serial.println("Address "+String(address)+" data1 "+String(data1)+" data2 "+String(data2));
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial1.write( 0x55);// synchronization frame
  delay(10);
  for(int i=0;i<NUM_DATA_BYTES/2;i++){
    stream[i]=0;
  }
  
  stream[0]  =CUST_LOCK<<7 | RED_BIT <<4 |RED_ADD;
  stream[1]  = HYSTSEL<<1 | AIRGAPSEL;
  stream[2]  = QUADEN<<6|DIAG_HIGH<<5|ANGLERNG<<4|((BP & (0b1111<<10))>>10);
  stream[3]  = ((BP & (0x00ff<<2))>>2);
  stream[4]  =((BP & 0b11)<<6) | ((GAIN & (0b0000000000111111<<11))>>11);
  stream[5]  = ((GAIN & (0x00ff<<3))>>3);
  stream[6]  =((GAIN & 0b111)<<5)  | ((OFFSET & (0b11111<<15))>>15);
  stream[7]  = ((OFFSET & (0x00ff<<7))>>7);
  stream[8]  = ((OFFSET<<1)&0xFF) |CLL>>8;
  stream[9]  = ((CLL)&0xFF) ;
  stream[10]  = (CLH)>>1;
  stream[11]  = (CLH & 0b1)<<7;
  stream[13]  =CUSTID<<2;

  for(int i=0;i<NUM_DATA_BYTES;i+=2){
    send(true,i,stream[i],stream[i+1]);
  }
  delay(10);

  for(int i=0;i<NUM_DATA_BYTES;i+=2){
    send(false,i,stream[i],stream[i+1]);
  }
  send(true,Pass2Func,AS5162KEY& 0xFF,(AS5162KEY>>8) & 0xFF);
 Serial.println("\r\n\r\nSession ");

  delay(100);
  
}
