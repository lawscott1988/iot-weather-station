#include <Arduino.h>
#include <DHT.h>
#include <RtcDS3231.h>
#include <Wire.h>

// ── Pin config ──────────────────────────────────────────
#define DHT_PIN     2
#define DHT_TYPE    DHT11

// ── Sensor objects ───────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);
RtcDS3231<TwoWire> rtc(Wire);

// ── Helpers ──────────────────────────────────────────────

// Pads a number with a leading zero if needed (e.g. 9 → "09")
void printPadded(int value) {
    if (value < 10) Serial.print("0");
    Serial.print(value);
}

// Prints a formatted timestamp: YYYY-MM-DD HH:MM:SS
void printTimestamp(const RtcDateTime& dt) {
    Serial.print(dt.Year());
    Serial.print("-");
    printPadded(dt.Month());
    Serial.print("-");
    printPadded(dt.Day());
    Serial.print(" ");
    printPadded(dt.Hour());
    Serial.print(":");
    printPadded(dt.Minute());
    Serial.print(":");
    printPadded(dt.Second());
}

// ── Setup ────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);
    Serial.println("IoT Weather Station starting...");

    // Start DHT sensor
    dht.begin();

    // Start RTC
    rtc.Begin();

    // If RTC lost power or time is not set, set it to compile time
    if (!rtc.IsDateTimeValid()) {
        Serial.println("RTC lost power — setting time to compile time.");
        rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    }

    if (!rtc.GetIsRunning()) {
        Serial.println("RTC was not running, starting now.");
        rtc.SetIsRunning(true);
    }

    // Disable unused RTC features to save power
    rtc.Enable32kHzPin(false);
    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    Serial.println("Timestamp            | Temp (C) | Humidity (%)");
    Serial.println("---------------------|----------|-------------");
}

// ── Loop ─────────────────────────────────────────────────
void loop() {
    // Read RTC
    RtcDateTime now = rtc.GetDateTime();

    // Read DHT11 (takes ~250ms)
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check for failed reads
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("ERROR: Failed to read from DHT11 sensor.");
        delay(2000);
        return;
    }

    // Print timestamped reading
    printTimestamp(now);
    Serial.print(" | ");
    Serial.print(temperature, 1);
    Serial.print("°C      | ");
    Serial.print(humidity, 1);
    Serial.println("%");

    // Wait 2 seconds before next reading (DHT11 minimum interval)
    delay(2000);
}