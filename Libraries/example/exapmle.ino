#include <ads1262.h>
#include <SPI.h>

ads1262 ADS1292;

volatile char SPI_RX_Buff[150];
volatile char EEG_Ch2_Data[10];
volatile static int SPI_RX_Buff_Count = 0;
volatile char *SPI_RX_Buff_Ptr;
volatile int Responsebyte = false;
volatile unsigned int pckt =0 , buff=0,t=0 , j1=0,j2=0;
unsigned long ueegtemp = 0,Pkt_Counter=0;
signed long seegtemp=0;
volatile int i;
int count=0;


void setup() 
{
  // initalize the  data ready and chip select pins:
  pinMode(ADS1262_DRDY_PIN, INPUT);
  pinMode(ADS1262_CS_PIN, OUTPUT);
  pinMode(ADS1262_START_PIN, OUTPUT);
  pinMode(ADS1262_PWDN_PIN, OUTPUT);
  
  //initalize ADS1292 slave
  ADS1292.ads1262_Init();
  Serial.begin(9600);
 }

void loop() 
{
  volatile int i,data;

  if( !(millis()%1000) && count)
  {
  //  Serial.print("DRDY low: "); 
 // Serial.println(count); 
  count= 0;
  }
  
 if((digitalRead(ADS1262_DRDY_PIN)) == LOW)
  {  
    SPI_RX_Buff_Ptr = ADS1292.ads1262_Read_Data();
    Responsebyte = true ;
   // Serial.print("DRDY low: "); 
   count++;
  }

  if(Responsebyte == true)
  {
    /*Serial.print("start Time: ");
    time = millis();
    //prints time since program started
    Serial.println(time);
    */   
    for(i = 0; i <12; i++)
    {
      SPI_RX_Buff[SPI_RX_Buff_Count++] = *(SPI_RX_Buff_Ptr + i);
    }
    Responsebyte = false;
  }
  
  if(SPI_RX_Buff_Count >= 12)
  {     
    pckt = 0; j1=0;   j2=0;

    EEG_Ch2_Data[0]= (unsigned char)SPI_RX_Buff[0];
    EEG_Ch2_Data[1]= (unsigned char)SPI_RX_Buff[1];
    EEG_Ch2_Data[2]= (unsigned char)SPI_RX_Buff[2];
    EEG_Ch2_Data[3]= (unsigned char)SPI_RX_Buff[3];

   
    //seegtemp = -89485;
  
     ueegtemp = (unsigned long) ((EEG_Ch2_Data[3]<<24)|(EEG_Ch2_Data[2]<<16)|(EEG_Ch2_Data[1]<<8)|EEG_Ch2_Data[0]);
 //    ueegtemp = (unsigned long) (ueegtemp<<8);
     seegtemp = (signed long) (ueegtemp);
 //    seegtemp = (signed long) (seegtemp>>8); 

   Serial.println( seegtemp,8);
    }
    
  SPI_RX_Buff_Count = 0;

}