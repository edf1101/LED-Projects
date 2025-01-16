/*
 * Created by Ed Fillingham on 16/01/2025.
 *
 * Code for web server partially from randomnerdtutorials.com by Rui Santos
*/

#ifndef LED_PROJECTS_WEB_H
#define LED_PROJECTS_WEB_H


#include <WiFi.h>
#include "Arduino.h"
#include "string"
#include "vector"

class Project; // forward declaration

class Web {
public:
    void setupWeb(Project *project);

    void loopWeb();

private:
    Project *project;
    std::vector<std::string> effectNames = {};

    // Replace with your network credentials
    const char *ssid = "SKYF93G6";
    const char *password = "CSZt6Q9x4rDJ";

    // Set web server port number to 80
    WiFiServer server = WiFiServer(80);
    String header; // Variable to store the HTTP request
    unsigned long currentTime = millis(); // Current time
    unsigned long previousTime = 0; // Previous time
    const long timeoutTime = 500; // Define timeout time in milliseconds (example: 2000ms = 2s)


};


#endif //LED_PROJECTS_WEB_H
