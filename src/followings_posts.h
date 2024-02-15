#ifndef FOLLOWINGS_POSTS_H
#define FOLLOWINGS_POSTS_H

#include <QWidget>
#include "personal_page.h"
#include "other_page.h"

namespace Ui {
class Followings_posts;
}

class Followings_posts : public QWidget
{
	Q_OBJECT

public:
	explicit Followings_posts(QWidget *parent = nullptr);
	~Followings_posts();
	class post;
	void show_posts();
	void show_posts2();
	void get_username();
	bool check_like(int index);
	void list_users_liked();
	void like(int index);
	void unlike(int index);
	void add_to_stack(QString page_name);

private slots:
	void on_btn_mypage_clicked();

	void on_btn_like1_clicked();

	void on_btn_like2_clicked();

	void on_btn_like3_clicked();

	void on_btn_like4_clicked();

	void on_btn_like5_clicked();

	void on_btn_like6_clicked();

	void on_btn_seepage1_clicked();

	void on_btn_seepage2_clicked();

	void on_btn_seepage3_clicked();

	void on_btn_seepage4_clicked();

	void on_btn_seepage5_clicked();

	void on_btn_seepage6_clicked();

	void on_btn_next_clicked();

	void on_btn_pervious_clicked();

private:
	Ui::Followings_posts *ui;
	QSqlDatabase db;
	QString username;
	QList<post> posts;
	QVector<bool> liked;
	QList<QString> followings;
	int first_post_index;
};

#endif // FOLLOWINGS_POSTS_H
