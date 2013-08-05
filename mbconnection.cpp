#include "mbconnection.h"

MBConnection *MBConnection::newTcpConnection(const QHostAddress &servIp, quint16 port, MBThreadedConnManager *threadedConnManager)
{
    modbus_t *ctx = modbus_new_tcp(servIp.toString().toLatin1(), port);
    return new MBConnection(ctx, threadedConnManager);
}

MBConnection *MBConnection::newRtuConnection(const QString &device, int baudRate, MBConnection::Parity parity, int dataBitsNo, int stopBitsNo, MBThreadedConnManager *threadedConnManager)
{
    char parChar;

    switch (parity) {
    case (ParityNone): {
        parChar = 'N';
        break;
    }
    case (ParityEven): {
        parChar = 'E';
        break;
    }
    case (ParityOdd):
    default:
        parChar = 'O';
    }

    modbus_t *ctxt = modbus_new_rtu(device.toLatin1(), baudRate, parChar, dataBitsNo, stopBitsNo);
    return new MBConnection(ctxt, threadedConnManager);
}

MBConnection::MBConnection(modbus_t *ctxt, MBThreadedConnManager *threadMgr):
    _lastRequestId(0),
    _ctxt(ctxt),
    _threadMgr(threadMgr)
{
    if (0 != _threadMgr) {
        _threadMgr->registerConnection(this);
    }
}

MBConnection::~MBConnection()
{
    if (0 != _threadMgr) {
        _threadMgr->unregisterConnection(this);
    }
    modbus_free(_ctxt);
    _ctxt = 0;
}
