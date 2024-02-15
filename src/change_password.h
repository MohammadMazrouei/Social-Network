#ifndef CHANGE_PASSWORD_H
#define CHANGE_PASSWORD_H

#include <QDialog>
#include "setting.h"

namespace Ui {
class change_password;
}

class change_password : public QDialog
{
    Q_OBJECT

public:
    explicit change_password(QWidget *parent = nullptr);
    void getUsername();
    bool check_password(QString password);
    ~change_password();

private slots:
    void on_btn_cancel_clicked();

    void on_btn_ok_clicked();

private:
    Ui::change_password *ui;
    QSqlDatabase db;
    QString username;
};

#endif // CHANGE_PASSWORD_H
