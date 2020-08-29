#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>

const char* ssid = "TP-LinkA"; // nombre de la red a la que se va a conectar
const char* password =  "804abcd20"; //password de la red
const char* device_secret_key = "d_sk8zRbBwrkuzU2hcXeG2"; //contraseña que se obtiene de la pagina de wia

char server[] = "api.wia.io"; // sitio web al que se enlaza
char path[] = "/v1/events"; // trayectoria de los eventos a almacenar
int port = 80; // puerto que atiende el servicio MQTT

/*******************************
 * Inicialización
 */
WiFiClient client;
int status = WL_IDLE_STATUS;
HttpClient httpClient = HttpClient(client, server, port);

StaticJsonDocument<200> jsonBuffer;
JsonObject root = jsonBuffer.to<JsonObject>();
/*******************************
 * Inicialización
 */
void setup() {
  Serial.begin(115200); // Se habilita el puerto serial para detectar errores
  while (!Serial) {
    ; 
  }
  WiFi.begin(ssid, password);
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP  network:
    // wait 5 seconds for connection:
    delay(5000);
  }
  /*******************************
 * Datos de la conección que serán impresos en la terminal
 */
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connected");
 /*******************************
 * Datos de la conección que serán impresos en la terminal
 */ 
}

void loop() {
  root["name"] = "hello-wia"; // nombre del evento a reportar
  root["data"] = 36.85; // valor a entregar
  postToWia(root); // llamado a función que hace la cadena de información con la especificación de JSON
  delay(10000);
}

/*******************************
 * Función que crea las cadenas para que Wia defina los elementos a guardar y mostrar
 */
void postToWia(JsonObject& data) {
  String dataStr = "";
  serializeJson(data, dataStr);
  httpClient.beginRequest();
  httpClient.post(path);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", dataStr.length());
  httpClient.sendHeader("Authorization", "Bearer "  + String(device_secret_key));
  httpClient.beginBody();
  httpClient.print(dataStr);
  httpClient.endRequest();
}
