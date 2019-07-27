# TimeBot

The file telegram_bot_ntp.ino is a code to the Wemos D1, that connects to an WiFi network and allow the control of the time of activation of 4 relays, by sending coded messages to a Telegram's bot. In the messages one can configure the interval of time which the relays will be turned on, using the NTP protocol. This configurations are saved in the EEPROM of the Wemos, persisting if the hardware is turned off. This project was originally developed to control the time of irrigation of a garden, but can be used in many other aplications where the time of activation of some device need to be controlled by the user from distance. 

## Creating a Telegram Bot:
https://core.telegram.org/bots.

## Needed alterations in source-code:
#### Line 7: token from your bot
#### Line 17: your WiFi network
#### Line 18: password of your network
#### Line 198: your Telegram's ID

## Needed external libraries:
Already in the repository. It's necessary install in the Wemos' libraries folder.
#### NTPClient
#### ESP8266WiFi
#### UniversalTelegramBot

## Pins: 
D5, D6, D7 e D8.

## Messages format: 
Example: RELE1 00:00 23:59

Where, 
1 -> relay number (1 to 4)
00:00 -> begin time (will be turned on after this time)
23:59 -> end time (will be turned off after this time)
