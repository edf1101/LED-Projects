#include "Web.h"
#include "Projects/Project.h"
#include "Lib/Gradient.h"

// HTML Template
const char Web::index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>LED Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 2.0rem; margin-top: 20px;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .row {display: flex; justify-content: center; gap: 20px; margin-bottom: 20px;}
    .color-picker-container {display: flex; flex-direction: column; align-items: center;}
    .color-picker-container label {font-size: 1.5rem; margin-bottom: 5px;}
    .button {background-color: #4CAF50; border: none; color: white; padding: 10px 20px; text-align: center; font-size: 1.5rem; cursor: pointer; border-radius: 5px;}
    .button:hover {background-color: #45a049;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px;}
    .switch input {display: none;}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px;}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px;}
    input:checked+.slider {background-color: #b30000;}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px);}
  </style>
</head>
<body>
  <h2>LED Controller</h2>

  <!-- Effect Selection Section -->
  <p>Effect Selection</p>
  %BUTTONPLACEHOLDER%

  <!-- Color Selection Section -->
  <p>Color Selection</p>
  <div class="row">
    <div class="color-picker-container">
      <label for="colorPicker1">Primary Color</label>
      <input type="color" id="colorPicker1" value="#ff0000">
    </div>
    <div class="color-picker-container">
      <label for="colorPicker2">Secondary Color</label>
      <input type="color" id="colorPicker2" value="#0000ff">
    </div>
  </div>
  <button class="button" onclick="setColors()">Set Color</button>
  <button class="button" style="margin-top: 10px;" onclick="setRainbow()">Rainbow Color</button>

<script>
  function toggleCheckbox(element) {
    var checkboxes = document.querySelectorAll("input[type='checkbox']");
    checkboxes.forEach(function(checkbox) {
      if (checkbox !== element) {
        checkbox.checked = false;
      }
    });

    var xhr = new XMLHttpRequest();
    if (element.checked) {
      xhr.open("GET", "/update?output=" + element.id + "&state=1", true);
    }
    xhr.send();
  }

  function setColors() {
    // Get color values
    var color1 = document.getElementById("colorPicker1").value;
    var color2 = document.getElementById("colorPicker2").value;

    // Convert colors from hex to RGB
    var r1 = parseInt(color1.substring(1, 3), 16);
    var g1 = parseInt(color1.substring(3, 5), 16);
    var b1 = parseInt(color1.substring(5, 7), 16);

    var r2 = parseInt(color2.substring(1, 3), 16);
    var g2 = parseInt(color2.substring(3, 5), 16);
    var b2 = parseInt(color2.substring(5, 7), 16);

    // Send RGB values to ESP32
    var xhr = new XMLHttpRequest();
    xhr.open("GET", `/setColors?r1=${r1}&g1=${g1}&b1=${b1}&r2=${r2}&g2=${g2}&b2=${b2}`, true);
    xhr.send();
  }

  function setRainbow() {
    // Send request to ESP32 to set rainbow gradient
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/setRainbow", true);
    xhr.send();
  }
</script>
</body>
</html>
)rawliteral";




void Web::setup(Project *_project, const char *ssid, const char *password) {
  myProject = _project;
  names = myProject->getEffectNames();
  // Debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  // Configure routes
  configureRoutes();

  // Start the server
  server.begin();
}


String Web::outputState(int output) {

  return (myProject->getCurrentEffectName() == names[output]) ? "checked" : "";
}

String Web::processor(const String &var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons;
    for (int index = 0; index < names.size(); index++) {
      String name = names[index].c_str();
      buttons += "<h4>" + name + "</h4>";
      buttons += "<label class=\"switch\">";
      buttons += "<input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(index) + "\" " +
                 outputState(index) + ">";
      buttons += "<span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}

void Web::handleRootRequest(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html, [this](const String &var) { return processor(var); });
}

void Web::handleUpdateRequest(AsyncWebServerRequest *request) {
  String inputMessage1, inputMessage2;
  if (request->hasParam("output") && request->hasParam("state")) {
    inputMessage1 = request->getParam("output")->value();
    inputMessage2 = request->getParam("state")->value();
    digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    if (inputMessage2 == "1") {
      int index = inputMessage1.toInt();
      std::string effectName = names[index];
      // handle effect here
      myProject->setEffect(effectName);
    }
    request->send(200, "text/plain", "OK");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void Web::handleSetColorsRequest(AsyncWebServerRequest *request) {
  if (request->hasParam("r1") && request->hasParam("g1") && request->hasParam("b1") &&
      request->hasParam("r2") && request->hasParam("g2") && request->hasParam("b2")) {
    String r1 = request->getParam("r1")->value();
    String g1 = request->getParam("g1")->value();
    String b1 = request->getParam("b1")->value();

    String r2 = request->getParam("r2")->value();
    String g2 = request->getParam("g2")->value();
    String b2 = request->getParam("b2")->value();

    // Initial gradient
    GradientStop *newStops = new GradientStop[3]{
            {0,   (uint8_t)r1.toInt(), (uint8_t)g1.toInt(), (uint8_t)b1.toInt(),   0},
            {128, (uint8_t)r2.toInt(), (uint8_t)g2.toInt(), (uint8_t)b2.toInt(), 0},
            {255, (uint8_t)r1.toInt(), (uint8_t)g1.toInt(), (uint8_t)b1.toInt(),   0},
    };
    auto *newGradient = new Gradient(newStops, 3, false);
    Gradient::setCurrentGradient(newGradient);



    request->send(200, "text/plain", "Colors received");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void Web::handleSetRainbowRequest(AsyncWebServerRequest *request) {
  // Define a simple rainbow gradient
  GradientStop *rainbowStops = new GradientStop[6]{
          {0,   255, 0,   0,   0},  // Red
          {85,  0, 255, 0,   0},  // green
          {170, 0, 0, 255,   0},  // blue
          {255, 255,   0, 0,   0} // red
  };
  auto *rainbowGradient = new Gradient(rainbowStops, 4, false);
  Gradient::setCurrentGradient(rainbowGradient);

  request->send(200, "text/plain", "Rainbow gradient set");
}

void Web::configureRoutes() {
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) { handleRootRequest(request); });
  server.on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) { handleUpdateRequest(request); });
  server.on("/setColors", HTTP_GET, [this](AsyncWebServerRequest *request) { handleSetColorsRequest(request); });
  server.on("/setRainbow", HTTP_GET, [this](AsyncWebServerRequest *request) { handleSetRainbowRequest(request); });
}


