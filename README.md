# Arduino Menu System with OLED Display, RTC, DHT, and Buttons

This project implements a menu system on an OLED display using an RTC for timekeeping, a DHT sensor for temperature and humidity, and buttons for user interaction.

## Hardware Requirements

- Arduino board
- OLED display (SSD1306)
- RTC module (DS3231)
- DHT11 sensor
- EasyButton library compatible buttons (4 buttons)
- Connecting wires

## Libraries Required

- `Wire.h`
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`
- `RTClib.h`
- `DHT.h`
- `EasyButton.h`

## Setup

1. Connect the OLED display to the Arduino (SDA, SCL, VCC, and GND).
2. Connect the DS3231 RTC module to the Arduino (SDA, SCL, VCC, and GND).
3. Connect the DHT11 sensor to a digital pin on the Arduino (data pin, VCC, and GND).
4. Connect four buttons to the Arduino with one side to digital pins and the other to ground.

## Code Overview

The code is structured to display different menus and handle interactions using buttons. The menus include displaying the current time, showing temperature and humidity, editing the time, and editing the date.

## Adding a New Menu State

To add a new `MenuState`, follow these steps:

1. **Define the New MenuState:**
   Add the new state to the `MenuState` enum.
   ```cpp
   enum MenuState { SHOW_MENU, SHOW_TIME, SHOW_TEMP_HUMID, EDIT_TIME, EDIT_DATE, NEW_STATE };
   ```

2. **Update the Menu Display:**
   Add the new menu item to the `showMenu` function.
   ```cpp
   void showMenu() {
     // Existing menu items...
     display.setCursor(0, 40);
     display.print(selectedMenuItem == 4 ? "> " : "  ");
     display.print("5. New State");
   }
   ```

3. **Handle Menu Selection:**
   Update the `onSelectPressed` function to handle the new state.
   ```cpp
   void onSelectPressed() {
     if (menuState == SHOW_MENU) {
       switch (selectedMenuItem) {
         case 0:
           menuState = SHOW_TIME;
           break;
         case 1:
           menuState = SHOW_TEMP_HUMID;
           break;
         case 2:
           menuState = EDIT_TIME;
           break;
         case 3:
           menuState = EDIT_DATE;
           break;
         case 4:
           menuState = NEW_STATE;
           break;
       }
     } else {
       // Handle other states...
     }
   }
   ```

4. **Implement the New Menu Function:**
   Create a function to handle the display and logic for the new state.
   ```cpp
   void showNewState() {
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0, 0);
     display.print("New State");
     // Add additional display or logic code here
   }
   ```

5. **Update the Main Loop:**
   Add a case for the new state in the main `loop` function.
   ```cpp
   void loop() {
     buttonNext.read();
     buttonSelect.read();
     buttonIncrease.read();
     buttonDecrease.read();

     display.clearDisplay();
     switch (menuState) {
       case SHOW_MENU:
         showMenu();
         break;
       case SHOW_TIME:
         showTime();
         break;
       case SHOW_TEMP_HUMID:
         showTempHumid();
         break;
       case EDIT_TIME:
         editTimeMenu();
         break;
       case EDIT_DATE:
         editDateMenu();
         break;
       case NEW_STATE:
         showNewState();
         break;
     }
     display.display();

     // Fast increment or decrement logic...
   }
   ```

6. **Compile and Upload:**
   Compile the code and upload it to your Arduino. Verify that the new menu state appears and functions as expected.

## Detailed Button Functions

- **Next Button:** Cycles through menu items in the main menu.
- **Select Button:** Selects the highlighted menu item.
- **Increase Button:** Increases values when in edit modes.
- **Decrease Button:** Decreases values when in edit modes.
- **Long Press Increase Button:** Fast increment values when held down.
- **Long Press Decrease Button:** Fast decrement values when held down.

## Fast Increment and Decrement

The fast increment and decrement functionality is achieved through long presses on the increase and decrease buttons. This allows for quicker adjustments when editing time or date values.

## Example Usage

Upon startup, the OLED displays the main menu. Use the `Next` button to cycle through the menu options. Press the `Select` button to enter a selected menu. In the edit modes, use the `Increase` and `Decrease` buttons to adjust values. Long press these buttons for fast adjustments.

## License

This project is open source and available under the [MIT License](LICENSE.md).