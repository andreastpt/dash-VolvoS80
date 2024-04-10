# Dash_VolvoS80

This repo contains a Vehicle Plugin for [Dash](https://github.com/OpenDsh/dash/) to support a 2004 Volvo S80 with a D5 engine.

Current functionality:

* Use steering wheel buttons in Android Auto
* Button page for Trunk, open/close all windows (currently only front passenger window does anything lmao)
  and a gauge sweep
* Automatic switching to reverse camera when reverse gear is engaged
* Data page (speed, rpm and reverse status are currently working)

TODO:

* Change canbus id-s for gauges and exchange speed for boost
* Integrate parking sensors
* Webasto heater control (preferably with a microcontroller to enable remote webasto controll over the internet while minimizing power usage but also integrating the control panel in dash)
* Add wheel button combos (for example pressing cruise control- and vol- changes the page in Dash)
* Fix bugs on the vehicle page (only shows one door open at a time and shows it repeatedly opening/closing)
* Fix swtiching between pages (currently as long as reverse is engaged it forces the camera page)
* integrate light status in the vehicle page
* Change opendash source code to display rpm, speed etc on the gauges page instead

Layout (currently they only interface with the Android Auto Page):

* RTI dpad - acts like a dpad
* Enter and Back above dpad - Enter and Back
* NEXT and PREV - NEXT and PREV
* Call accept - PLAY/PAUSE
* Call deny - Call deny

This repo also contains a .txt file with the candump output of different canbus buttons.

For me the easiest location to tap into the low speed canbus was in the trunk from the stock RTI connector.

Based on [Dash_InfinitiG37](https://github.com/icecube45/Dash_InfinitiG37)
