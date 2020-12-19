#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Map.h"
#include "QDate"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Map<QDate, QString>* currentCollection;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addRecordFromForm();

    void listenForAddEnable();

    QString getFile(bool save=false);

    void loadFromFile();

    void saveToFile();

    void insertToCollection(QString lastName,
                            QString firstName,
                            QString middleName,
                            QString course,
                            QDate date);

    void showCollection();

    void checkDeleteButtonEnabled();

    void deleteRecord();

    void clearFields();

    void on_open_triggered();

    void on_save_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
