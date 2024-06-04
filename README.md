# IoT Controlled House

This project is an IoT controlled house implemented using Arduino. It uses a variety of libraries and components to display and control various aspects of a house such as time, temperature, and humidity.

## Table of Contents

- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Description

The project uses an SSD1306 OLED display to show the current time, temperature, and humidity. It also allows the user to edit the time and date. The time is kept using a DS3231 RTC module. The temperature and humidity are read from a DHT11 sensor. The user interface is controlled using four buttons: next, select, increase, and decrease.

## Installation

1. Install the Arduino IDE.
2. Install the following libraries in the Arduino IDE:
   - Wire
   - Adafruit_GFX
   - Adafruit_SSD1306
   - RTClib
   - DHT
   - EasyButton
3. Connect the components as defined in the code.
4. Upload the code to your Arduino board.

## Usage

After installation, the OLED display will show a menu with four options: Show Time, Show Temp/Humid, Edit Time, and Edit Date. Use the next button to navigate through the menu, the select button to select an option, and the increase and decrease buttons to edit the time or date.

## Contributing

Contributions are welcome. Please open an issue to discuss your idea or submit a pull request.

## License

This project is open source and available under the [MIT License](LICENSE.md).