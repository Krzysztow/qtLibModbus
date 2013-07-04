#ifndef MBCONNECTIONSMANAGER_H
#define MBCONNECTIONSMANAGER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class MBConnection;
class MBCommand;

class MBThreadedConnManager : public QObject
{
    Q_OBJECT

public:
    explicit MBThreadedConnManager(QObject *parent = 0);
    const QThread *managedThread();

public:
    void registerConnection(MBConnection *conn);
    void unregisterConnection(MBConnection *conn);

    void mbConnectAsync(MBConnection *conn, int invokeId);
    void closeAsync(MBConnection *conn, bool prioritize, int invokeId);

    void readBitsAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint8> *result, int invokeId);
    void readInputBitsAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint8> *result, int invokeId);
    void readRegistersAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint16> *result, int invokeId);
    void readInputRegistersAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint16> *result, int invokeId);

    void writeBitAsync(MBConnection *conn, int slaveId, int coilAddr, int status, int invokeId);
    void writeRegisterAsync(MBConnection *conn, int slaveId, int regAddr, int value, int invokeId);
    void writeBitsAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint8> *data, int invokeId);
    void writeRegistersAsync(MBConnection *conn, int slaveId, int addr, int nb, QVector<quint16> *data, int invokeId);

    void reportSlaveIdAsync(MBConnection *conn, int slaveId, QVector<quint8> *dest, int invokeId);

    void sendRawRequestAsync(MBConnection *conn, int slaveId, QVector<quint8> *req, int invokeId);
    void waitForConfirmationAsync(MBConnection *conn, int slaveId, QVector<quint8> *resp, int invokeId);

    bool cancelRequest(MBConnection *conn, int invokeId);

signals:
    void commandAdded();

private slots:
    void _threadFinished();
    void _runModbusAsync();

    void _nextCommandAdded();

private:
    void _appendCommand(MBCommand *cmd);
    void _prependCommand(MBCommand *cmd);

private:

    QThread _managedThread;
    QMutex _mutex;
    QWaitCondition _hasSomeWorkToDo;

    QList<MBCommand*> _commands;//protected by _mutex
    MBCommand *_currentCommand;//protected by _mutex

    int _managedConnectionsCnt;
};

#endif // MBCONNECTIONSMANAGER_H
