/*
   Metronome & Tuner with OLED Display Ver 1.2
   Prof. Hideki Isoda
   Sookmyung Women's University
*/

#include "U8glib.h" // This is the OLED display library

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // Activate I2C for OLED

const int pitch = 440;        // for Tuner Reference Pitch (440Hz A4)
const int frequency = 880;    // for Metronome (880Hz A5)
const int duration = 50;      // Beat duration
const int speakerPin = 6;     // Speaker (use 500Ω potentiometer for volume control)
const int ledPin = 12;        // Green LED to indicate beats and activation of reference pitch
const int potPin = 0;         // 10KΩ potentiometer for selecting the tempo preset
int potValue;
const int beatButtonPin = 7;  // push switch for start/stop the metronome
const int tuneButtonPin = 8;  // push swithc for activating the reference pitch
int beatButtonStatus = HIGH;
int tuneButtonStatus = HIGH;
boolean beatOn = false;
boolean tuneOn = false;
unsigned int beats_per_minute = 60;
unsigned int MS_per_beat = 0;
long last_time = 0;

// Musical Tempo Presets
int tempo_preset[] = {60, 63, 66, 69, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 126, 132, 138, 144, 152, 160, 168, 176, 184, 192};

void setup() {
  // Serial.begin(9600);      // Serial display can be used instead of OLED.
  pinMode(beatButtonPin, INPUT_PULLUP);
  pinMode(tuneButtonPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  unsigned int milliseconds_per_minute = 1000 * 60;
  MS_per_beat = milliseconds_per_minute / beats_per_minute;
}

void loop() {
  checkTuneButton();

  if (tuneOn == true) {
    beatOn = false;
    tone(speakerPin, pitch);
    digitalWrite(ledPin, HIGH);
    // Serial.println(pitch);

    u8g.firstPage();
    do {
      draw_tune();
    } while ( u8g.nextPage() );
  }

  if (tuneOn == false) {
    noTone(speakerPin);
    digitalWrite(ledPin, LOW);
    // Serial.println(beats_per_minute);

    u8g.firstPage();
    do {
      draw_metronome();
    } while ( u8g.nextPage() );
  }

  checkBeatButton();
  checkPot();

  long current_time = millis();

  if ((current_time - last_time >= MS_per_beat) && beatOn == true) {
    tuneOn = false;
    tone(speakerPin, frequency, duration);
    digitalWrite(ledPin, HIGH);
    delay(duration);
    digitalWrite(ledPin, LOW);
    delay(duration);
    last_time = current_time;
  }
}

void checkPot()
{
  int value = analogRead(potPin);
  if (value != potValue) {
    beats_per_minute = map(value, 0, 1024, 0, 27);
    beats_per_minute = tempo_preset[beats_per_minute];
    unsigned int milliseconds_per_minute = 1000 * 60;
    MS_per_beat = milliseconds_per_minute / beats_per_minute;
    potValue = value;
  }
}

void checkBeatButton() {
  int beat_button_state = digitalRead(beatButtonPin);
  if (beat_button_state == LOW && beat_button_state != beatButtonStatus) {
    beatOn = ! beatOn;
    beatButtonStatus = beat_button_state;
    delay(20);
  }
  if (beat_button_state == HIGH && beat_button_state != beatButtonStatus) {
    beatButtonStatus = beat_button_state;
    delay(20);
  }
}

void checkTuneButton() {
  int tune_button_state = digitalRead(tuneButtonPin);
  if (tune_button_state == LOW && tune_button_state != tuneButtonStatus) {
    tuneOn = ! tuneOn;
    tuneButtonStatus = tune_button_state;
    delay(20);
  }
  if (tune_button_state == HIGH && tune_button_state != tuneButtonStatus) {
    tuneButtonStatus = tune_button_state;
    delay(20);
  }
}

void draw_metronome() {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 15);
  u8g.print("Practice Hard!!!"); // You can change this message.

  u8g.setFont(u8g_font_8x13r);
  u8g.setPrintPos(20, 22);
  u8g.setScale2x2();
  u8g.print(beats_per_minute);
  u8g.undoScale();

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(96, 44);
  u8g.print("BPM");

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 62);
  u8g.print("SOOKMYUNG W UNIV");
}

void draw_tune() {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 15);
  u8g.print("Reference Pitch:");

  u8g.setFont(u8g_font_8x13r);
  u8g.setPrintPos(20, 22);
  u8g.setScale2x2();
  u8g.print("440");
  u8g.undoScale();

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(96, 44);
  u8g.print("Hz");

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 62);
  u8g.print("SOOKMYUNG W UNIV");
}
