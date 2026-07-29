# p5-watchface

A watchface based on the UI from Persona 5/Persona 5 Royal. It features:
- Caldendar display with animated weather icons based on local weather
- Calling Card-style time font
- Background from Persona 5 Day Advance screen, which switches between day and night versions based on local sunrise/sunset times

## Assets
Fonts: 
* Time: https://www.fontspace.com/poland-canned-into-kaito-font-f163520
* Temperature: https://github.com/uswds/public-sans

Persona 5/Persona 5 Royal Assets:
* https://github.com/Mive82/Persona-5-Calendar
* https://mega.nz/folder/zlZxXLqS#sDgtK16KSdoMmB-hOwrQfg

BG images edited for Pebble Time 2 displays using the Pebble Color Optimizer: https://czmanix.github.io/pebble-color-optimizer/

No generative AI was used in the making of this watchface.

## Roadmap
- More animation settings (e.g. using accelerometer to detect looking at watchface, when double-tapped/backlight is on, etc.)
- Proper handling of high-latitude cases for sunrise/sunset background setting
- Improve legibility of Current Time (contrast could be improved, especially with daytime BG)
- More user control for "time of day" (morning, afternoon, etc.) setting
- General asset cleanup (lots of jaggies from the basic scaling I did)
