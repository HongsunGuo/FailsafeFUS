# Table of Contents
- [StimuliScheduler](#StimuliScheduler)
  - [Features](#Features)
- [FailSafe](#FailSafe)


# StimuliScheduler
## Features

<img src='StimuliScheduler/StimuliScheduler/images/UNM%20diagram.png' width='600'>


## Screenshots

<img src='StimuliScheduler/StimuliScheduler/images/add.png' width='600'>

<img src='StimuliScheduler/StimuliScheduler/images/change%20US%20parameter.png' width='600'>

<img src='StimuliScheduler/StimuliScheduler/images/running.png' width='600'>

<img src='StimuliScheduler/StimuliScheduler/images/done.png' width='600'>


# FailSafe
## Features
- Sanity check the connection of the focused ultrasound equipment set-up
- Real-time monitoring of voltage data acquired from an NI-DAQ card
- Trigger an electronic relay when detecting a voltage beyond the threshold
- Save the data and logs of abnormal events

## Main window
<img src='failSafeC/Images/main%20GUI.png' width='600'>

- Stop running when detecting a high voltage. The threshold can be customized.

<img src='failSafeC/Images/main%20GUI%20wiht%20abnormal%20voltage.png' width='600'>
