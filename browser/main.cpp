/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "browserapplication.h"
#include "webview.h"
#include "browsermainwindow.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/kd.h>

#define TERM_TTY_SETTING_MODE	0
#define TERM_TTY_SETTING_KEYBOARD	1

char term_tty_settings[2];

void term_get_prev_setting();
void term_restoring_lazy();

int main(int argc, char **argv)
{
    term_get_prev_setting();

    Q_INIT_RESOURCE(data);
    BrowserApplication application(argc, argv);
    if (!application.isTheOnlyBrowser() || !application.isCorrectlyInitialized())
        return 0;
    application.newMainWindow();
    application.exec();

    return 0;
}

void term_get_prev_setting()
{
    int term_tty_fd = open("/dev/tty1", O_RDWR);

    ioctl(term_tty_fd, KDGETMODE, &term_tty_settings[TERM_TTY_SETTING_MODE]);
    ioctl(term_tty_fd, KDGETMODE, &term_tty_settings[TERM_TTY_SETTING_KEYBOARD]);
    close(term_tty_fd);

    atexit(term_restoring_lazy);
}

void term_restoring_lazy()
{
    int term_tty_fd = open("/dev/tty1", O_RDWR);
    const char term_tty_reset_msg[] = "\033[9;15]\033[?33h\033h\?25h\033[?0c";

    ioctl(term_tty_fd, KDGETMODE, &term_tty_settings[TERM_TTY_SETTING_MODE]);
    ioctl(term_tty_fd, KDGETMODE, &term_tty_settings[TERM_TTY_SETTING_KEYBOARD]);

    write(term_tty_fd, term_tty_reset_msg, sizeof(term_tty_reset_msg));

    close(term_tty_fd);
}
