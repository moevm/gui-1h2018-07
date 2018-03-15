#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    QSqlDatabase sdb;
    QSqlTableModel *model;
    QSqlQuery query;
};

#endif // WIDGET_H
