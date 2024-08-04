## Description
Developed by AN, CAC Bangalore

Code developed on behalf of Automotive BU

This code consists of a static library of MAX25432 which is linked to the MAX32660 I2C driver.


## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analog-devices-msdk.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

(None - this project builds as a standard example)

## Required Connections

-   Connect a USB cable between the PC and the USB connector.
-   Open an terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
-   You must connect P0.08 to P0.02 (SCL) and P0.09 to P0.03 (SCL) , ALERT PIN of MAX25432 to P0.11 of MAX32660, GND to GND,
Jumper settings on MAX25432: You will need drivers for I2C, GPIO read, and timer interrupt (1ms).

For the EVKIT, we short HVEN to VCC (J4), J34 to L, VDD and VCC to 3.3V. You would also need to connect SCL, SDA, ALERTB, and GND to your MCU (see following picture for your reference).

## Expected Output
```
Chargers mobile phones, laptops,.... 
Connect to MAX77958EVKIT sink and try to call get_src_command and the MAX25432 will respond to it!
```