#ifndef SEARCH_H
#define SEARCH_H

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



namespace Ui {
class search;
}

class search : public QWidget
{
	Q_OBJECT

public:
	explicit search(QWidget *parent = nullptr);
	~search();
	void add_to_stack(QString page_name);
	void setData();
	QString getUsername();
	class user;
	void getData(QSqlQuery q);
	void follow(int index);
	void unfollow(int index);
    void Sort();
	bool followed_username(QString useraname);
	bool is_public(QString account);
	void suggestion();


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

	void on_pte_user1_selectionChanged();

	void on_pte_user2_selectionChanged();

	void on_pte_user3_selectionChanged();

	void on_pte_user4_selectionChanged();

	void on_pte_user5_selectionChanged();

	void on_pte_user6_selectionChanged();

private:
	Ui::search *ui;
	QList<user> users;
	QString username_visitor;
	int start = 0;
	QSqlDatabase db;
};

#endif // SEARCH_H
