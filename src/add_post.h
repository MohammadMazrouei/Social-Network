#ifndef ADD_POST_H
#define ADD_POST_H

#include <QWidget>
#include <QSqlDatabase>
#include "personal_page.h"

namespace Ui {
class add_post;
}

class add_post : public QWidget
{
	Q_OBJECT

public:
	explicit add_post(QWidget *parent = nullptr);
	~add_post();
	bool check_post();
	void get_username();

private slots:
	void on_btn_ok_clicked();

	void on_btn_cancel_clicked();

	void on_btn_clear_clicked();

private:
	Ui::add_post *ui;
	QSqlDatabase db;
	QString username;
};

#endif // ADD_POST_H
