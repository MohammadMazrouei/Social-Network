#ifndef MESSAGES_H
#define MESSAGES_H

#include <QWidget>
#include "setting.h"

namespace Ui {
class messages;
}

class messages : public QWidget
{
	Q_OBJECT

public:
	explicit messages(QWidget *parent = nullptr);
	~messages();
	void get_username();
	void show_messages();
	void show_messages2();
    void follow(int index);
    void delete_message(int index);

private slots:
	void on_btn_back_clicked();

	void on_btn_accept1_clicked();

	void on_btn_accept2_clicked();

	void on_btn_accept3_clicked();

	void on_btn_accept4_clicked();

	void on_btn_accept5_clicked();

	void on_btn_accept6_clicked();

	void on_btn_notaccept1_clicked();

	void on_btn_notaccept2_clicked();

	void on_btn_notaccept3_clicked();

	void on_btn_notaccept4_clicked();

	void on_btn_notaccept5_clicked();

	void on_btn_notaccept6_clicked();

	void on_btn_up_clicked();

	void on_btn_down_clicked();

private:
	Ui::messages *ui;
	QSqlDatabase db;
	QString username;
	QList<QString> my_messages;
	QList<QString> usernames;
	int first_post_index;
};

#endif // MESSAGES_H
