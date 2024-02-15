#ifndef PERSONAL_PAGE_H
#define PERSONAL_PAGE_H

#include <QWidget>
#include <QList>
#include "add_post.h"
#include "show_followers.h"
#include "show_followings.h"
#include "setting.h"
#include "search.h"
#include "show_post.h"
#include "followings_posts.h"
#include "explorer.h"

#define push_button_css_like "QPushButton {color:#1f1137 ;background-color:#ff0000;} QPushButton:hover {color:#ffffff; background-color:#8b0000;}"
#define push_button_css_unlike "QPushButton {color:#1f1137; background-color:#8a02b2;} QPushButton:hover {color: #ffffff; background-color:#341d5c;}"
#define push_button_css_follow "QPushButton {color:#1f1137; background-color:#8a02b2;} QPushButton:hover {color: #ffffff; background-color:#341d5c;}"
#define push_button_css_unfollow "QPushButton {color:#1f1137 ;background-color:#39e75f;} QPushButton:hover {color:#ffffff; background-color:#136207;}"

namespace Ui {
class personal_page;
}

class personal_page : public QWidget
{
	Q_OBJECT

public:
	explicit personal_page(QWidget *parent = nullptr);
	~personal_page();
	void get_username();
	void show_posts();
	void show_posts2();
	void show_number_followings_followers();
	void show_information();
	bool check_like(int index);
	void list_users_liked();
	void like(int index);
	void unlike(int index);
	void set_stack();
	class post;

private slots:
	void on_btn_addpost_clicked();

	void on_btn_followers_clicked();

	void on_btn_followings_clicked();

	void on_btn_setting_clicked();

	void on_btn_search_clicked();

	void on_pte_post1_selectionChanged();

	void on_btn_followingsposts_clicked();

	void on_btn_explorer_clicked();

	void on_pte_post2_selectionChanged();

	void on_pte_post4_selectionChanged();

	void on_pte_post3_selectionChanged();

	void on_pte_post5_selectionChanged();

	void on_pte_post6_selectionChanged();

	void on_btn_next_clicked();

	void on_btn_pervious_clicked();

	void on_btn_like1_clicked();

	void on_btn_like2_clicked();

	void on_btn_like3_clicked();

	void on_btn_like4_clicked();

	void on_btn_like5_clicked();

	void on_btn_like6_clicked();

private:
	Ui::personal_page *ui;
	QSqlDatabase db;
	QString username;
	QList<post> posts;
	QVector<bool> liked;
	int first_post_index;
};

#endif // PERSONAL_PAGE_H
