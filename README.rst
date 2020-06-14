Indoor Sampler
==============

Sample the temperature logging readings to central REST API service.

.. contents::

Introduction
------------

I'm reusing an old Arduino POE Ethernet board to capture temperature and other 
data. The sampler will perform a HTTP POST request to a central server on the 
LAN to handle the reading. The board will be powered without the need for 
battery or external power supply. My LAN has a POE switch I can use to support 
this. I'm working with a DHT11 sensor but I will move to BME680 when I get a 
chance.

I'm using https://platformio.org/ as the tool/framework to allow me to build 
and test my core logic on the arduino or desktop. Its taken a while to get up
to speed on Platformio but I believe it was worth the effort. They have put a 
lot of thought into how to structure a project. They can also support TDD 
development across environments, which is how I prefer to work.

Environment Setup
-----------------

From a fresh check-out or after removing the .pio/ directory::

  $ pio lib install "Ethernet"
  $ pio lib install "DHT sensor library for ESPx"

Development
-----------

To run tests checking the functionality you can use::

  pio test -e <environment from platform.ini>

On the host machine AKA the desktop (without arduino specific libs/headers)::

  $ pio test -e desktop

To run tests on the arduino board::

  $ pio test -e arduino

To build and release code to the board::

  # Upload and run the project (don't forget the -t upload)
  $ pio run -e arduino -t upload

Have at the serial output::

  # Monitor and display the serial output (if any)
  $ pio device monitor
  :
  --- Miniterm on /dev/cu.usbserial-00000000  9600,8,N,1 ---
  --- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
  Initialize Ethernet with DHCP.
  DHCP assigned IP:
  192.168.1.135
  Sending report 't=27.20&h=40.00&d=12.44' to tarsis:8080.
  Sending report 't=27.20&h=41.00&d=12.82' to tarsis:8080.
  Sending report 't=27.30&h=40.00&d=12.53' to tarsis:8080.
  Sending report 't=27.30&h=40.00&d=12.53' to tarsis:8080.
  :
  etc