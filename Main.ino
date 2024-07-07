#include <map>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <WebServer.h>

// Substitua essas informações pelas suas credenciais de rede WiFi
const char* ssid = "networkName";
const char* password = "password";
String strX = "30";
String strY = "30";

WebServer server(80);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME  5000
esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE; // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
esp_spp_role_t role = ESP_SPP_ROLE_SLAVE; // or ESP_SPP_ROLE_MASTER
int executeOneTime = 1;

void setup() {
  Serial.begin(115200);
  if (!SerialBT.begin("ESP32test", true)) {
    Serial.println("========== serialBT failed!");
    abort();
  }
  WiFi.begin(ssid, password);
  Serial.println("");

  // Esperar pela conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to wifi network.");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Rota para manipular solicitações HTTP
  server.on("/", handleRoot);
  server.on("/data", handleData);

  // Iniciar o servidor
  server.begin();
  Serial.println("Server initialized.");
}

void handleRoot() {
  // HTML para o gráfico XY
  String html = "<!DOCTYPE HTML>\r\n";
  html += "<html>\r\n";
  html += "<head>\r\n";
  html += "<script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\r\n";
  html += "</head>\r\n";
  html += "<body>\r\n";
  html += "<canvas id=\"myChart\" width=\"400\" height=\"400\"></canvas>\r\n";
  html += "<script>\r\n";
  html += "var ctx = document.getElementById('myChart').getContext('2d');\r\n";
  html += "var myChart = new Chart(ctx, {\r\n";
  html += "  type: 'scatter',\r\n";
  html += "  data: {\r\n";
  html += "    datasets: [{\r\n";
  html += "      label: 'Pontos de dados',\r\n";
  html += "      data: [{ x: " + strX + ", y: " + strY + " }],\r\n";
  html += "      pointRadius: 15,\r\n";
  html += "      pointBackgroundColor: 'blue',\r\n";
  html += "      backgroundColor: 'rgba(255, 99, 132, 0.5)',\r\n";
  html += "      borderColor: 'rgba(255, 99, 132, 1)',\r\n";
  html += "      borderWidth: 1\r\n";
  html += "    }]\r\n";
  html += "  },\r\n";
  html += "  options: {\r\n";
  html += "    scales: {\r\n";
  html += "      x: {\r\n";
  html += "        type: 'linear',\r\n";
  html += "        position: 'bottom',\r\n";
  html += "        min: 0,\r\n"; 
  html += "        max: 5\r\n";
  html += "      },\r\n";
  html += "      y: {\r\n";
  html += "        type: 'linear',\r\n";
  html += "        position: 'left',\r\n";
  html += "        min: 0,\r\n";
  html += "        max: 10\r\n";
  html += "      }\r\n";
  html += "    }\r\n";
  html += "  }\r\n";
  html += "});\r\n";
  html += "setInterval(function() {\r\n";
  html += "  fetch('/data')\r\n";
  html += "    .then(response => response.json())\r\n";
  html += "    .then(data => {\r\n";
  html += "      myChart.data.datasets[0].data = [{ x: data.x, y: data.y }];\r\n";
  html += "      myChart.update();\r\n";
  html += "    });\r\n";
  html += "}, 5000);\r\n";
  html += "</script>\r\n";
  html += "</body>\r\n";
  html += "</html>\r\n";

  server.send(200, "text/html", html);
}


void handleData() {
  String json = "{ \"x\": " + strX + ", \"y\": " + strY + " }";
  server.send(200, "application/json", json);
}

void loop() {
  
  float rssiBeacon1 = 0, rssiBeacon2 = 0, rssiBeacon3 = 0;
  float d1 = 0, d2 = 0, d3 = 0;
  float x1 = 0.8, y1 = 10.0, x2 = 4.75, y2 = 4.8, x3 = 1.1, y3 = 2.2;
  float x, y;

  BTScanResults* btDeviceList = SerialBT.getScanResults();
  if (SerialBT.discoverAsync([](BTAdvertisedDevice* pDevice) {
        Serial.printf("scan\n");
      }))
    delay(BT_DISCOVER_TIME);
    SerialBT.discoverAsyncStop();
    delay(2000);
    if (btDeviceList->getCount() > 0) {
      BTAddress addr;
      int channel = 0;
      for (int i = 0; i < btDeviceList->getCount(); i++) {
        BTAdvertisedDevice* device = btDeviceList->getDevice(i);
        if (device->getAddress().toString() == "98:d3:32:10:63:69") // Beacon 1
        {
          rssiBeacon1 = device->getRSSI();
        }
        if (device->getAddress().toString() == "98:d3:31:fd:4c:eb") // Beacon 2
        {
          rssiBeacon2 = device->getRSSI();
        }
        if (device->getAddress().toString() == "98:d3:31:70:99:3b") // Beacon 3
        {
          rssiBeacon3 = device->getRSSI();
        }
      }
    }

  d1 = pow(10, (40 + rssiBeacon1) / (-55.4));
  d2 = pow(10, (40 + rssiBeacon2) / (-55.4););
  d3 = pow(10, (40 + rssiBeacon3) / (-55.4));

  x = ((pow(x1, 2) + pow(y1, 2) - pow(d1, 2)) * (y3 - y2) + (pow(x2, 2) + pow(y2, 2) - pow(d2, 2)) * (y1 - y3) + (pow(x3, 2) + pow(y3, 2) - pow(d3, 2)) * (y2 - y1)) / (2 * (x1 * (y3 - y2) + x2 * (y1 - y3) + x3 * (y2 - y1)));
  y = ((pow(x1, 2) + pow(y1, 2) - pow(d1, 2)) * (x3 - x2) + (pow(x2, 2) + pow(y2, 2) - pow(d2, 2)) * (x1 - x3) + (pow(x3, 2) + pow(y3, 2) - pow(d3, 2)) * (x2 - x1)) / (2 * (y1 * (x3 - x2) + y2 * (x1 - x3) + y3 * (x2 - x1)));

  Serial.printf("RSSI Beacon1 -- %.1f\n", rssiBeacon1);
  Serial.printf("RSSI Beacon2 -- %.1f\n", rssiBeacon2);
  Serial.printf("RSSI Beacon3 -- %.1f\n", rssiBeacon3);
  Serial.printf("Axis x: %.1f\n", x);
  Serial.printf("Axis y: %.1f\n", y);


  strX = String(x);
  strY = String(y);
  server.handleClient();

  delay(5000);
 }

