//Error codes
#define SUCCESS 0x53
#define NODATA 0xEE

//Define the SPI Pin Numbers
#define DATAOUT 11//MOSI
#define DATAIN  12//MISO 
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

//Define the variables we'll need later in the program
char colorBuffer [64]; 

void setup() 
{  
  //SPI Bus setup
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	//Enable SPI HW, Master Mode, divide clock by 16    //SPI Bus setup
  
  //Set the pin modes for the RGB matrix
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);
  
  //Make sure the RGB matrix is deactivated
  //digitalWrite(SLAVESELECT,HIGH);
  digitalWrite(SLAVESELECT, LOW);
 
  //Start serial connection at 115200 baud
  Serial.begin(115200);

  //Clear screen
  clearScreen();
} 
 
void loop() 
{
    if (Serial.available() >= 63)
    {
      for (int i=0; i < 64; i++)
      {
        int received = Serial.read();
        if (received == -1)
        {
          Serial.write(NODATA);
          received = 0;
        }
        colorBuffer[i] = received;
      }
      Serial.write(SUCCESS);
      sendBuffer();
    }
}

void clearScreen()
{
  for (int i=0; i < 64; i++)
    colorBuffer[i] = 0;
  sendBuffer();
}

void sendBuffer()
{
  //Activate the RGB Matrix
  //digitalWrite(SLAVESELECT, LOW);
  //Send the color buffer to the RGB Matrix
  for(int LED=0; LED<64; LED++){
    //row * 8 + remainder
    //spiTransfer(colorBuffer[(floor((64-LED)/8)*8) + (LED % 8)]);
    spiTransfer(colorBuffer[LED]);
  }
  //Deactivate the RGB matrix.
  //digitalWrite(SLAVESELECT, HIGH); 
}


//Use this command to send a single color value to the RGB matrix.
//NOTE: You must send 64 color values to the RGB matrix before it displays an image!
char spiTransfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait for the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}
