#define SERIAL1_RXPIN 12 
#define SERIAL1_TXPIN 13
HardwareSerial Serial1(1);
#define AS5162KEY 0b0101000101100010
#define FuseRegister 0x41
#define Pass2Func 0x60 

#define NUM_DATA_BYTES 16


typedef union  _EncoderPacket{
    uint16_t stream[NUM_DATA_BYTES/2];
    uint8_t bytes[NUM_DATA_BYTES];
    struct
    {
      unsigned DATA15:8;//reserved
      unsigned DATA14:8;//reserved
      unsigned DATA13:2;//reserved
      unsigned CUSTID :7;
      unsigned CLH :9;
      unsigned CLL :9;
      unsigned OFFSET :20;
      unsigned GAIN :17;
      unsigned BP :14;
      unsigned ANGLERNG :1;
      unsigned DIAG_HIGH :1;
      unsigned QUADEN :2;
      unsigned AIRGAPSEL :1;
      unsigned HYSTSEL :2;
      unsigned DATA1:5;
      unsigned RED_ADD:4;
      unsigned RED_BIT:3;
      unsigned CUST_LOCK:1;
    } use;
}EncoderPacket;

EncoderPacket packet;

void setup() {
  Serial1.begin(9600, SERIAL_8E1, SERIAL1_RXPIN, SERIAL1_TXPIN);
   
}

void send(bool rw ,uint8_t address,uint16_t data){
  Serial1.write( (address&0x7f)+(rw?(1<<7):0));// address
  delay(1);
  Serial1.write( data & 0xFF);// data two
  Serial1.write( (data>>8) & 0xFF);// data one

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial1.write( 0x55);// synchronization frame
  delay(1);
  send(true,Pass2Func,AS5162KEY);
  for(int i=0;i<NUM_DATA_BYTES/2;i++){
    packet.stream[i]=0;
  }
  
  delay(100);
  
}
