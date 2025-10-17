# 3.5 inch RPi Waveshare LCD (B)

Documentation on the LCD Display can be found [here](https://www.waveshare.com/wiki/3.5inch_RPi_LCD_%28B%29).

The LCD requires specific rasbian versions for compatibility depending on the RasPi model. 

As we are using Raspberry Pi Model 3Bs, we install a custom bookworm image with the drivers preinstalled, this can be found [here](https://www.waveshare.com/wiki/3.5inch_RPi_LCD_%28B%29#Image).

Finally we change `/boot/firmware/config.txt` file and set the display version to be version (B).
