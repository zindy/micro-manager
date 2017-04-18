## Synopsis

This device adapter makes it possible to use any USB serial -> TTL converters directly as TTL shutters.
As long as the RTS or DTR line are exposed, no microcontroller is actually required to make this work.

## Wiring
Basically, the adapter is able to control the two output lines of any serial output (RTS or DTR).
Connect those to a TTL input and you have yourself a shutter!

There are two ways to wire the cable. You can solder a BNC connector onto either
  * centre pin: RTS or DTR line (whichever is available)
  * casing to ground

or
  * centre pin: +5V
  * casing to RTS or DTR

If you don't own a crimping tool, some BNC connectors come with screw terminals.
Or in a pinch, you can splice two cables together.

Both ways are taken into account in the adapter, by toggling the "Logic" variable (either positive or inverted).
You need to find a wiring that mostly keeps your output low during normal operation of your Operating System.

The line RTS or DTR can also be chosen during the adapter hardware setup.

## Adapter code

Not something I'm entirely happy with, but the code is different whether it is compiled on Windows or Linux.
This is handled with ```#ifdef WIN32``` statemements. I noticed that the same approach was used in SerialManager.cpp,
so there may not me any ways around it. The serial communication code and file descriptor / file handle *are* different.

*Hack alert!* In the ```SerialShutter::Initialize()``` method, the first thing that is done is to shutdown the
SerialManager and take direct control of the port.

Then the most barebone code is used to toggle the control lines when needed.

If this gets accepted in the main tree, I can write about various
converters on the MM wiki.

## Tests

I have now been able to test my code with a PL2303hx, a FTDI FT232RL and a CH340g adapter in both Windows and Linux, in both
Micromanager 1.4 and 2.0

**TODO** I will publish detailed wiring examples for each.

## Contributions

This adapter is based on the FreeSerial code by Karl Hoover.

The Windows code to toggle the control lines is from http://stackoverflow.com/questions/18539104

And the linux code from http://stackoverflow.com/questions/27673344

Additional fixes were made possible by looking at the SerialManager.cpp code.

## License

The orginal code was released under BSD license. So is this one.
