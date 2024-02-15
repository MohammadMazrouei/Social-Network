#ifndef OTHER_PAGE_H
#define OTHER_PAGE_H

#include <QWidget>
#include "personal_page.h"
#include "show_followers.h"
#include "show_followings.h"

namespace Ui {
class other_page;
}

class other_page : public QWidget
{
	Q_OBJECT

public:
	explicit other_page(QString username, QWidget *parent = nullptr);
	~other_page();
	class post;
	void show_information();
	void show_posts();
	void show_posts2();
	void show_number_followings_followers();
	bool check_like(int index);
	void list_users_liked();
	void like(int index);
	void unlike(int index);
	void get_username();
	void set_followed();
	void follow();
	void unfollow();
	void add_to_stack(QString page_name);
	bool is_public(QString account);

private slots:
	void on_btn_personalpage_clicked();

	void on_btn_followers_clicked();

	void on_btn_followings_clicked();

	void on_btn_like1_clicked();

	void on_btn_like2_clicked();

	void on_btn_like3_clicked();

	void on_btn_like4_clicked();

	void on_btn_like5_clicked();

	void on_btn_like6_clicked();

	void on_btn_follow_clicked();

	void on_btn_back_clicked();

private:
	Ui::other_page *ui;
	QString username_owner, username_visitor;
	QSqlDatabase db;
	QList<post> posts;
	QVector<bool> liked;
	int first_post_index;
	bool followed = false;
};

#endif // OTHER_PAGE_H
