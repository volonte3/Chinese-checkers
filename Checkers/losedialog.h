#ifndef LOSEDIALOG_H
#define LOSEDIALOG_H

#include <QDialog>

namespace Ui {
class LoseDialog;
}

class LoseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoseDialog(QWidget *parent = nullptr);
    ~LoseDialog();

signals:
    void loseback();

private:
    Ui::LoseDialog *ui;
};

#endif // LOSEDIALOG_H
