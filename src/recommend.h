#ifndef RECOMMEND_H
#define RECOMMEND_H

#include <QWidget>
#include "mainwindow.h"
#include "get_information.h"
#include "personal_page.h"

namespace Ui {
class recommend;
}

class recommend : public QWidget
{
	Q_OBJECT

public:
	explicit recommend(QWidget *parent = nullptr);
	~recommend();
	void get_username();
	void show_recommend();
	void follow(int index);
	void unfollow(int index);
	bool is_public(QString account);

private slots:
	void on_btn_cancel_clicked();

	void on_btn_ok_clicked();

	void on_btn_follow1_clicked();

	void on_btn_follow2_clicked();

	void on_btn_follow3_clicked();

	void on_btn_follow4_clicked();

	void on_btn_follow5_clicked();

	void on_btn_follow6_clicked();

private:
	Ui::recommend *ui;
	QSqlDatabase db;
	QString username;
	QString usernames[7];
	bool follow_users[7] = {false};
	int number_recommend;
};

#endif // RECOMMEND_H
