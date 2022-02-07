#include <Adafruit_SSD1306.h>
#include <splash.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <Wire.h>
#include <Adafruit_BMP085.h>

#define seaLevelPressure_hPa 1013.25

#define BMP085_ADDRESS 0x77 

Adafruit_BMP085 bmp;

struct BpmMeasurements {
  float temperature;
  float pressure;
};

// @TODO: add hash to prevent redundant updates

struct ViewState {
  String temperature;
  String pressure;
} viewState;

void setup() {
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("\nBarometer v.1.prototype START");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 :o(");
    while (1) {}
  }
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.setRotation(2);
  display.display();
  delay(2000);
}


void loop() {
  BpmMeasurements m = readBpm();

  viewState.temperature = formatTemperature(m.temperature);
  viewState.pressure = formatPressure(m.pressure);
  
  Serial.print("Temperature = ");
  Serial.println(viewState.temperature);
  
  Serial.print("Pressure = ");
  Serial.println(viewState.pressure);
  
  Serial.println();

  updateDisplay();

  delay(6660);
}

BpmMeasurements readBpm(void) {
  return (BpmMeasurements){bmp.readTemperature(), bmp.readPressure()};
}

String formatPressure(float p) {
  p = round(p / 100.0) / 10.0;

  return String(p, 1) + " hPa";
}

String formatTemperature(float t) {
  return String(t, 1) + " *C";
}

void updateDisplay() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(viewState.temperature);

  display.setCursor(0, 20);
  display.print(viewState.pressure);

  display.display();
}
