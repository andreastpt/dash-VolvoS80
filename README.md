# Dash_VolvoS80

This repo contains a Vehicle Plugin for [Dash](https://github.com/OpenDsh/dash/) to support a 2004 Volvo S80 with a D5 engine.

Current functionality:

* Use steering wheel buttons in Android Auto

TODO:

* Update lights, doors and windows on the vehicle page
* Change canbus id-s for gauges and exchange speed for boost
* Add button page for fuel door, trunk and whatever comes to mind
* Add automatic switching to reverse camera when reverse gear is engaged
* Integrate parking sensors
* Webasto heater control
* Add wheel button combos (for example pressing cruise control- and vol- changes the page in Dash)
* Send diag enable packets to enable the opening of windows, trunk, etc from dash

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
