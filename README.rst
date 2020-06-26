Indoor Sampler
==============

Sample the temperature, humidity, pressure and gas values logging readings to 
central REST API service.

This is part of the `House Weather Project <https://github.com/users/oisinmulvihill/projects/3>`_.

.. contents::

Introduction
------------

I'm reusing an old Arduino POE Ethernet board to capture temperature and other 
data. The sampler will perform a HTTP POST request to a central server on the 
LAN to handle the reading. The board will be powered without the need for 
battery or external power supply. My LAN has a POE switch I can use to support 
this. I'm working with the sensors provided by the `BME680 <https://shop.pimoroni.com/products/bme680-breakout>`_ 
breakout board from `Pimoroni <https://shop.pimoroni.com/products/bme680-breakout>`_.

I'm using `PlatformIO <https://platformio.org/>`_ as the tool/framework to allow me to build 
and test my core logic on the arduino or desktop. Its taken a while to get up
to speed on Platformio but I believe it was worth the effort. They have put a 
lot of thought into how to structure a project. They can also support TDD 
development across environments, which is how I prefer to work.

BME680 References:

- https://community.bosch-sensortec.com/t5/Knowledge-base/BME680-Sensor-Data-Interpretation/ta-p/8713
- https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors-bme680/


Environment Setup
-----------------

The library dependancies are listed are requirements in the platformio.ini.
To reset you can remove the .pio/ directory.

Development
-----------

To run tests checking the functionality you can use::

  pio test -e <environment from platform.ini>

On the host machine AKA the desktop (without arduino specific libs/headers)::

  $ pio test -e desktop

To run tests on the arduino board (Currently hanging for reasons I've not worked out)::

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
  BME680: Configuring.
  Sending report 'type=bme680&t=02473&h=040913&p=101461&g=01358087' to tarsis:8080.
  Sending report 'type=bme680&t=02464&h=040909&p=101465&g=01194097' to tarsis:8080.
  Sending report 'type=bme680&t=02448&h=040774&p=101469&g=01070569' to tarsis:8080.
  Sending report 'type=bme680&t=02438&h=040681&p=101475&g=00979457' to tarsis:8080.
  :
  etc


API Endpoint
------------

The indoor sampler will POST its results to the `Sample Server <https://github.com/oisinmulvihill/sample_server>`_
which I run on my local LAN. The sampler will set its MAC Address as the custom 
header "X-MAC".
