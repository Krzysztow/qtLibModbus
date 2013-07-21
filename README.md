qtLibModbus
===========

Qt wrapper over the libmodubs library

This wrapper makes libmodbus more Qt friendly. It provides:
- simple wrapper for synchronous modbus requests;
- asynchronous (non-blocking main thread) requests execution. They are processed in an MBConnectionManager thread in a FIFO manner. 

If you use libmodbus and it's installed in a standard location (Linux, so file in /usr/lib and headers in /usr/include/modbus) no configuration is needed.
Otherwise create mbconfig.pri (see _mbconfig.pri) and put it in qtLibModbus folder or above it. 
