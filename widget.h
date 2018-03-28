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
    QString searchText = "";
    //Новый рецепт
    QString nameRec, desRec, ingRec, logRec, countryRec, typeRec;

private slots:

    void on_addButton_clicked();

    void on_delButton_clicked();

    void searchName(QString text);

    void on_listView_clicked(const QModelIndex &index);

    void on_backButton_clicked();

    void on_saveButton_clicked();

    void on_nameOfNewRecipe_textEdited(const QString &arg1);

    void on_typeOfNewRecipe_textEdited(const QString &arg1);

    void on_countryOfNewRecipe_textEdited(const QString &arg1);

    void on_testNo_clicked();

    void on_testYes_clicked();

private:
    Ui::Widget *ui;
    QSqlDatabase sdb;
    QSqlTableModel *model;
    QSqlQuery query;
    QStringListModel recModel;
    QStringList recipes;
};

#endif // WIDGET_H
