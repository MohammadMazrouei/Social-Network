#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "personal_page.h"
#include "change_profile.h"
#include "change_password.h"
#include "mainwindow.h"
#include "messages.h"

class userInfo {
public:
	QString name;
	QString lastname;
	QString email;
	QString phonenum;
	QString bio;
	QString birthday;
	int gender;
};

namespace Ui {
class setting;
}

class setting : public QWidget
{
	Q_OBJECT

public:
	explicit setting(QWidget *parent = nullptr);

	QString getUsername();

	void show_information();

	~setting();

private slots:
	void on_btn_back_clicked();

	void on_btn_edit_clicked();

	void on_btn_logout_clicked();

	void on_btn_exit_clicked();

    void on_btn_deleteaccount_clicked();

    void on_btn_publicprivate_clicked();

    void on_btn_changepassword_clicked();

	void on_btn_messages_clicked();

private:
	Ui::setting *ui;
	QSqlDatabase db;
};

#endif // SETTING_H
