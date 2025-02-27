# **Guide: USB Serial Communication Between Raspberry Pi and Digispark ATtiny85**

## **Overview**
This guide explains how **bidirectional USB serial communication** was established between a **Raspberry Pi** and a **Digispark ATtiny85** using **DigiCDC**. It covers:
- **Why baud rate settings are ignored**
- **How data is sent and received**
- **How to properly flush the USB buffer**

---

## **1. Understanding Baud Rate in DigiCDC**
DigiCDC is a **USB Communication Device Class (CDC-ACM)** that emulates a **serial port**, but **does not use traditional UART baud rates**.

### **Why Baud Rate is Ignored**
- **USB controls data transfer speed, not a UART clock.**
- **DigiCDC reports 9600 baud to Linux, but this is arbitrary.**
- **Real transfer speed depends on USB Full Speed (12 Mbps max, ~1.5 Mbps effective).**
- **Changing the baud rate in Python (`12000` or `9600`) has no effect.**


## **3. Receiving from and Sending to the ATtiny**
### **How Data is Sent from the Raspberry Pi**

1. **The Raspberry Pi sends a string as bytes**:
   ```python
   ser.write(b'Hello\n')
   ```
   - `b''` ensures **raw bytes** are sent (USB expects binary format).
   - `\n` signals the end of a message for easier parsing.

2. **DigiSpark reads the incoming data**:
   ```cpp
   char c = SerialUSB.read();
   ```
   - Reads one byte from the USB buffer.
   - Messages are read **until a newline (`\n`) is received**.

3. **DigiSpark processes and responds**:
   ```cpp
   SerialUSB.println("Hello to you too!");
   ```
   - Sends a response **back to the Raspberry Pi**.

---

### **How the Raspberry Pi Reads the Response**
1. **DigiSpark sends a message:**
   ```cpp
   SerialUSB.println("Hello to you too!");
   ```
   - Queues the response in the USB outgoing buffer.

2. **The Raspberry Pi reads the response:**
   ```python
   response = ser.readline().decode().strip()
   ```
   - Reads data **until a newline (`\n`) is found**.
   - `.decode()` converts **bytes to a string**.

## **4. Flushing USB and Keeping the Connection Active**
### **Why is Flushing Necessary?**
- **USB buffers messages** before sending them.
- If the buffer is **not cleared**, **new messages may not be received.**

### **How to Flush the USB Buffer in DigiSpark**
```cpp
SerialUSB.flush();
```
This ensures **all outgoing data is sent immediately**.

### **How to Prevent USB Disconnection**
#### **Keep the Connection Open**
Instead of `echo`, use `cat` to **continuously send data**:
```bash
sudo cat > /dev/ttyACM0
```

#### **TODO Disable USB Autosuspend on Raspberry Pi**
```bash
echo -1 | sudo tee /sys/module/usbcore/parameters/autosuspend
```
This prevents the **Raspberry Pi from putting the USB device to sleep.**

#### **Use a Watchdog Timer in DigiSpark**
If the connection **becomes unresponsive**, reset the Digispark automatically:
```cpp
#include <avr/wdt.h>
wdt_enable(WDTO_8S);  // Reset if unresponsive for 8 seconds
```
