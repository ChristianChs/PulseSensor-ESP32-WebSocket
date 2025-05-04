# ESP32 Real-Time Pulse Sensor with WebSocket Streaming 🚀

Este proyecto permite leer datos de un **sensor de pulso cardíaco** usando un ESP32 y enviarlos en tiempo real mediante **WebSockets** a un servidor o aplicación web.

Ideal para proyectos de:
- Monitoreo de salud IoT 💓
- Visualizaciones en tiempo real 📊

## 🚀 Características
- Lee BPM (latidos por minuto) y señal cruda del sensor.
- Calcula el IBI (intervalo entre latidos).
- Envía datos en **JSON** a través de WebSocket.
- Soporte para **ESP32** usando Arduino IDE o PlatformIO.

## 📦 Requisitos
- ESP32 (cualquier modelo: DevKit, NodeMCU, etc.)
- Sensor de pulso (como el KY-039, SEN-11574 o PulseSensor oficial)
- Librerías:
  - [`WebSocketsClient`](https://github.com/Links2004/arduinoWebSockets)
  - [`PulseSensor Playground`](https://github.com/WorldFamousElectronics/PulseSensorPlayground)

## 🔌 Conexión
| Sensor Pulse | ESP32 Pin  |
|--------------|-----------|
| VCC          | 3.3V      |
| GND          | GND       |
| SIGNAL       | VP (GPIO36) |

> Por defecto se usa el pin **GPIO36 (VP)**, pero puedes cambiarlo en el código.

## 📡 Configuración del WebSocket
Asegúrate que tu servidor WebSocket está escuchando en:
- IP: `IP_SERVER`
- Puerto: `PORT`
- Path: `/ws`

## 🖥️ Ejemplo de Payload Enviado
```json
{
  "timestamp": 1234567,
  "bpm": 72,
  "ibi": 830,
  "signal": 512
}
