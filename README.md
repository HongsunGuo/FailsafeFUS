# Table of Contents
- [FailSafe](#FailSafe)

## FailSafe
### Features
- Sanity check the connection of the focused ultrasound equipment set-up
- Real-time monitoring of voltage data acquired from an NI-DAQ card
- Trigger an electronic relay when detecting a voltage beyond the threshold
- Save the data and logs of abnormal events

### Main window
<img src='failSafeC/Images/main%20GUI.png' width='800'>

- Stop running when detecting a high voltage. The threshold can be customized.

<img src='failSafeC/Images/main%20GUI%20wiht%20abnormal%20voltage.png' width='800'>
