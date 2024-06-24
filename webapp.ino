//#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>   // Include the LittleFS library
#include <OtaHelper.h>

//ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

#include "secrets.h"  // include wifi data

// Instance of the ConnectOta class
OtaHelper otaInstance;

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void setup() {
  //Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  otaInstance.start(ssid_1, password_1, nombre, "Sr71bL4k", 3232, 115200);
  //delay(10);
  //Serial.println('\n');

  //wifiMulti.addAP(ssid_1, password_1);   // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP(ssid_2, password_2);
  //wifiMulti.addAP(ssid_3, password_3);

  //Serial.println("Connecting ...");
  //int i = 0;
  //while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
  //  delay(250);
  //  Serial.print('.');
  //}
  //Serial.println('\n');
  //Serial.print("Connected to ");
  //Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  //Serial.print("IP address:\t");
  //Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  /*if (MDNS.begin(nombre)) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }*/

  Serial.println("\n\nIniciando de LittleFS");

  if (!LittleFS.begin()) {
    Serial.println("Error montando LittleFS");
    return;
  }

  Serial.println("Iniciando de LittleFS");                           // Start the LittleFS Flash Files System
  
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  otaInstance.handle();
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".svg")) return "image/svg+xml";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
