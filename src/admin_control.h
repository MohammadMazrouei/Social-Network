#ifndef ADMIN_CONTROL_H
#define ADMIN_CONTROL_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class admin_control;
}

class admin_control : public QWidget
{
	Q_OBJECT

public:
	explicit admin_control(QWidget *parent = nullptr);
	~admin_control();

private slots:
	void on_btn_logout_clicked();

private:
	Ui::admin_control *ui;
};

#endif // ADMIN_CONTROL_H
