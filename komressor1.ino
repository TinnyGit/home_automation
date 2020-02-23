#include <elapsedMillis.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>

elapsedMillis timer;
bool timerFlag = false;
String duration;

#define RELAY_CH1 5
#define RELAY_CH2 6
#define RELAY_CH3 7
#define RELAY_CH4 8

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,1,178);
IPAddress server(192,168,1,106);

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String mess = String((char*)payload);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  mess = mess.substring(0,length);
  Serial.println(mess);
  controller(mess);
}
  
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","Redo");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(57600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);

  //initialize relays to low
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_CH1, OUTPUT);
  digitalWrite(RELAY_CH1, LOW);  // switch off Relay1
  pinMode(RELAY_CH2, OUTPUT);
  digitalWrite(RELAY_CH2, LOW);  // switch off Relay2
  pinMode(RELAY_CH3, OUTPUT);
  digitalWrite(RELAY_CH3, LOW);  // switch off Relay3
  pinMode(RELAY_CH4, OUTPUT);
  digitalWrite(RELAY_CH4, LOW);  // switch off Relay4
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(timerFlag){
    if(timer > duration.toInt()*60000){
      stop_komp(1);
      timerFlag = false;
    }
  }
}

void reset_timer(){
  timer = 0;
  }
  
void start_komp() {
  digitalWrite(RELAY_CH1, HIGH);
  digitalWrite(RELAY_CH2, HIGH);
  digitalWrite(RELAY_CH3, HIGH);
  digitalWrite(RELAY_CH4, HIGH);
  reset_timer();
  client.publish("outTopic","On");
  }

void stop_komp(int tajm) {
  if(timer > tajm){
    digitalWrite(RELAY_CH1, LOW);
    digitalWrite(RELAY_CH2, LOW);
    digitalWrite(RELAY_CH3, LOW);
    digitalWrite(RELAY_CH4, LOW);
    reset_timer();
    client.publish("outTopic","Off");
  }
}

void controller(String flag){

    int intFlag = flag.toInt();
    
    if(flag == "kompressor_on"){
      start_komp();
      }

    if(flag == "kompressor_off"){
      stop_komp(1);
      }
     
     if (intFlag > 0){
      start_komp();
      timerFlag = true;
      duration = flag;
      client.publish("outTopic","Timer set");
      }
  }
