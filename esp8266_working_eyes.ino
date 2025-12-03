/*******************************************************
   ESP8266 ❱ RoboEyes Full UI
   Dark Mode + Joystick Toggle (when toggled then it will restart.did u fix it no ) + All Effects
********************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

// Fix conflict
#undef DEFAULT
#define DEFAULT 0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C

// ESP8266 I2C Pins
#define SDA_PIN 4  // D2
#define SCL_PIN 5  // D1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

const char* ssid = "SSID";
const char* password = "PASS";

ESP8266WebServer server(80);

/*******************************************************
                     STATE
********************************************************/
struct State {
  String mood = "DEFAULT";
  String position = "DEFAULT";
  bool curiosity = false;
  bool cyclops = false;
  bool sweat = false;
  bool joystickEnabled = false;
  bool hflicker = false;
  bool vflicker = false;
  int hf_amp = 1;
  int vf_amp = 1;
  int eyeWidth = 36;
  int eyeHeight = 36;
  int borderRadius = 8;
  int spaceBetween = 10;
} state;

/*******************************************************
                   WEBPAGE
********************************************************/
String webpage(); 

/*******************************************************
                    FULL WEBPAGE
********************************************************/
String webpage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

:root{
  --bg:#f2f2f7;
  --text:#1c1c1e;
  --smallbtn:#111010;
  --card:#ffffff;
  --accent:#007aff;
  --accent2:#34c759;
  --muted:#8e8e93;
  --radius:18px;
  --shadow:0 8px 20px rgba(0,0,0,0.1);
}

.dark{
  --bg:#000;
  --text:#00ff9d;
  --card:#111;
  --smallbtn:#000;
  --muted:#66ffb2;
  --shadow:0 8px 20px rgba(0,255,127,0.25);
}

body{
  margin:0;padding:18px;background:var(--bg);
  color:var(--text);
  font-family:-apple-system,BlinkMacSystemFont,Segoe UI,Roboto;
}

.wrap{ max-width:960px;margin:auto; }

.topbar{
  display:flex;
  justify-content:space-between;
  align-items:center;
  margin-bottom:18px;
}

.toggle-dark{
  width:52px;height:30px;border-radius:20px;
  background:#444;cursor:pointer;padding:4px;
  display:flex;align-items:center;
  transition:0.3s;
}
.toggle-dark.active{ background:var(--accent); }
.toggle-dark .dot{
  width:22px;height:22px;background:white;border-radius:50%;
  transition:0.3s;
}
.toggle-dark.active .dot{ transform:translateX(20px); }

.grid{
  display:grid;
  gap:16px;
  grid-template-columns:repeat(auto-fit,minmax(260px,1fr));
}

.card{
  background:var(--card);
  border-radius:var(--radius);
  padding:16px;
  box-shadow:var(--shadow);
  min-height:220px;
}

.label{
  font-size:13px;color:var(--muted);
  margin-bottom:8px;
}

.btn{
  padding:8px 14px;border-radius:12px;border:0;
  background:#e5e5ea;color:var(--smallbtn);
  cursor:pointer;transition:0.2s;
}
.btn-primary{ background:var(--accent);color:black; }

.ios-toggle{
  width:48px;height:28px;border-radius:20px;
  background:#ccc;cursor:pointer;padding:3px;
  display:flex;align-items:center;
  transition:0.3s;
}
.ios-toggle.active{ background:var(--accent2); }
.ios-toggle .dot{
  width:22px;height:22px;background:white;
  border-radius:50%;transition:0.3s;
}
.ios-toggle.active .dot{ transform:translateX(20px); }

#joystickwrap{
  width:100%;height:230px;
  display:flex;justify-content:center;align-items:center;
}
#joystick{
  width:200px;height:200px;position:relative;
}

input[type=range]{ width:80%; margin-top:10px; }

.animBtn{ margin-bottom:8px; }

</style>
</head>

<body>

<div class="wrap">

  <div class="topbar">
    <h1>RoboEyes</h1>
    <div id="darkToggle" class="toggle-dark" onclick="toggleDark()">
      <div class="dot"></div>
    </div>
  </div>

  <div class="grid">

    <!-- MOOD -->
    <div class="card">
      <div class="label">Mood</div>
      <div id="moodBtns">
        <button class="btn btn-primary" onclick="setMood('DEFAULT',this)">Normal</button>
        <button class="btn" onclick="setMood('HAPPY',this)">Happy</button>
        <button class="btn" onclick="setMood('ANGRY',this)">Angry</button>
        <button class="btn" onclick="setMood('TIRED',this)">Tired</button>
      </div>
    </div>

    <!-- POSITION -->
    <div class="card">
      <div class="label">Position</div>
      <div id="posBtns">
        <button class="btn" onclick="setPositionUI('N',this)">N</button>
        <button class="btn" onclick="setPositionUI('NE',this)">NE</button>
        <button class="btn" onclick="setPositionUI('E',this)">E</button>
        <button class="btn" onclick="setPositionUI('SE',this)">SE</button>
        <button class="btn" onclick="setPositionUI('S',this)">S</button>
        <button class="btn" onclick="setPositionUI('SW',this)">SW</button>
        <button class="btn" onclick="setPositionUI('W',this)">W</button>
        <button class="btn" onclick="setPositionUI('NW',this)">NW</button>
        <button class="btn btn-primary" onclick="setPositionUI('DEFAULT',this)">Center</button>
      </div>
    </div>

    <!-- JOYSTICK -->
    <div class="card">
      <div class="row" style="display:flex;gap:10px;margin-bottom:10px;">
        <div class="label">Joystick</div>
        <div id="joyToggle" class="ios-toggle" onclick="toggleJoystick()">
          <div class="dot"></div>
        </div>
      </div>

      <div id="joystickwrap">
        <div id="joystick"></div>
      </div>
    </div>

    <!-- MODES -->
    <div class="card">
      <div class="row">
        <div class="label">Curiosity</div>
        <div class="ios-toggle" id="curToggle" onclick="toggleCuriosity()">
          <div class="dot"></div>
        </div>
      </div>

      <div class="row" style="margin-top:14px;">
        <div class="label">Cyclops</div>
        <div class="ios-toggle" id="cycToggle" onclick="toggleCyclops()">
          <div class="dot"></div>
        </div>
      </div>
    </div>

    <!-- EFFECTS -->
    <div class="card">

      <div class="row">
        <div class="label">Chill</div>
        <div class="ios-toggle" id="sweatToggle" onclick="toggleSweat()">
          <div class="dot"></div>
        </div>
      </div>

      <div class="row" style="margin-top:14px;">
        <div class="label">Horizontal Flicker</div>
        <div class="ios-toggle" id="hfToggle" onclick="toggleHF()">
          <div class="dot"></div>
        </div>
      </div>
      <input type="range" id="hfamp" min="0" max="6" value="1" oninput="setHFamp(this.value)">

      <div class="row" style="margin-top:14px;">
        <div class="label">Vertical Flicker</div>
        <div class="ios-toggle" id="vfToggle" onclick="toggleVF()">
          <div class="dot"></div>
        </div>
      </div>
      <input type="range" id="vfamp" min="0" max="6" value="1" oninput="setVFamp(this.value)">

    </div>

    <!-- GEOMETRY -->
    <div class="card">
      <div class="label">Geometry</div>

      <p>Width <span id="wval">36</span></p>
      <input type="range" id="width" min="8" max="80" value="36" oninput="setGeom('width',this.value)">

      <p>Height <span id="hval">36</span></p>
      <input type="range" id="height" min="8" max="80" value="36" oninput="setGeom('height',this.value)">

      <p>Radius <span id="rval">8</span></p>
      <input type="range" id="radius" min="0" max="30" value="8" oninput="setGeom('radius',this.value)">

      <p>Space <span id="sval">10</span></p>
      <input type="range" id="space" min="-30" max="40" value="10" oninput="setGeom('space',this.value)">
    </div>

    <!-- ANIM -->
    <div class="card">
      <div class="label">Animations</div>
      <button class="btn animBtn" onclick="triggerAnim('laugh',this,900)">Laugh</button>
      <button class="btn animBtn" onclick="triggerAnim('confused',this,900)">Confused</button>
      <button class="btn animBtn" onclick="triggerAnim('blink',this,600)">Blink</button>
    </div>

  </div>
</div>

<script src="https://cdnjs.cloudflare.com/ajax/libs/nipplejs/0.9.0/nipplejs.min.js"></script>

<script>

let joystickEnabled = false;

function fetchOK(u){ fetch(u).catch(()=>{}); }

function toggleDark(){
  document.body.classList.toggle("dark");
  document.getElementById("darkToggle").classList.toggle("active");
}

function setMood(m,el){
  document.querySelectorAll("#moodBtns .btn").forEach(b=>b.classList.remove("btn-primary"));
  el.classList.add("btn-primary");
  fetchOK("/mood/" + m);
}

function setPositionUI(p,el){
  if(joystickEnabled) return;
  document.querySelectorAll("#posBtns .btn").forEach(b=>b.classList.remove("active"));
  el.classList.add("active");
  fetchOK("/pos/"+p);
}

function toggleJoystick(){
  let el = document.getElementById("joyToggle");
  el.classList.toggle("active");
  joystickEnabled = el.classList.contains("active");
  fetchOK("/joystick/en?on="+(joystickEnabled?1:0));
}

var joystick = nipplejs.create({
  zone: document.getElementById("joystick"),
  mode: "static",
  position: { left:"50%", top:"50%" },
  color: "#007aff"
});

joystick.on("move",(e,data)=>{
  if(!joystickEnabled) return;
  if(!data.vector) return;
  let x = Math.round((data.vector.x + 1) * 50);
  let y = Math.round((1 - data.vector.y) * 50);
  fetchOK(`/joystick?x=${x}&y=${y}`);
});

joystick.on("end",()=>{
  if(joystickEnabled) fetchOK("/joystick?x=50&y=50");
});

function toggleCuriosity(){
  curToggle.classList.toggle("active");
  fetchOK(`/curiosity?on=${curToggle.classList.contains("active")?1:0}`);
}

function toggleCyclops(){
  cycToggle.classList.toggle("active");
  fetchOK(`/cyclops?on=${cycToggle.classList.contains("active")?1:0}`);
}

function toggleSweat(){
  sweatToggle.classList.toggle("active");
  fetchOK(`/sweat?on=${sweatToggle.classList.contains("active")?1:0}`);
}

function toggleHF(){
  hfToggle.classList.toggle("active");
  setHFamp(hfamp.value);
}
function toggleVF(){
  vfToggle.classList.toggle("active");
  setVFamp(vfamp.value);
}

function setHFamp(v){
  fetchOK(`/flicker/h?on=${hfToggle.classList.contains("active")?1:0}&amp=${v}`);
}
function setVFamp(v){
  fetchOK(`/flicker/v?on=${vfToggle.classList.contains("active")?1:0}&amp=${v}`);
}

function setGeom(which,val){
  document.getElementById(which[0]+"val").innerText=val;
  fetchOK(`/geom/${which}?val=${val}`);
}

function triggerAnim(name,el,time){
  el.classList.add("active");
  fetchOK("/anim/" + name);
  setTimeout(()=>{ el.classList.remove("active"); }, time);
}

</script>

</body>
</html>
)rawliteral";
}
  // defined in part 2

/*******************************************************
                BASIC HANDLERS
********************************************************/

void handleRoot() {
  server.send(200, "text/html", webpage());
}

void handleMood() {
  String m = server.uri().substring(server.uri().lastIndexOf('/') + 1);

  if (m == "HAPPY") roboEyes.setMood(HAPPY);
  else if (m == "ANGRY") roboEyes.setMood(ANGRY);
  else if (m == "TIRED") roboEyes.setMood(TIRED);
  else roboEyes.setMood(DEFAULT);

  state.mood = m;
  server.send(200, "text/plain", "OK");
}

void handlePos() {
  if (state.joystickEnabled) {
    server.send(200, "text/plain", "IGNORED");
    return;
  }

  String p = server.uri().substring(server.uri().lastIndexOf('/') + 1);

  if (p == "N") roboEyes.setPosition(N);
  else if (p == "NE") roboEyes.setPosition(NE);
  else if (p == "E") roboEyes.setPosition(E);
  else if (p == "SE") roboEyes.setPosition(SE);
  else if (p == "S") roboEyes.setPosition(S);
  else if (p == "SW") roboEyes.setPosition(SW);
  else if (p == "W") roboEyes.setPosition(W);
  else if (p == "NW") roboEyes.setPosition(NW);
  else roboEyes.setPosition(DEFAULT);

  state.position = p;
  server.send(200, "text/plain", "OK");
}

void handleAnim() {
  String a = server.uri().substring(server.uri().lastIndexOf('/') + 1);

  if (a == "laugh") roboEyes.anim_laugh();
  else if (a == "confused") roboEyes.anim_confused();
  else if (a == "blink") roboEyes.blink();

  server.send(200, "text/plain", "OK");
}

void handleCuriosity() {
  bool on = server.arg("on") == "1";
  state.curiosity = on;
  roboEyes.setCuriosity(on ? ON : OFF);
  server.send(200, "text/plain", "OK");
}

void handleCyclops() {
  bool on = server.arg("on") == "1";
  state.cyclops = on;
  roboEyes.setCyclops(on ? ON : OFF);
  server.send(200, "text/plain", "OK");
}

void handleSweat() {
  bool on = server.arg("on") == "1";
  state.sweat = on;
  roboEyes.setSweat(on ? ON : OFF);
  server.send(200, "text/plain", "OK");
}

void handleFlicker() {
  bool on = server.arg("on") == "1";
  int amp = server.arg("amp").toInt();

  if (server.uri().indexOf("/flicker/h") >= 0) {
    state.hflicker = on;
    state.hf_amp = amp;
    if (on) roboEyes.setHFlicker(ON, amp);
    else roboEyes.setHFlicker(OFF);
  } else {
    state.vflicker = on;
    state.vf_amp = amp;
    if (on) roboEyes.setVFlicker(ON, amp);
    else roboEyes.setVFlicker(OFF);
  }

  server.send(200, "text/plain", "OK");
}

void handleGeom() {
  int val = server.arg("val").toInt();
  String k = server.uri().substring(server.uri().lastIndexOf('/') + 1);

  if (k == "width") {
    roboEyes.eyeLwidthNext = val;
    roboEyes.eyeRwidthNext = val;
    state.eyeWidth = val;
  }
  else if (k == "height") {
    roboEyes.eyeLheightNext = val;
    roboEyes.eyeRheightNext = val;
    state.eyeHeight = val;
  }
  else if (k == "radius") {
    roboEyes.eyeLborderRadiusNext = val;
    roboEyes.eyeRborderRadiusNext = val;
    state.borderRadius = val;
  }
  else if (k == "space") {
    roboEyes.spaceBetweenNext = val;
    state.spaceBetween = val;
  }

  server.send(200, "text/plain", "OK");
}

/*******************************************************
         ** SAFE JOYSTICK — NO CRASHES **
********************************************************/

unsigned long lastJoy = 0;
int lastX = -1, lastY = -1;

void handleJoystickEnable() {
  state.joystickEnabled = (server.arg("on") == "1");
  if (state.joystickEnabled) state.position = "";
  server.send(200, "text/plain", "OK");
}

void handleJoystick() {
  if (!state.joystickEnabled) {
    server.send(200, "text/plain", "IGNORED");
    return;
  }

  unsigned long now = millis();
  if (now - lastJoy < 30) {
    server.send(200, "text/plain", "SKIP");
    return;
  }
  lastJoy = now;

  int x = server.arg("x").toInt();
  int y = server.arg("y").toInt();

  if (x == lastX && y == lastY) {
    server.send(200, "text/plain", "OK");
    return;
  }

  lastX = x;
  lastY = y;

  int sx = (x * roboEyes.getScreenConstraint_X()) / 100;
  int sy = (y * roboEyes.getScreenConstraint_Y()) / 100;

  roboEyes.eyeLxNext = sx;
  roboEyes.eyeRxNext = sx;
  roboEyes.eyeLyNext = sy;
  roboEyes.eyeRyNext = sy;

  server.send(200, "text/plain", "OK");
}

/*******************************************************
                    SETUP
********************************************************/

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) {
    Serial.println("OLED FAIL");
    while (1);
  }

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 70);
  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setIdleMode(ON, 2, 2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(200);

  server.on("/", handleRoot);

  server.on("/mood/DEFAULT", handleMood);
  server.on("/mood/HAPPY", handleMood);
  server.on("/mood/ANGRY", handleMood);
  server.on("/mood/TIRED", handleMood);

  server.on("/pos/DEFAULT", handlePos);
  server.on("/pos/N", handlePos);
  server.on("/pos/NE", handlePos);
  server.on("/pos/E", handlePos);
  server.on("/pos/SE", handlePos);
  server.on("/pos/S", handlePos);
  server.on("/pos/SW", handlePos);
  server.on("/pos/W", handlePos);
  server.on("/pos/NW", handlePos);

  server.on("/curiosity", handleCuriosity);
  server.on("/cyclops", handleCyclops);
  server.on("/sweat", handleSweat);

  server.on("/flicker/h", handleFlicker);
  server.on("/flicker/v", handleFlicker);

  server.on("/geom/width", handleGeom);
  server.on("/geom/height", handleGeom);
  server.on("/geom/radius", handleGeom);
  server.on("/geom/space", handleGeom);

  server.on("/anim/laugh", handleAnim);
  server.on("/anim/confused", handleAnim);
  server.on("/anim/blink", handleAnim);

  server.on("/joystick/en", handleJoystickEnable);
  server.on("/joystick", handleJoystick);

  server.begin();
}

/*******************************************************
                      LOOP
********************************************************/
void loop() {
  server.handleClient();
  roboEyes.update();
}
