#include <VirtualWire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 7
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
bool n;
// перед заливкой отключить моргание 13 пина в
byte addr[8]={0x28,0xFF,0xF7,0xDC,0x81,0x16,0x03,0xC1};
bool addrOk = false;
void setup() 
{
  Serial.begin(9600);  
  startradio();
  proverkaserinika();
     
}
void loop()
{
  vklviklten(-5,-1,1000);//t - min, t-max, задержка
}
void vklviklten(float tmin, float tmax, int pause)
{
  Serial.println(gradusnik()); 
  if (gradusnik()<=tmin) digitalWrite(8, HIGH);
  else if(gradusnik() > tmax) digitalWrite(8, LOW);
  delay(pause);
}

float gradusnik()
{  
  float c = sensors.getTempC(tempDeviceAddress);
  sensors.requestTemperatures();
  return c;
}
void proverkaserinika()
{
    if (serialvalid())
    {
      sensors.requestTemperatures();
      sendsms(50,70);// количество смс, задержка перед отправкой
    }
    
  else {
         while(true)delay(100000);
       }
}

void startradio()
{
pinMode(13,OUTPUT);
vw_set_ptt_inverted(true);
vw_set_tx_pin(6);
vw_setup(2000);// скорость передачи данных в Kbps  
}

bool serialvalid()
{
sensors.begin();
if(!sensors.getAddress(tempDeviceAddress, 0)) return false;   
bool valid = true;
for (uint8_t i = 0; i < 8; i++)
  { 
    valid = valid && (addr[i] == tempDeviceAddress[i]);
  }  
  return valid;
} 
 
void sendsms(int kolichestvo, int pause)
{
  for (int i = 0; i <kolichestvo; i++)
  {
  n=!n;
  delay(pause);
  digitalWrite(13,n);
  char *controller = "28FFF7DC811603C1";
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // ждем, пока отправится все сообщение
  }
/*delay(2000);
controller="0" ;
vw_send((uint8_t *)controller, strlen(controller));
vw_wait_tx(); // ждем, пока отправится все сообщение
digitalWrite(13,0);
//delay(2000);*/
}

