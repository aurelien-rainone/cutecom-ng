/**
 * \file
 * <!--
 * Copyright 2015 Develer S.r.l. (http://www.develer.com/)
 * -->
 *
 * \brief YModemTransfer class header
 *
 * \author Aurelien Rainone <aurelien@develer.com>
 */

#ifndef YMODEMTRANSFER_H
#define YMODEMTRANSFER_H

#include "filetransfer.h"

/**
 * \brief ymodem protocol implementation of FileTransfer
 */
class YModemTransfer : public FileTransfer
{
    Q_OBJECT

public:

    /**
     * \brief create a ymodem transfer thread
     * \param parent   object taking ownership
     * \param serial   opened instance of QSerialPort
     * \param filename file to transfer
     */
    YModemTransfer(QObject *parent, QSerialPort *serial, const QString &filename);

private:

    /**
     * \brief initiate an ymodem protocol file transfer
     * \return transfer end code
     */
    void performTransfer();
};

#endif // YMODEMTRANSFER_H
