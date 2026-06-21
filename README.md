# IoT Weather Station

An Arduino Uno project that reads temperature and humidity from a DHT11 sensor and timestamps each reading using a DS3231 RTC (Real Time Clock) module. Readings are printed to the serial monitor in a formatted table.

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