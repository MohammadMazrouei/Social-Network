#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "get_information.h"
#include "personal_page.h"
#include "admin_control.h"
#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QtDebug>

#ifdef __linux__
    #define addressDatabas "/home/mohammad/Documents/University/Projects/social_network/Social-Network/src/database.sqlite"
#elif _WIN32
	#define addressDatabas "D:/Blue Folder/Project/1 Big Projects/Social Network/database.sqlite"
#endif

#define username_admin "mohammad"
#define password_admin "123456"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	bool check_username();
	bool check_password();

private slots:
	void on_btn_login_clicked();

	void on_btn_signup_clicked();

private:
	Ui::MainWindow *ui;
	QSqlDatabase db;
};
#endif // MAINWINDOW_H
