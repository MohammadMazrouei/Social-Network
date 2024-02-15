#ifndef SHOW_FOLLOWERS_H
#define SHOW_FOLLOWERS_H

#include <QWidget>
#include "personal_page.h"

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QtDebug>

class follower
{
public:
	QString id;
	QString username = "";
	QString name = "";
	QString last_name = "";
};

namespace Ui {
class show_followers;
}

class show_followers : public QWidget
{
	Q_OBJECT

public:
    explicit show_followers(QString username, QWidget *parent = nullptr);
	~show_followers();
	void setData();
	QString getUsername();
	void readData(QString username);
	void getData(QSqlQuery q);
	void follow(int index);
	void unfollow(int index);
	void add_to_satck(QString page_name);
	bool followed_username(QString username);
	bool is_public(QString account);

private slots:
	void on_btn_back_clicked();

	void on_btn_search_clicked();

	void on_btn_refresh_clicked();

	void on_btn_down_clicked();

	void on_btn_up_clicked();

	void on_btn_follow1_clicked();

	void on_btn_follow2_clicked();

	void on_btn_follow3_clicked();

	void on_btn_follow4_clicked();

	void on_btn_follow5_clicked();

	void on_btn_follow6_clicked();

	void on_btn_follow7_clicked();

	void on_btn_follow8_clicked();

	void on_btn_follow9_clicked();

	void on_btn_follow10_clicked();

	void on_pte_follower1_selectionChanged();

	void on_pte_follower2_selectionChanged();

	void on_pte_follower3_selectionChanged();

	void on_pte_follower4_selectionChanged();

	void on_pte_follower5_selectionChanged();

	void on_pte_follower6_selectionChanged();

	void on_pte_follower7_selectionChanged();

	void on_pte_follower8_selectionChanged();

	void on_pte_follower9_selectionChanged();

	void on_pte_follower10_selectionChanged();

private:
	Ui::show_followers *ui;
	QString username_owner, username_visitor;
	int start = 0;
	QList<follower> followers;
	QSqlDatabase db;
};

#endif // SHOW_FOLLOWERS_H
