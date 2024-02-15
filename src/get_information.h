#ifndef GET_INFORMATION_H
#define GET_INFORMATION_H

#include <QWidget>
#include <QSqlDatabase>
#include "mainwindow.h"
#include "recommend.h"

class info;

bool isNumber(QString str);

namespace Ui {
class get_information;
}

class get_information : public QWidget
{
	Q_OBJECT

public:
    explicit get_information(QWidget *parent = nullptr);

	QString getUsername();
	bool checkName();
	bool checkLastname();
	bool checkBio();
	bool checkEmail();
	bool checkPhonenum();
	bool checkYear();
	bool checkMonth();
	bool checkDay();
	bool checkGender();
	class info;
    bool isNumber(QString str);

	~get_information();

private slots:
	void on_btn_cancel_clicked();

	void on_btn_ok_clicked();

	void on_checkBox_male_clicked();

	void on_checkBox_female_clicked();

private:
	Ui::get_information *ui;
	QSqlDatabase db;
	QString username;
};

#endif // GET_INFORMATION_H
