/**
 * \file
 * <!--
 * Copyright 2015 Develer S.r.l. (http://www.develer.com/)
 * -->
 *
 * \brief XModemTransfer class implementation
 *
 * \author Aurelien Rainone <aurelien@develer.com>
 */

#include <QtSerialPort>

#include "xmodemtransfer.h"
#include "xmodem.h"

extern QSerialPort *g_serial;

extern qint64 _total_bytes;

extern qint64 _byte_sent;

static XModemTransfer *_g_transfer = 0;

/**
 * \brief global variable representing the last transfer percentage emitted
 */
int _last_progress = 0;

/**
 * \brief _inbyte consume 1 byte from serial port
 * \param timeout timeout in miliseconds
 * \return consumed byte or -1 in case of error
 */
int _inbyte(unsigned short timeout)
{
    char c = 0;

    if (g_serial->isOpen())
    {
        // blocking
        if (g_serial->waitForReadyRead(timeout))
            if (g_serial->getChar(&c))
                return c;
    }
    return -1;
}

/**
 * \brief _outbyte write 1 byte to the serial port
 * \param c byte to write
 */
void _outbyte(int c)
{
    QByteArray data(1, (char)c);
    if (g_serial->isOpen())
    {
        g_serial->write(data);
        ++_byte_sent;

        // update progress
        int cur_progress = 100 * _byte_sent / _total_bytes;
        if (cur_progress > _last_progress)
        {
            // emit transferProgressed if we progressed of at least 1%
            _last_progress = cur_progress;
            emit _g_transfer->transferProgressed(cur_progress);
        }
    }
}

XModemTransfer::XModemTransfer(QObject *parent, QSerialPort *serial, const QString &filename)
    : FileTransfer(parent, serial, filename)
{
    quit_requested = false;
    _byte_sent = 0;
    _last_progress = 0;
    _g_transfer = this;
}

void XModemTransfer::performTransfer()
{
    // set global QSerialPort pointer used by _inbyte/_outbyte
    g_serial = serial;
    _total_bytes = this->total_size;

    TransferError ret;

    // call xmodem transmission routine
    int errcode = xmodemTransmit((unsigned char*)buffer.data(), buffer.size(), &quit_requested);

    switch (errcode)
    {
        case -6:
            ret = LocalCancelledError;
            break;
        case -5:
            ret = UnknownError;
            break;
        case -4:
            ret = TransmissionError;
            break;
        case -2:
            ret = NoSyncError;
            break;
        case -1:
            ret = RemoteCancelledError;
            break;
        default:
            if (errcode >= 0)
                // success
                ret = NoError;
            else
                // shouldn't be here
                ret = UnknownError;
            break;
    }

    emit transferEnded(ret);
}

