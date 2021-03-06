
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "ssid";
const char *pass =  "pass";

WiFiClient wclient;

// mqtt server
IPAddress mqtt_server(192,168,0,20);
PubSubClient client(wclient, mqtt_server);

//	client name
String str_name="esp";

String str_topic = String("/" + str_name + "/out/rec");
String str_payload;

bool bt;
byte rec;
int count;
//char stream[24000];

void setup()
{
	Serial.begin(115200);
	Serial.println(0);			//start
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, pass);
	pinMode(0,INPUT_PULLUP);	//	button D3
	pinMode(16,OUTPUT);			//	led D0
}

void loop()
{
	my_connect();
	if(client.connected())
	{
		my_button();
		my_analog();
	}

}

void my_connect()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println(1);			//Connecting to
		WiFi.begin(ssid, pass);

		if(WiFi.waitForConnectResult() != WL_CONNECTED)
		{
			Serial.println(2);		//WiFi NOT connected
			if(digitalRead(10)==LOW)
				digitalWrite(10,HIGH);
			return;
		}
		Serial.println(3);			//WiFi connected
	}
	if(WiFi.status() == WL_CONNECTED)
	{
		if(!client.connected())
		{
			Serial.println(4);				//Connecting to MQTT server
			if(client.connect(str_name))
			{
				Serial.println(5);				//Connected to MQTT server
			}
			else
			{
				Serial.println(6);		//Could not connect to MQTT server
				if(digitalRead(10)==LOW)
					digitalWrite(10,HIGH);
			}
		}
		if(client.connected())
		{
			client.loop();
			if(digitalRead(10)==HIGH)
				digitalWrite(10,LOW);
		}
	}
}

void my_print()
{
	//Serial.println(millis());
	client.publish(str_topic,str_payload);
	str_payload="";
	//Serial.println(millis());
}

void my_button()
{
	if(digitalRead(12)==LOW && count<15)
		count++;
	if(digitalRead(12)==HIGH && count>0)
		count--;
	if(count>10 && bt==false)
		bt=true;
	if(count<5 && bt==true)
		bt=false;
}

void my_analog()
{
	if(bt==true)
	{
		//Serial.println(millis());
		my_record();
		//Serial.println(millis());
	}
}

void my_record()
{
	bt=false;
	for(int y=0;y<11;y++)
	{
		for(int i=0;i<1000;i++)
			str_payload += char(analogRead(A0)/4);
		my_print();
	}
}
