#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "iostream"
#include <QFileDialog>
#include <QMessageBox>
#include "stdexcept"
#include "Exceptions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    this->currentCollection = new Map<QDate, QString>{};
    ui->setupUi(this);
    connect(ui->addButton, SIGNAL(clicked()),
            this, SLOT(addRecordFromForm()));
    connect(ui->lastNameLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(listenForAddEnable()));
    connect(ui->firstNameLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(listenForAddEnable()));
    connect(ui->middleNameLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(listenForAddEnable()));
    connect(ui->courseLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(listenForAddEnable()));
    connect(ui->deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteRecord()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::listenForAddEnable() {
    if (this->ui->lastNameLine->text().isEmpty() ||
            this->ui->firstNameLine->text().isEmpty() ||
            this->ui->middleNameLine->text().isEmpty() ||
            this->ui->courseLine->text().isEmpty()) {
        this->ui->addButton->setEnabled(false);
    } else {
        this->ui->addButton->setEnabled(true);
    }
}

void MainWindow::showCollection() {
    auto* list = this->ui->listRecords;
    list->clear();
    Collection<Node<QDate, QString>> *collection = \
            this->currentCollection->ascendingOrderOfNodes();
    if (collection == nullptr) return;
    for (int index = 0; index < collection->getLength(); index++) {
        QDate date = collection->at(index).getKey();
        QString student = collection->at(index).getValue();
        QString listLine{date.toString(QString{"dd.MM.yyyy"}) + " " + student};
        list->addItem(listLine);
    }
}

void MainWindow::insertToCollection(QString lastName,
                        QString firstName,
                        QString middleName,
                        QString course,
                        QDate date) {
    if (lastName.isEmpty() ||
            firstName.isEmpty() ||
            middleName.isEmpty() ||
            course.isEmpty()) {
        throw InvalidValueError();
    }
    QString student = (lastName + " " +
                       firstName + " " +
                       middleName + " " +
                       course);
    currentCollection->insert(date, student);
}

void MainWindow::clearFields() {
    this->ui->lastNameLine->clear();
    this->ui->firstNameLine->clear();
    this->ui->middleNameLine->clear();
    this->ui->courseLine->clear();
}

void MainWindow::addRecordFromForm() {
    QString lastName = this->ui->lastNameLine->text();
    QString firstName = this->ui->firstNameLine->text();
    QString middleName = this->ui->middleNameLine->text();
    QString course = this->ui->courseLine->text();
    QDate date = this->ui->dateEdit->selectedDate();
    try {
        insertToCollection(lastName, firstName, middleName, course, date);
        showCollection();
        clearFields();
    }  catch (InvalidValueError&) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", "Неправильные данные!");
        messageBox.setFixedSize(500,200);
    }
    checkDeleteButtonEnabled();
}

QString MainWindow::getFile(bool save) {
    QString fileName = nullptr;
    if (save){
        fileName = QFileDialog::getSaveFileName(this,
                tr("Save Records"), "",
                tr("Records (*.txt);;All Files (*)"));
    } else {
        fileName = QFileDialog::getOpenFileName(this,
                tr("Load Records"), "",
                tr("Records (*.txt);;All Files (*)"));
    }
    return fileName;
}

void MainWindow::checkDeleteButtonEnabled() {
    bool enables = true;
    if (this->ui->listRecords->count() == 0 ) {
        enables = false;
    }
    this->ui->deleteButton->setEnabled(enables);
}

void MainWindow::deleteRecord() {
    auto* list = this->ui->listRecords;
    int index = list->currentIndex().row();
    QString item = list->takeItem(index)->text();
    QString key = item.split(QString{" "})[0];
    QStringList keyList = key.split(QString{"."});
    int day = keyList[0].toInt();
    int month = keyList[1].toInt();
    int year = keyList[2].toInt();
    QDate date{year, month, day};
    this->currentCollection->remove(date);
    showCollection();
    checkDeleteButtonEnabled();


}

void MainWindow::loadFromFile() {
    QString path = getFile(false);
    if (path.isEmpty()) {
        return;
    }
    this->currentCollection->loadFromFile(path.toStdString());
    showCollection();
}

void MainWindow::saveToFile() {
    QString path = getFile(true);
    if (path.isEmpty()) {
        return;
    }
    this->currentCollection->writeToFile(path.toStdString());
}

void MainWindow::on_open_triggered()
{
    loadFromFile();
}

void MainWindow::on_save_triggered()
{
    saveToFile();
}
