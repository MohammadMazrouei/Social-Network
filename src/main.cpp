#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


    QFile cur_user("curUser.txt");
    cur_user.open(QFile::WriteOnly | QFile::Text );
    cur_user.close();

    QFile stack_pages("stack.txt");
    stack_pages.open(QFile::WriteOnly | QFile::Text );
    stack_pages.close();

	bool log_in = true;
	QFile curUser("curUser.txt");
	curUser.open(QFile::ReadOnly | QFile::Text );
	QTextStream in(&curUser);
	if(in.readLine().isNull())
		log_in = false;
	curUser.close();

	if(log_in) {
		personal_page w;
		w.show();
		return a.exec();
	}
	else {
		MainWindow w;
		w.show();
		return a.exec();
	}
	return a.exec();
}
