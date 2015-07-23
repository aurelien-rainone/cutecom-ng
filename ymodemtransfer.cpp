/**
 * \file
 * <!--
 * Copyright 2015 Develer S.r.l. (http://www.develer.com/)
 * -->
 *
 * \brief YModemTransfer class implementation
 *
 * \author Aurelien Rainone <aurelien@develer.com>
 */

#include <QtSerialPort>

#include "ymodemtransfer.h"
#include "fymodem.h"

void qdebug(const char *str)
{
   qDebug() << str;
}


extern QSerialPort *g_serial;

extern qint64 _total_bytes;

extern qint64 _byte_sent;

static YModemTransfer *_g_transfer = 0;

/**
 * \brief global variable representing the last transfer percentage emitted
 */
static int _last_progress = 0;

/**
 * \brief flag indicating that, from now on, bytes sent to the serial port
 * represent the file data. (used for progress reporting)
 */
bool _transfer_started;


#include <QDebug>
/**
 * \brief _inbyte consume 1 byte from serial port
 * \param timeout timeout in miliseconds
 * \return consumed byte or -1 in case of error
 */
int32_t _getchar(unsigned short timeout)
{
    char c = 0;

    if (g_serial->isOpen())
    {
        // blocking
        if (g_serial->waitForReadyRead(timeout))
            if (g_serial->getChar(&c))
            {
                QByteArray dbg(1, c);
                qDebug() << "_getchar(0x" << dbg.toHex() << ") : " << (isprint(c)? c : ' ');
                return (int32_t)c;
            }
    }
    return -1;
}

/**
 * \brief _outbyte write 1 byte to the serial port
 * \param c byte to write
 */
void _putchar(char c)
{
    QByteArray data(1, (char)c);
    if (g_serial->isOpen())
    {
        if (g_serial->write(data) > 0)
        {
            qDebug() << "_putchar(0x" << data.toHex() << ") : " << (isprint(c)? c : ' ');
            ++_byte_sent;
        }
        else
        {
            // we wrote nothing
        }
#if 1

        // commented original code out because, as ymodem sends more bytes than the file itself
        // because of filesize and filename also sent, the progress would report
        // 100M before the end, leading to various problems
        // this problem can be corrected in various ways :
        // - do not autoclose the progress dialog
        // - stop a 99% and give the 100% only when last byte is sent (that mean knowing in _putchar that its the case)
        // - make the _byte_sent increase only when a 'file' byte is sent, not an 'extra protocol byte'

        // but for now we will just report progress the first time, so that the dialog knows that the transfer started
        // update progress
        int cur_progress = 100 * _byte_sent / _total_bytes;
        if (cur_progress > _last_progress)
        {
            // emit transferProgressed if we progressed of at least 1%
            _last_progress = cur_progress;
            emit _g_transfer->transferProgressed(cur_progress);


            // THEN NEVER EMIT AGAIN
            _last_progress = 10000000;
        }

        // org code
#else
        // update progress
        int cur_progress = 100 * _byte_sent / _total_bytes;
        if (cur_progress > _last_progress)
        {
            // emit transferProgressed if we progressed of at least 1%
            _last_progress = cur_progress;
            emit _g_transfer->transferProgressed(cur_progress);
        }
#endif
    }
}

void _sleep(int miliseconds)
{
    QThread::currentThread()->msleep(miliseconds);
}

void _flush()
{
    g_serial->flush();
}

YModemTransfer::YModemTransfer(QObject *parent, QSerialPort *serial, const QString &filename)
    : FileTransfer(parent, serial, filename)
{
    quit_requested = false;
    _byte_sent = 0;
    _last_progress = 0;
    _g_transfer = this;
}

void YModemTransfer::performTransfer()
{
    // set global QSerialPort pointer used by _inbyte/_outbyte
    g_serial = serial;
    _total_bytes = this->total_size;

    TransferError ret;

//    // call ymodem transmission routine
//    int errcode = xmodemTransmit((unsigned char*)buffer.data(), buffer.size(), &quit_requested);


    Q_ASSERT_X(false, "ESSAYER AUSSI AVEC UN VRAI FICHIER;, ET VERIFIER CE QUE L'OBN ENVOIE");

    // qu'est ce que c'est que ces 0x01 0x00 0xFF  avant le nomdu fichier??

    /*
        0x01 -> 1024 bytes packets

mais les deux suivants seraient :

  // write seq numbers
  _putchar(block_nbr & 0xFF);
  _putchar(~block_nbr & 0xFF);

  est-ce correct?

  est ce 0x38 apres le nom du fichier, est-ce normal?

  car ensuite on n'envoie que des 0 alors qu'appaeremment le receiver s'attend a autre chose


    */

    char testbuffer[] = "ABCDEFGH";

    // call ymodem transmission routine
    int32_t errcode = fymodem_send((uint8_t*)testbuffer, strlen(testbuffer),
        "testfile.test");

    if (errcode == 0)
        ret = UnknownError;
    else if (errcode > 0)
        ret = NoError;

    emit transferEnded(ret);
}
