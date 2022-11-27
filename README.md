## ESP32 PING PONG

### Required hardware
- ESP32
- SSD1306 display
- KY-023 joystick

### Hardware Connection
```
   ESP Board               OLED LCD (I2C)
+--------------+          +--------------+
|            5V+----------+VCC           |
|              |          |              |
|           GND+----------+GND           |
|              |          |              |
|        GPIO17+----------+SCL           |
|              |          |              |
|        GPIO16+----------+SDA           |
|              |          +--------------+
|              |              JOYSTICK
|              |          +--------------+
|           GND+----------+GND           |
|              |          |              |
|          3.3V+----------+5V            |
|              |          |              |
|        GPIO39+----------+VRx           |
|              |          |              |
|        GPIO36+----------+VRy           |
|              |          |              |
|        GPIO34+----------+SW            |
+--------------+          +--------------+
```

### Build and Flash
Run next command from the root directory
```
idf.py build flash monitor
```