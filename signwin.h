#ifndef SIGNWIN_H
#define SIGNWIN_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
namespace Ui {
class SignWin;
}

class SignWin : public QWidget
{
    Q_OBJECT

public:
    explicit SignWin(QWidget *parent = nullptr);
    ~SignWin();
public:
    QString getusername() const{
        return this->username;
    }
    QString getpasswd() const{
        return this->passwd;
    }
signals:
    void confirm(QString,QString);
    void backlog();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::SignWin *ui;
    QString username;
    QString passwd;
};

#endif // SIGNWIN_H
