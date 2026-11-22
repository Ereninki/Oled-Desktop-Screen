# Oled Desk App

This is an firmware for esp32 that has shows you the time, your daily hackatime time, the date and how many days left until january 1!!

its using an api that made by me called [Oled Desk API](https://github.com/Ereninki/Oled-Desk-API)!!

![image](https://cdn.hackclub.com/01a204bf-d82b-7f44-83f3-cc464974ead6/oled-desk-app.jpeg)

# Requirements

- Esp32 (it needs to have wifi)
- Oled Screen (1.3 inc)
- 4 jumper wires (for connecting esp32 to the oled screen)
- breadboard (optional)
- micro-usb cable (for uploading code and powering esp32)

# Usage

To use this, you need to git clone this project `git clone https://github.com/Ereninki/Oled-Desk-App` and fill the **Secrets.h.example** file with the information it needs and then rename it to **Secrets.h** (btw the time api key thingy is my api's key, if you want it you can just email me eren61794@gmail.com or dm me on slack @ereninki!!). Then you just open the **oled-desk-app.ino** file with Arduino IDE and upload the code to the esp32 and boom thats it!!

# The Wiring

![image](https://cdn.hackclub.com/01a204ec-0292-7f53-b333-ce3da45a93eb/Screenshot%202026-11-22%20232110.png)
