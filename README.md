# Arduino ATSHA204 Library
This repo contains a library designed to simplify working with the Atmel ATSHA204 chips.

The library supports the 1-Wire or I2C chips.

# Arduino Due Compatibility
This library does not support the SWI chips with the Due.  If you are using the Due, you will need to use the I2C chips.

# What is the Atmel ATSHA204?
The Atmel ATSHA204 is an optimized authentication chip that includes a 4.5Kb EEPROM. This array can be used for storage of keys, miscellaneous read/write, read-only, password or secret data, and consumption tracking. Access to the various sections of memory can be restricted in a variety of ways and then the configuration locked to prevent changes.

Each ATSHA204 ships with a guaranteed unique 72-bit serial number. Using the cryptographic protocols supported by the chip, a host system or remote server can prove that the serial number is both authentic and not a copy. The ATSHA204 can generate high-quality random numbers and employ them for any purpose, including usage as part of the crypto protocols of this chip.

# Licensing
All documentation and graphical contained on this website and within the sources unless otherwise noted is licensed under the 
[Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)](http://creativecommons.org/licenses/by-sa/3.0/deed.en_US) license.

Source code is licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.html) unless otherwise specified.