#ifndef SHOW_POST_H
#define SHOW_POST_H

#include <QWidget>
#include <QDebug>
#include "personal_page.h"
#include "edit_post.h"

namespace Ui {
class show_post;
}

class show_post : public QWidget
{
	Q_OBJECT

public:
	explicit show_post(QString username, int id, QWidget *parent = nullptr);
	~show_post();
	void show_postt();

private slots:
	void on_btn_back_clicked();

	void on_btn_edit_clicked();

	void on_btn_delete_clicked();

private:
	Ui::show_post *ui;
	QSqlDatabase db;
	QString username;
	int id;
};

#endif // SHOW_POST_H
