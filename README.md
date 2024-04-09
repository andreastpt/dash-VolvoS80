# Dash_VolvoS80

This repo contains a Vehicle Plugin for [Dash](https://github.com/OpenDsh/dash/) to support a 2004 Volvo S80 with a D5 engine.

Current functionality:

* Use steering wheel buttons in Android Auto
* Button page for Trunk, open/close all windows (currently only front passenger window does anything lmao)
and a gauge sweep

TODO:

* Change canbus id-s for gauges and exchange speed for boost
* Automatic switching to reverse camera when reverse gear is engaged
* Integrate parking sensors
* Integrate media buttons (seek and prev/next from the stock radio)
* Discover possibilities with PHM (what codes does the phm keypad send out if any at all and how hard would it be to hijack its audio signal to use it as a phone with android auto)
* Discover possibilities with the stock radio (display song progress, name and such on the screen)
* Webasto heater control (preferably with a microcontroller to enable remote webasto controll over the internet while minimizing power usage but also integrating the control panel in dash)
* Add wheel button combos (for example pressing cruise control- and vol- changes the page in Dash)

Layout (currently they only interface with the Android Auto Page):

* RTI dpad - acts like a dpad
* Enter and Back above dpad - Enter and Back
* NEXT and PREV - NEXT and PREV
* Call accept - PLAY/PAUSE
* Call deny - Call deny

This repo also contains a .txt file with the candump output of different canbus buttons.

It is likely possible this plugin works with other P2 Volvos but my expirience says no. I've seen so many different canbus id-s for the SWM.

For me the easiest location to tap into the low speed canbus was in the trunk from the stock RTI connector.

Based on [Dash_InfinitiG37](https://github.com/icecube45/Dash_InfinitiG37)
