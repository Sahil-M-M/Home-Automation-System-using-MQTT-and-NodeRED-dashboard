 /*******************************************************************************
  *
  *  @file      Home_Automation_System_with_MQTT_using_Node-RED_dashboard.ino
  *  @author    Sahil Mahendra Mangaonkar
  *  @date      24.09.2021
  *  @copyright © Sahil Mahendra Mangaonkar, 2021. All right reserved.
  *
  ******************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-FI User name & Password
#define wifi_ssid "Galaxy M51B56A"
#define wifi_password "Home@123"

// MQTT server info
#define mqtt_server "test.uniconvergetech.in"
#define mqtt_user "guest"      
#define mqtt_password "guest"  

// Pin definations
#define BedroomLightPin D2
#define BedroomFanPin D5    
#define KitchenLightPin D6
#define KitchenFanPin D7    
#define BathroomLightPin D8  

// MQTT Topic definations
#define BedroomLightTopic "bedroom/light"
#define BedroomFanTopic "bedroom/fan"   
#define KitchenLightTopic "kitchen/light"
#define KitchenFanTopic "kitchen/fan"    
#define BathroomLightTopic "bathroom/light"  

//Buffer to decode MQTT messages
char message_buff[100];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);   
  //Defining all pins as output 
  pinMode(BedroomLightPin, OUTPUT);
  pinMode(BedroomFanPin, OUTPUT);
  pinMode(KitchenLightPin, OUTPUT);
  pinMode(KitchenFanPin, OUTPUT);
  pinMode(BathroomLightPin, OUTPUT);
 
  wifi_setup();                           //Connect to Wifi network
  client.setServer(mqtt_server, 1883);    //Configure MQTT connexion
  client.setCallback(callback);           //callback function to execute when a MQTT message   
}

//Function to setup wifi connection
void wifi_setup() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi OK ");
  Serial.print("=> ESP8266 IP address: ");
  Serial.println(WiFi.localIP());
}

//Function to reconnect to MQTT broker
void reconnect() {

  while (!client.connected()) {                    //Try until connected to MQTT broker

    Serial.print("Connecting to MQTT broker ...");

    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {      //Try to connect to MQTT broker
      Serial.println("OK");
      client.subscribe(BedroomLightTopic);          //subscribe topic "bedroom/light"
      client.subscribe(BedroomFanTopic);            //subscribe topic "bedroom/fan"
      client.subscribe(KitchenLightTopic);          //subscribe topic "kitchen/light"
      client.subscribe(KitchenFanTopic);            //subscribe topic "kitchen/fan"
      client.subscribe(BathroomLightTopic);         //subscribe topic "bathroom/light"
    } 

    else {
      Serial.print("error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }

  }

}

void loop() {
  if (!client.connected()) {          // Reconnect to MQTT server if disconnected
    reconnect();                      
  }
  client.loop();
}

//MQTT callback function
void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    // Store message into a temporary buffer
    for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }

    Serial.println();

    String topic_received=String(topic);                  //Convert topic into string

    if(topic_received == BedroomLightTopic){              //Check if BedroomLightTopic state has changed
        Serial.print("Changing bedroom light to ");       
        if(messageTemp == "ON"){                          //Check if bedroom light was turned "ON" from the dashboard
          digitalWrite(BedroomLightPin, HIGH);            //Turn on bedroom light
          Serial.println("On");
        }
        else if(messageTemp == "OFF"){                    //Check if bedroom light was turned "OFF" from the dashboard
          digitalWrite(BedroomLightPin, LOW);             //Turn off bedroom light
          Serial.println("Off");
        }
    }

    else if(topic_received=="BedroomFanTopic"){           //Check if BedroomFanTopic state has changed
        Serial.print("Changing bedroom fan to ");       
        if(messageTemp == "ON"){                          //Check if bedroom fan was turned "ON" from the dashboard
          digitalWrite(BedroomFanPin, HIGH);              //Turn on bedroom fan
          Serial.print("On");
        }
        else if(messageTemp == "OFF"){                    //Check if bedroom fan was turned "OFF" from the dashboard
          digitalWrite(BedroomFanPin, LOW);               //Turn off bedroom fan
          Serial.print("Off");
        }
    }

    else if(topic_received == KitchenLightTopic){         //Check if KitchenLightTopic state has changed
        Serial.print("Changing bedroom fan to ");         //Check if kitchen light was turned "ON" from the dashboard
        if(messageTemp == "ON"){
          digitalWrite(KitchenLightPin, HIGH);            //Turn on kitchen light
          Serial.print("On");
        }
        else if(messageTemp == "OFF"){                    //Check if kitchen light was turned "OFF" from the dashboard
          digitalWrite(KitchenLightPin, LOW);             //Turn off kitchen light
          Serial.print("Off");
        }
    }
    else if(topic_received == KitchenFanTopic){           //Check if KitchenFanTopic state has changed
        Serial.print("Changing bedroom fan to ");
        if(messageTemp == "ON"){                          //Check if kitchen fan was turned "ON" from the dashboard
          digitalWrite(KitchenFanPin, HIGH);              //Turn on kitchen fan
          Serial.print("On");
        }
        else if(messageTemp == "OFF"){                    //Check if kitchen fan was turned "OFF" from the dashboard
          digitalWrite(KitchenFanPin, LOW);               //Turn off kitchen fan
          Serial.print("Off");
        }
    }

    else if(topic_received == BathroomLightTopic){        //Check if BathroomLightTopic state has changed
        Serial.print("Changing bedroom fan to ");
        if(messageTemp == "ON"){                          //Check if bathroom light was turned "ON" from the dashboard
          digitalWrite(BathroomLightPin, HIGH);           //Turn on bathroom light
          Serial.print("On");
        }
        else if(messageTemp == "OFF"){                    //Check if bathroom light was turned "OFF" from the dashboard
          digitalWrite(BathroomLightPin, LOW);            //Turn off bathroom light
          Serial.print("Off");
        }
    }

    Serial.println();
}
