#include "change_password.h"
#include "ui_change_password.h"

change_password::change_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_password)
{
    ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(addressDatabas);
    getUsername();
}

change_password::~change_password()
{
    delete ui;
}

void change_password::on_btn_cancel_clicked()
{
    setting * st = new setting();
    st->show();
    this->close();
}


void change_password::on_btn_ok_clicked()
{
    QString curPass = ui->le_currentpassword->text();
    QString newPass = ui->le_newpassword->text();
    QString CnewPass = ui->le_confirmnewpassword->text();
    QString realPass;

    if(!db.open()) {
        QMessageBox::warning(this, "Not Open Database!", "Not Successes To Open Database. \nPage : change_password -> Function : on_btn_ok_clicked");
        return;
    }
    QSqlQuery q;
    // Reading current password
    if(!q.exec("SELECT * FROM information WHERE username = '"+username +"'; ")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Checking Password. \nPage : change_password -> Function : on_btn_ok_clicked");
        db.close();
        return;
    }
    while(q.next()) {
        realPass = q.value(1).toString();
    }
    // checking if current password in correct
    if(realPass != curPass) {
        QMessageBox::warning(this, "Incorrect Password", "Your Current Password Is Incorrect!");
        db.close();
        return;
    }
    // confirm new password
    if(newPass != CnewPass) {
        QMessageBox::warning(this, "Incorrect Password", "Your New Password Is Incorrect!");
        db.close();
        return;
    }
    // check password to be valid
    if(!check_password(newPass)) {
        QMessageBox::warning(this, "Unvalid Password", "Your New Password Is Unvalid!");
        db.close();
        return;
    }
    // updating password
    if(!q.exec("UPDATE information SET password = '"+newPass +"' WHERE username = '"+username +"' ;")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : change_password -> function : on_btn_ok_clicked");
        db.close();
        return;
    }

    db.close();
    // clearing database
    {
        QFile curUser("curUser.txt");
        curUser.open(QFile::WriteOnly | QFile::Text);
        curUser.close();
    }

    MainWindow * mw = new MainWindow;
    mw->show();
    this->close();
}

void change_password::getUsername() {
    QFile curUser("curUser.txt");
    curUser.open(QFile::ReadOnly | QFile::Text );
    QTextStream in(&curUser);
    username = in.readLine();
    curUser.close();
}

bool change_password::check_password(QString password) {
    if(password.size() <= 0 || password.size() > 20)
        return false;

    return true;
}
