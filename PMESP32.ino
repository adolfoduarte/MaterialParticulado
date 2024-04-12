//https://how2electronics.com/interfacing-pms5003-air-quality-sensor-arduino/

#include <WiFi.h>
const char * ssid = "ONU-FAMILIA LOPEZ VALERA ."; // Ingrese su nombre red wifi
const char * pass = "3184867589"; //Ingresa la contraseña de tu red
WiFiClient client;


#include "ThingSpeak.h"
unsigned long Channel_ID = 2500108; //Ingrese su numero de canal de ThingSpeak
const char * WriteAPIKey = "5QI931Q8SGD3QEUV"; //Ingrese su clave de API de escritura de canal


void setup() {
  // our debugging output
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial.println("ya estoy funcionando");
 
  WiFi.begin(ssid, pass); //Se inicia la conexión al Wifi
  //Minetras se conecta imprimirá ...
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
  //Ya que se estableció la conexión al Wifi se imprime conexión establecida
  Serial.println("Conexion establecida");
  ThingSpeak.begin(client); //Iniciar el servidor de ThingSpeak
    // sensor baud rate is 9600

}
 
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;
    
void loop() {
  if (readPMSdata(&Serial2)) {
    // reading data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    /*Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);*/
    Serial.println("---------------------------------------");


    ThingSpeak.setField(1, int(data.pm10_standard)); 
    ThingSpeak.setField(2, int(data.pm25_standard)); 
    ThingSpeak.setField(3, int(data.pm100_standard)); 

    //Transmitir los datos al sevidor ThingSpeak
    ThingSpeak.writeFields(Channel_ID, WriteAPIKey);
    Serial.println("Datos enviados a ThingSpeak");
    delay(58000);
  }
}
 
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}