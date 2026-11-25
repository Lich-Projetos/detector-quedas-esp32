/* RECONSTRUCAO DE TRABALHO - NAO E O SKETCH ORIGINAL VALIDADO NO WOKWI.
   Conferir parametros, topicos e conexoes antes de executar. */
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>
constexpr int SDA_PIN=21, SCL_PIN=22, BUZZER_PIN=25;
constexpr char WIFI_SSID[]="Wokwi-GUEST", WIFI_PASS[]="";
constexpr char MQTT_HOST[]="broker.emqx.io";
constexpr int MQTT_PORT=1883;
// Proposta de topico; confirme o prefixo exclusivo antes de atualizar os clientes.
constexpr char TOPIC_ALERT[]="mackenzie/iot/queda/esp32-mpu6050-a4-2026/alerta";
constexpr char TOPIC_STATUS[]="mackenzie/iot/queda/esp32-mpu6050-a4-2026/status";
constexpr float LOW_G=0.55f, IMPACT_G=2.20f;
constexpr uint32_t WINDOW_MS=5000;
Adafruit_MPU6050 mpu; WiFiClient net; PubSubClient mqtt(net);
enum State { MONITORING, LOW_ACCEL, IMPACT }; State state=MONITORING;
uint32_t sequenceStart=0, nextRead=0;
void connectWifi(){ if(WiFi.status()==WL_CONNECTED)return; WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID,WIFI_PASS,6); uint32_t t=millis(); while(WiFi.status()!=WL_CONNECTED && millis()-t<15000)delay(100); }
void connectMqtt(){ if(mqtt.connected()||WiFi.status()!=WL_CONNECTED)return; String id="fall-"+String((uint32_t)ESP.getEfuseMac(),HEX); mqtt.connect(id.c_str()); }
void setup(){ Serial.begin(115200); pinMode(BUZZER_PIN,OUTPUT); Wire.begin(SDA_PIN,SCL_PIN); if(!mpu.begin(0x68,&Wire)){Serial.println("MPU6050 nao encontrado");while(true)delay(1000);} mqtt.setServer(MQTT_HOST,MQTT_PORT); connectWifi(); }
void loop(){ connectWifi();connectMqtt();mqtt.loop();if(millis()<nextRead)return;nextRead=millis()+50;
 sensors_event_t a,g,t;mpu.getEvent(&a,&g,&t);float x=a.acceleration.x/9.80665f,y=a.acceleration.y/9.80665f,z=a.acceleration.z/9.80665f;float accel=sqrtf(x*x+y*y+z*z);
 bool rest=accel>=0.75f&&accel<=1.25f;
 switch(state){case MONITORING:if(accel<LOW_G){state=LOW_ACCEL;sequenceStart=millis();}break;
 case LOW_ACCEL:if(millis()-sequenceStart>WINDOW_MS)state=MONITORING;else if(accel>IMPACT_G)state=IMPACT;break;
 case IMPACT:if(millis()-sequenceStart>WINDOW_MS)state=MONITORING;else if(rest){digitalWrite(BUZZER_PIN,HIGH);char msg[100];snprintf(msg,sizeof(msg),"{\"evento\":\"possivel_queda\",\"impacto_g\":%.2f,\"tempo_ms\":%lu}",accel,(unsigned long)millis());if(mqtt.connected())mqtt.publish(TOPIC_ALERT,msg);delay(400);digitalWrite(BUZZER_PIN,LOW);state=MONITORING;}break;}
 if(mqtt.connected()){char msg[100];snprintf(msg,sizeof(msg),"{\"estado\":\"monitorando\",\"aceleracao_g\":%.2f,\"tempo_ms\":%lu}",accel,(unsigned long)millis());mqtt.publish(TOPIC_STATUS,msg);}}

