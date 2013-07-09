qtLibModbus
===========

Qt wrapper over the libmodubs library

Note: So far libmodbus is used not as a library but some files are pulled out of it. It won't be hard to modify it.
Note: config.h file was created for my particular desktop. If libmodbus is used as a library, this note will be invalid too.

This wrapper makes libmodbus more Qt friendly. It provides:
- simple wrapper for synchronous modbus requests;
- asynchronous (non-blocking main thread) requests execution. They are processed in an MBConnectionManager thread in a FIFO manner. 
