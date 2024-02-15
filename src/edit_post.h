#ifndef EDIT_POST_H
#define EDIT_POST_H

#include <QWidget>
#include "show_post.h"

namespace Ui {
class edit_post;
}

class edit_post : public QWidget
{
	Q_OBJECT

public:
	explicit edit_post(QString username, int id, QWidget *parent = nullptr);
	~edit_post();
	void show_postt();

private slots:
	void on_btn_cancel_clicked();

	void on_btn_save_clicked();

private:
	Ui::edit_post *ui;
	QSqlDatabase db;
	QString username;
	int id;
};

#endif // EDIT_POST_H
