#include "mfm.h"
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <QMessageBox>




QByteArray cmd(QByteArray a){
    QTcpSocket socket;
    socket.connectToHost ( "192.168.10.104", 54311);
    if(socket.waitForConnected(1000)){
        socket.write(a+"\n");
        socket.disconnect();
        if(socket.waitForReadyRead(1000)){
            return socket.readAll();
        }else{
            return "unable to read";
        }
    }else{
        return "unable to connect";
    }
}


Mfm::Mfm():QWidget(),Ui::Mfm(){
    setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog);

    setStyleSheet("background:");

    setWindowFlags(windowFlags() | Qt::Dialog);
    //        setAttribute(Qt::WA_Maemo5DesktopApplet);

    Atom window_type = XInternAtom (QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    Atom hildonwinType = XInternAtom (QX11Info::display(), "_HILDON_WM_WINDOW_TYPE_HOME_APPLET", False);
    XChangeProperty(QX11Info::display(), winId(), window_type, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *) &hildonwinType, 1);

    Atom appletId = XInternAtom(QX11Info::display(), "_HILDON_APPLET_ID", False);
    Atom utf8String = XInternAtom(QX11Info::display(), "UTF8_STRING", False);
    char appletIdStr[] = "PLASMA_APPLET_THING";
    XChangeProperty(QX11Info::display(), winId(), appletId, utf8String, 8,
                    PropModeReplace, (unsigned char*)appletIdStr, strlen(appletIdStr));

    // the following adds a configure button to the widget, which will be send by some X event...
    Atom hildonAppletSettings = XInternAtom(QX11Info::display(), "_HILDON_APPLET_SETTINGS", False);
    int value = 1;
    XChangeProperty(QX11Info::display(), winId(), hildonAppletSettings, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char*)&value, 1);


    timerEvent(0);
    startTimer(4000);
}
void Mfm::timerEvent(QTimerEvent *event){
    label->setText(cmd("info %a \\n%t").replace("\\n","\n"));
}


void Mfm::paintEvent ( QPaintEvent * event ){
    QPainter painter(this);
    painter.fillRect(event->rect(),QColor ( 0,0,0,200));
}



void Mfm::on_playButton_clicked(){
    cmd("play");
}
void Mfm::on_skipButton_clicked(){
    cmd("skip");
}
void Mfm::on_loveButton_clicked(){
    cmd("love");
}
void Mfm::on_banButton_clicked(){
    if (QMessageBox::question ( this, "Really ban?","Really ban?",QMessageBox::Yes|QMessageBox::No )==QMessageBox::Yes)
        cmd("ban");
}
