#ifndef MBCONNECTION_H
#define MBCONNECTION_H

#include "qtlibmodbus_global.h"

#include <QHostAddress>

#include "errno.h"
#include "libmodbus/modbus.h"

#include "mbconnectionsmanager.h"

class QTLIBMOBUSSHARED_EXPORT MBConnection:
        public QObject
{
    Q_OBJECT

public:
    ~MBConnection();


    enum Parity {
        ParityNone,
        ParityEven,
        ParityOdd
    };

    enum ErrorRecovery {
        ERNone,
        ERLink,
        ERProtocol
    };

    enum RequestType {
        Connect,
        Close,

        ReadBits,
        ReadInputBits,
        ReadRegisters,
        ReadInputRegisters,

        WriteBit,
        WriteBits,
        WriteRegister,
        WriteRegisters,

        ReportSlaveId
    };

    /**
     * Following are creation methods for two connection types TCP and Rtu.
     * If you want to run connections in a separate thread, to use nonblocking behaviour, pass not NULL threadedConnManager pointer
     * and then use set of *Async(...) methods.
     */
    static MBConnection *newTcpConnection(const QHostAddress &servIp, quint16 port, MBThreadedConnManager *threadedConnManager = 0);
    static MBConnection *newRtuConnection(const QString &device, int baudRate, Parity parity, int dataBitsNo, int stopBitsNo, MBThreadedConnManager *threadedConnManager = 0);


    /**
     * Following set of methods is intended for connections that were created with runInThread set true.
     * That means their invokations are to be run in a separate thread managed by @MBConnectionsManager.
     * The order requests execution is in a FIFO manner.
     *
     * Every *Async invocation result in queued Modbus command. The command can be identified with invokeId returned by *Async methods.
     * When the the given Modbus command is executed, its completion (or error) is broadcasted with emission of apprioriate signal (or @errorOccured()) with the particular invokeId.
     *
     * @note :  Keep in mind, that you are interested in connection of the signals to the slots implemented in your objects. The MBConnection would run in the other thread, thus
     *          you have to use connection of Qt::Queuedconnection type in order not to fall into any race conditions.
     */

public:
    int connectAsync() {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->mbConnectAsync(this, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int closeAsync(bool prioritize = false) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->closeAsync(this, prioritize, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int readBitsAsync(int slaveId, int addr, int nb, QVector<quint8> *result) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->readBitsAsync(this, slaveId, addr, nb, result, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int readInputBitsAsync(int slaveId, int addr, int nb, QVector<quint8> *result) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->readInputBitsAsync(this, slaveId, addr, nb, result, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int readRegistersAsync(int slaveId, int addr, int nb, QVector<quint16> *result) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->readRegistersAsync(this, slaveId, addr, nb, result, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int readInputRegistersAsync(int slaveId, int addr, int nb, QVector<quint16> *result) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->readInputRegistersAsync(this, slaveId, addr, nb, result, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int writeBitAsync(int slaveId, int coilAddr, int status) {
        ++_lastRequestId;        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->writeBitAsync(this, slaveId, coilAddr, status, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int writeBitsAsync(int slaveId, int addr, int nb, QVector<quint8> *data) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->writeBitsAsync(this, slaveId, addr, nb, data, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int writeRegisterAsync(int slaveId, int regAddr, int value) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->writeRegisterAsync(this, slaveId, regAddr, value, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int writeRegistersAsync(int slaveId, int addr, int nb, QVector<quint16> *data) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->writeRegistersAsync(this, slaveId, addr, nb, data, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int reportSlaveIdAsync(int slaveId, QVector<quint8> *dest) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->reportSlaveIdAsync(this, slaveId, dest, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int sendRawRequestAsync(int slaveId, QVector<quint8> *dest) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->reportSlaveIdAsync(this, slaveId, dest, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    int waitForConfirmationAsync(int slaveId, QVector<quint8> *resp) {
        if (0 != _threadMgr) {
            incrementReqId();
            _threadMgr->waitForConfirmationAsync(this, slaveId, resp, _lastRequestId);
            return _lastRequestId;
        }
        else
            return -1;
    }

    /**
     * Following set of methods is just a wrapper around libmobdus.
     * These methods are to be invoked on objects that were created without threadManager.
     * If you want to use connection that was meant to be run in a separate thread (created with threadManager),
     * use *Async() methods set.
     */
public:
    int setSlave(int slave) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_set_slave(_ctxt, slave);
    }

    int mbConnect() {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_connect(_ctxt);
    }

    void close() {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        modbus_close(_ctxt);
    }

    int readBits(int addr, int nb, QVector<quint8> *result) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_read_bits(_ctxt, addr, nb, result->data());
    }

    int readInputBits(int addr, int nb, QVector<quint8> *result) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_read_input_bits(_ctxt, addr, nb, result->data());
    }

    int readRegisters(int addr, int nb, QVector<quint16> *result) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_read_registers(_ctxt, addr, nb, result->data());
    }

    int readInputRegisters(int addr, int nb, QVector<quint16> *result) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_read_input_registers(_ctxt, addr, nb, result->data());
    }

    int writeBit(int coilAddr, int status) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_write_bit(_ctxt, coilAddr, status);
    }

    int writeRegister(int regAddr, int value) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_write_register(_ctxt, regAddr, value);
    }

    int writeBits(int addr, int nb, QVector<quint8> *data) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_write_bits(_ctxt, addr, nb, data->data());
    }

    int writeRegisters(int addr, int nb, QVector<quint16> *data) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_write_registers(_ctxt, addr, nb, data->data());
    }

    int reportSlaveId(QVector<quint8> *dest) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_report_slave_id(_ctxt, dest->data());
    }

    int sendRawRequest(QVector<quint8> *req) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_send_raw_request(_ctxt, req->data(), req->size());
    }

    int waitForConfirmation(QVector<quint8> *resp) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_receive_confirmation(_ctxt, resp->data());
    }

    int writeAndReadRegisters(int writeAddr, int writeNb, QVector<quint16> *wData,
                              int readAddr, int readNb, QVector<quint16> *rData) {
        Q_ASSERT((0 == _threadMgr) ? true : (QThread::currentThread() == _threadMgr->managedThread()));
        return modbus_write_and_read_registers(_ctxt, writeAddr, writeNb, wData->data(),
                                               readAddr, readNb, rData->data());
    }

public:
    int setErrorRecovery(ErrorRecovery erMode) {
        //@todo : not thread safe. This should be done only when the connection is not executing any request currelntly.
        modbus_error_recovery_mode mode;
        switch (erMode) {
        case (ERLink):
            mode = MODBUS_ERROR_RECOVERY_LINK;
            break;
        case (ERProtocol):
            mode = MODBUS_ERROR_RECOVERY_PROTOCOL;
            break;
        case (ERNone)://fall through
        default:
            mode = MODBUS_ERROR_RECOVERY_NONE;
            break;
        }

        return modbus_set_error_recovery(_ctxt, mode);
    }

    void setResponseTimeout(int timeout_ms) {
        //@todo : not thread safe. This should be done only when the connection is not executing any request currelntly.
        Q_ASSERT(timeout_ms > 0);
        timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        modbus_set_response_timeout(_ctxt, &tv);
    }

    int responseTimeout() {
        timeval tv;
        modbus_get_response_timeout(_ctxt, &tv);

        return ((tv.tv_sec * 1000) + (tv.tv_sec / 1000));
    }

    //negative value means don't care about the byte timeout
    void setByteTimeout(int timeout_ms) {
        //@todo : not thread safe. This should be done only when the connection is not executing any request currelntly.
        timeval tv;
        if (timeout_ms < 0) {
            tv.tv_sec = -1;
            tv.tv_usec = -1;
        }
        else {
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
        }

        modbus_set_byte_timeout(_ctxt, &tv);
    }

    int byteTimeout() {
        timeval tv;
        modbus_get_byte_timeout(_ctxt, &tv);

        return ((tv.tv_sec * 1000) + (tv.tv_sec / 1000));
    }

    int headerLength() {
        return modbus_get_header_length(_ctxt);
    }

    int flush() {
        return modbus_flush(_ctxt);
    }

    void setDebug(bool debug) {
        modbus_set_debug(_ctxt, debug);
    }

    static const char *errorStr(int errnum) {
        return modbus_strerror(errnum);
    }

public:
    bool cancelRequest(int invokeId) {
        if (0 != _threadMgr) {
            return _threadMgr->cancelRequest(this, invokeId);
        }
        else
            return false;
    }

signals:
    //! todo: where to use qRegisterMetaType<MBConnection::RequestType>("MBConnection::RequestType") to do it only once and within library?
    void requestFinished(int invokeId, int res, /*MBConnection::RequestType*/int type);

    void rawRequestSent(int invokeId, int res);
    void confirmationReceived(int invokeId, int res);

    void connectionChanged(int invokeId, bool isConnected);
    void errorOccured(int invokeId, int errNo);
    void requestCancelled(int invokeId);

public:
    inline void emitRequestFinished(int invokeId, int res, MBConnection::RequestType type) {
        emit requestFinished(invokeId, res, type);
    }

    inline void emitRawRequestSent(int invokeId, int res) {
        emit rawRequestSent(invokeId, res);
    }

    inline void emitConfirmationReceived(int invokeId, int res) {
        emit confirmationReceived(invokeId, res);
    }

    inline void emitConnectionChanged(int invokeId, bool isConnected) {
        emit connectionChanged(invokeId, isConnected);
    }

    inline void emitErrorOccured(int invokeId, int errNo) {
        emit errorOccured(invokeId, errNo);
    }

    inline void emitRequestCancelled(int invokeId) {
        emit requestCancelled(invokeId);
    }


private:
    MBConnection(modbus_t *ctxt, MBThreadedConnManager *threadMgr);

    inline void incrementReqId() {
        if (++_lastRequestId < 0)
            _lastRequestId = 0;
    }

    int _lastRequestId;
    modbus_t *_ctxt;

    MBThreadedConnManager *_threadMgr;
};

#endif // MBCONNECTION_H
