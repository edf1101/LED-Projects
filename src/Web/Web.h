#ifndef WEB_H
#define WEB_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include <string>

class Project;

class Web {
public:
    void setup(Project* _project);

private:
    Project* myProject;
    const char *ssid = "SKYF93G6";
    const char *password = "CSZt6Q9x4rDJ";

    AsyncWebServer server{80};
    std::vector<std::string> names{"Effect 1", "Effect 2", "Effect 3"};

    static const char index_html[];

    String outputState(int output);
    String processor(const String &var);

    void handleRootRequest(AsyncWebServerRequest *request);
    void handleUpdateRequest(AsyncWebServerRequest *request);
    void handleSetColorsRequest(AsyncWebServerRequest *request);
    void handleSetRainbowRequest(AsyncWebServerRequest *request);

    void configureRoutes();
};

#endif // WEB_H
