#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <DHT.h>
#include <EasyButton.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;
DHT dht(2, DHT11); // Assuming DHT sensor is connected to digital pin 2

// Define buttons and their pins
EasyButton buttonNext(3);
EasyButton buttonSelect(4);
EasyButton buttonIncrease(5);
EasyButton buttonDecrease(6);

enum MenuState { SHOW_MENU, SHOW_TIME, SHOW_TEMP_HUMID, EDIT_TIME, EDIT_DATE };
MenuState menuState = SHOW_MENU;
int selectedMenuItem = 0;

int editHour = 0, editMinute = 0, editSecond = 0;
int editYear = 0, editMonth = 0, editDay = 0;

const int longPressDuration = 1000; // Long press duration in milliseconds
bool fastIncrement = false;
bool fastDecrement = false;

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  dht.begin();

  buttonNext.begin();
  buttonSelect.begin();
  buttonIncrease.begin();
  buttonDecrease.begin();

  buttonNext.onPressed(onNextPressed);
  buttonSelect.onPressed(onSelectPressed);
  buttonIncrease.onPressed(onIncreasePressed);
  buttonDecrease.onPressed(onDecreasePressed);

  buttonIncrease.onPressedFor(longPressDuration, onLongPressIncrease);
  buttonDecrease.onPressedFor(longPressDuration, onLongPressDecrease);
}

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
  }
  display.display();

  // Fast increment or decrement
  if (fastIncrement) {
    if (menuState == EDIT_TIME) {
      increaseTime();
    } else if (menuState == EDIT_DATE) {
      increaseDate();
    }
    delay(100); // Adjust delay as needed for speed
  }

  if (fastDecrement) {
    if (menuState == EDIT_TIME) {
      decreaseTime();
    } else if (menuState == EDIT_DATE) {
      decreaseDate();
    }
    delay(100); // Adjust delay as needed for speed
  }
}

void showMenu() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(selectedMenuItem == 0 ? "> " : "  ");
  display.print("1. Show Time");
  display.setCursor(0, 10);
  display.print(selectedMenuItem == 1 ? "> " : "  ");
  display.print("2. Show Temp/Humid");
  display.setCursor(0, 20);
  display.print(selectedMenuItem == 2 ? "> " : "  ");
  display.print("3. Edit Time");
  display.setCursor(0, 30);
  display.print(selectedMenuItem == 3 ? "> " : "  ");
  display.print("4. Edit Date");
}

void showTime() {
  DateTime now = rtc.now();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Time: ");
  display.print(now.hour() < 10 ? "0" : "");
  display.print(now.hour());
  display.print(":");
  display.print(now.minute() < 10 ? "0" : "");
  display.print(now.minute());
  display.print(":");
  display.print(now.second() < 10 ? "0" : "");
  display.print(now.second());
  display.setCursor(0, 10);
  display.print("Date: ");
  display.print(now.day() < 10 ? "0" : "");
  display.print(now.day());
  display.print("/");
  display.print(now.month() < 10 ? "0" : "");
  display.print(now.month());
  display.print("/");
  display.print(now.year());
}

void showTempHumid() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  if (isnan(temp) || isnan(humid)) {
    display.print("Failed to read from DHT sensor!");
    return;
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.print(" *C");
  display.setCursor(0, 10);
  display.print("Humidity: ");
  display.print(humid);
  display.print(" %");
}

void onNextPressed() {
  if (menuState == SHOW_MENU) {
    selectedMenuItem = (selectedMenuItem + 1) % 4;
  } else {
    menuState = SHOW_MENU;
  }
}

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
    }
  } else {
    switch (menuState) {
      case EDIT_TIME:
        editTime();
        break;
      case EDIT_DATE:
        editDate();
        break;
      default:
        break;
    }
  }
}

void onIncreasePressed() {
  fastIncrement = false; // Stop fast increment if it was running
  switch (menuState) {
    case EDIT_TIME:
      increaseTime();
      break;
    case EDIT_DATE:
      increaseDate();
      break;
    default:
      break;
  }
}

void onDecreasePressed() {
  fastDecrement = false; // Stop fast decrement if it was running
  switch (menuState) {
    case EDIT_TIME:
      decreaseTime();
      break;
    case EDIT_DATE:
      decreaseDate();
      break;
    default:
      break;
  }
}

void onLongPressIncrease() {
  fastIncrement = true;
}

void onLongPressDecrease() {
  fastDecrement = true;
}

void editTimeMenu() {
  DateTime now = rtc.now();
  if (editHour == 0 && editMinute == 0 && editSecond == 0) {
    editHour = now.hour();
    editMinute = now.minute();
    editSecond = now.second();
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Editing Time");
  display.setCursor(0, 10);
  display.print("Time: ");
  display.print(editHour < 10 ? "0" : "");
  display.print(editHour);
  display.print(":");
  display.print(editMinute < 10 ? "0" : "");
  display.print(editMinute);
  display.print(":");
  display.print(editSecond < 10 ? "0" : "");
  display.print(editSecond);
}

void editDateMenu() {
  DateTime now = rtc.now();
  if (editYear == 0 && editMonth == 0 && editDay == 0) {
    editYear = now.year();
    editMonth = now.month();
    editDay = now.day();
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Editing Date");
  display.setCursor(0, 10);
  display.print("Date: ");
  display.print(editYear);
  display.print("-");
  display.print(editMonth < 10 ? "0" : "");
  display.print(editMonth);
  display.print("-");
  display.print(editDay < 10 ? "0" : "");
  display.print(editDay);
}

void increaseTime() {
  editSecond++;
  if (editSecond >= 60) {
    editSecond = 0;
    editMinute++;
    if (editMinute >= 60) {
      editMinute = 0;
      editHour++;
      if (editHour >= 24) {
        editHour = 0;
      }
    }
  }
}

void decreaseTime() {
  editSecond--;
  if (editSecond < 0) {
    editSecond = 59;
    editMinute--;
    if (editMinute < 0) {
      editMinute = 59;
      editHour--;
      if (editHour < 0) {
        editHour = 23;
      }
    }
  }
}

void increaseDate() {
  editDay++;
  if (editDay > 31) { // Simplified, does not account for month length
    editDay = 1;
    editMonth++;
    if (editMonth > 12) {
      editMonth = 1;
      editYear++;
    }
  }
}

void decreaseDate() {
  editDay--;
  if (editDay < 1) {
    editDay = 31; // Simplified, does not account for month length
    editMonth--;
    if (editMonth < 1) {
      editMonth = 12;
      editYear--;
    }
  }
}

void editTime() {
  // Apply the edited time
  rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), editHour, editMinute, editSecond));
  menuState = SHOW_MENU;
}

void editDate() {
  // Apply the edited date
  rtc.adjust(DateTime(editYear, editMonth, editDay, rtc.now().hour(), rtc.now().minute(), rtc.now().second()));
  menuState = SHOW_MENU;
}
