#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <PulseSensorPlayground.h>

// =========================
const char* ssid = "YOUR_WIFI_SSID"";
const char* password = "YOUR_WIFI_PASSWORD";

const char* websocket_server = "YOUR_SERVER_IP";  
const uint16_t websocket_port = YOUR_PORT;          
const char* websocket_path = "/ws";           
// =========================

const int PULSE_INPUT = 36;  // Pin de entrada de señal en ESP32 (VP)
const int THRESHOLD = 550;   // Umbral ajustable para la señal

PulseSensorPlayground pulseSensor; 
WebSocketsClient webSocket;         

unsigned long lastBeatTime = 0;
unsigned long ibi = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Conectado al WiFi!");

  
  webSocket.beginSSL(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);     
  webSocket.setReconnectInterval(5000); 

  
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setThreshold(THRESHOLD);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor listo.");
  } else {
    Serial.println("No se pudo iniciar PulseSensor.");
    while (1)
      ; 
  }
}

void loop() {
  webSocket.loop(); 

  int myBPM = pulseSensor.getBeatsPerMinute(); 
  int Signal = analogRead(PULSE_INPUT);         

  // Detectar inicio de latido
  if (pulseSensor.sawStartOfBeat()) {
    unsigned long now = millis();
    ibi = now - lastBeatTime; 
    lastBeatTime = now;

    
    String payload = "{";
    payload += "\"timestamp\": " + String(now) + ","; 
    payload += "\"bpm\": " + String(myBPM) + ",";      
    payload += "\"ibi\": " + String(ibi) + ",";        
    payload += "\"signal\": " + String(Signal);        
    payload += "}";

    Serial.println("➡️ Enviando datos WebSocket: " + payload);

    webSocket.sendTXT(payload);
  }

  delay(20);  
}


void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("❌ Desconectado del WebSocket!");
      break;
    case WStype_CONNECTED:{
      Serial.println("✅ Conectado al WebSocket!");

      // Opcional
      String subscribeMessage = "{";
      subscribeMessage += "\"type\": \"subscribe\",";
      subscribeMessage += "\"channel\": \"CANAL2\","; 
      subscribeMessage += "\"name\": \"ESP32-CLIENT1\""; 
      subscribeMessage += "}";

      Serial.println("➡️ Enviando suscripción WebSocket: " + subscribeMessage);
      webSocket.sendTXT(subscribeMessage);
      break;
    }
    case WStype_TEXT:
      Serial.println("📩 Mensaje recibido del servidor: " + String((char*)payload));
      break;
    case WStype_ERROR:
      Serial.println("⚠️ Error en la conexión WebSocket.");
      break;
  }
}
