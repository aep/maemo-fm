#include "ui_mfm.h"
#include <QTcpSocket>

class Mfm : public QWidget,private Ui::Mfm{
    Q_OBJECT;
 public:
    Mfm();
 protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void timerEvent(QTimerEvent *event);
 private:
    QTcpSocket * socket;
 private slots:
    void on_playButton_clicked();
    void on_skipButton_clicked();
    void on_loveButton_clicked();
    void on_banButton_clicked();
};

