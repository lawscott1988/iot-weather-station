# IoT Weather Station

An Arduino Uno project that reads temperature and humidity from a DHT11 sensor and timestamps each reading using a DS3231 RTC (Real Time Clock) module. Readings are printed to the serial monitor in a formatted table.

The long term goal is to evolve this into a full IoT data platform — a fleet of wireless weather stations around the house feeding a data pipeline, compared against official Bureau of Meteorology (BOM) data for Brisbane.

---

## Project roadmap

### Stage 1 — Single wired station ← current
**Goal:** One sensor, data visible on screen

- [x] Arduino Uno + DHT11 + DS3231
- [x] PlatformIO + VSCode setup
- [x] Git repo with branch protection
- [ ] Python logger saving to CSV locally
- [ ] README documented

### Stage 2 — Wireless single station
**Goal:** Cut the USB cable, data flows over WiFi

- [ ] Upgrade to ESP32
- [ ] Connect to home WiFi
- [ ] Install local Mosquitto MQTT broker
- [ ] ESP32 publishes readings to MQTT every 30 seconds
- [ ] Python ingestion script writes to PostgreSQL + TimescaleDB
- [ ] Verify data arriving correctly

### Stage 3 — Orchestration with Dagster
**Goal:** Automate the data flow with proper orchestration

- [ ] Set up Dagster locally
- [ ] Wrap ingestion script as a Dagster asset
- [ ] Schedule pipeline to run every 5 minutes
- [ ] Add alerting if sensor stops sending data
- [ ] Add data quality checks (e.g. temperature must be between -10 and 60°C)

### Stage 4 — Data modelling with dbt
**Goal:** Clean, structured, queryable data

- [ ] Set up dbt project on top of PostgreSQL
- [ ] Write raw, staging, and mart models
- [ ] Mart models: `hourly_weather`, `daily_weather`, `rolling_7_day`
- [ ] Add dbt tests (not null, valid ranges, freshness checks)
- [ ] Integrate dbt runs into Dagster

### Stage 5 — Grafana dashboard
**Goal:** See your data visually

- [ ] Install Grafana
- [ ] Build panels: live gauges, 24hr trend, daily min/max, humidity heatmap
- [ ] Set up Grafana alerts (e.g. notify if humidity exceeds 90%)

### Stage 6 — Fleet of sensors
**Goal:** Multiple stations around the house and garden

- [ ] Add `station_id` and `location` fields to data model
- [ ] Flash additional ESP32s with unique station IDs
- [ ] Each ESP32 publishes to its own MQTT topic (e.g. `weather/backyard`)
- [ ] Update ingestion to subscribe to `weather/#` (all stations)
- [ ] Update dbt models and Grafana to show all stations

### Stage 7 — BOM data integration
**Goal:** Compare local readings against official Brisbane data

- [ ] Pull BOM data via API into PostgreSQL
- [ ] Write dbt model joining sensor data with BOM data by timestamp
- [ ] Add Grafana panel comparing readings vs BOM side by side
- [ ] Identify local microclimate patterns vs official Brisbane Airport station

### Stage 8 — Apache Spark for scale
**Goal:** Handle large historical datasets and complex analysis

- [ ] Set up Apache Spark
- [ ] Move heavy transformations to Spark (backfill, anomaly detection, large joins)
- [ ] Add Spark jobs as Dagster assets
- [ ] Seasonal pattern analysis, sensor drift detection

### Stage 9 — Productionise
**Goal:** Reliable, maintainable, scalable system

- [ ] Move broker, database, Dagster, and Grafana to Raspberry Pi or cloud VM
- [ ] Proper secrets management
- [ ] Automated database backups
- [ ] CI/CD for dbt models via GitHub Actions
- [ ] Full architecture documented

---

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno | Microcontroller |
| DHT11 (3-pin module) | Temperature and humidity sensor |
| DS3231 RTC module | Real time clock with battery backup |
| 7x jumper wires | Connections |

## Wiring

### DHT11 (3-pin module)

| Module Pin | Arduino Pin | Wire colour |
|---|---|---|
| S (Signal) | Pin 2 | Green |
| + (VCC) | 5V | Red |
| - (GND) | GND | Black |

### DS3231 RTC

| Module Pin | Arduino Pin | Wire colour |
|---|---|---|
| VCC | 3.3V | Orange |
| GND | GND | Black |
| SDA | A4 | Blue |
| SCL | A5 | Purple |

> **Note:** The 3-pin DHT11 module has a pull-up resistor built in — no breadboard or external resistor needed.

> **Note:** The DS3231 runs on 3.3V, not 5V. Connecting it to 5V may damage the module.

## Software dependencies

Libraries are managed via PlatformIO and declared in `platformio.ini`. They will be downloaded automatically on first build.

| Library | Purpose |
|---|---|
| adafruit/DHT sensor library | Read DHT11 temperature and humidity |
| adafruit/Adafruit Unified Sensor | Required dependency for DHT library |
| makuna/RTC | Communicate with DS3231 over I2C |

## Serial output

Open the serial monitor at **9600 baud** after flashing. Output looks like:

```
Timestamp            | Temp (C) | Humidity (%)
---------------------|----------|-------------
2026-06-21 07:23:45  | 22.0°C   | 58.0%
2026-06-21 07:23:47  | 22.0°C   | 58.0%
```

Readings are taken every 2 seconds (the DHT11 minimum polling interval).

## RTC time sync

On first flash, the RTC is automatically set to the compile time of the sketch. If the RTC loses power and its coin cell battery is flat, it will re-sync to compile time on the next flash.

---

## Development setup

### Prerequisites

- [Visual Studio Code](https://code.visualstudio.com/)
- [Git](https://git-scm.com/)
- A USB cable for your Arduino Uno

### 1. Install the PlatformIO extension

1. Open VSCode
2. Go to Extensions (`Ctrl+Shift+X`)
3. Search for **PlatformIO IDE**
4. Click **Install**
5. Restart VSCode when prompted

A PlatformIO icon (alien head) will appear in the left sidebar.

### 2. Clone the repository

```bash
git clone git@github.com:lawscott1988/iot-weather-station.git
cd iot-weather-station
```

### 3. Open in VSCode

```bash
code .
```

PlatformIO will detect the `platformio.ini` file and configure the project automatically.

### 4. Add PlatformIO to PATH

The `pio` command line tool needs to be on your PATH so you can run builds and uploads from the terminal.

#### Windows (Git Bash)

Add the following line to your `~/.bashrc` file:

```bash
echo 'export PATH="$HOME/.platformio/penv/Scripts:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

Verify it worked:

```bash
pio --version
```

#### Linux / macOS

Add the following line to your `~/.bashrc` (or `~/.zshrc` if using Zsh):

```bash
echo 'export PATH="$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

Verify it worked:

```bash
pio --version
```

> **Note:** The difference between Windows and Linux/macOS is the folder name — `Scripts` on Windows, `bin` on Linux/macOS.

### 5. Build and upload

Plug in your Arduino Uno via USB, then run:

```bash
# Build only
pio run

# Build and upload to Arduino
pio run --target upload

# Open serial monitor
pio device monitor --baud 9600
```

Or use the PlatformIO toolbar buttons at the bottom of VSCode:
- **✓** — Build
- **→** — Upload
- **🔌** — Serial monitor

---

## Git workflow

This repository protects the `main` branch — all changes must go through a pull request.

```bash
# Create a new branch for your changes
git checkout -b feature/your-feature-name

# Make changes, then stage and commit
git add .
git commit -m "describe what you changed"

# Push the branch to GitHub
git push

# Then open a Pull Request on GitHub to merge into main
```

## Project structure

```
iot-weather-station/
├── src/
│   └── main.cpp        ← main sketch
├── include/            ← header files
├── lib/                ← local libraries
├── test/               ← unit tests
├── platformio.ini      ← board and library config
└── .gitignore
```

## License

MIT