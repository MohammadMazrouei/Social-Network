#ifndef CHANGE_PROFILE_H
#define CHANGE_PROFILE_H

#include <QWidget>
#include "setting.h"

namespace Ui {
class change_profile;
}

class change_profile : public QWidget
{
	Q_OBJECT

public:
	explicit change_profile(QWidget *parent = nullptr);

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
    QString getUsername();
    bool isNumber(QString str);
    void setData();

	~change_profile();

private slots:
	void on_btn_cancel_clicked();

	void on_btn_ok_clicked();

	void on_checkBox_male_clicked();

	void on_checkBox_female_clicked();

private:
	Ui::change_profile *ui;
	QSqlDatabase db;
	QString username;
};

#endif // CHANGE_PROFILE_H
