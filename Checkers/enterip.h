#ifndef ENTERIP_H
#define ENTERIP_H

#include <QDialog>

namespace Ui {
class EnterIP;
}

class EnterIP : public QDialog
{
    Q_OBJECT

public:
    explicit EnterIP(QWidget *parent = nullptr);
    ~EnterIP();

private:
    Ui::EnterIP *ui;
};

#endif // ENTERIP_H
