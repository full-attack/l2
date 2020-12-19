#include "mainwindow.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <QApplication>
#include <QMessageBox>


void testMapConstructor() {
    Map<QString, QString> map{};
    assert(map.getLength() == 0);
}

void testInsertion() {
    Map<QString, QString> map{};
    map.insert(QString{"new key"}, QString{"new value"});
    assert(map.getLength() == 1);
    map.insert(QString{"new key"}, QString{"another value"});
    assert(map.getLength() == 1);
}

void testChecking() {
    Map<QString, QString> map{};
    map.insert(QString{"new key"}, QString{"new value"});
    assert(map.check(QString{"new key"}) == true);
}

void testBracketsOperator() {
    Map<QString, QString> map{};
    map.insert(QString{"new key"}, QString{"new value"});
    assert(map[QString{"new key"}] == QString{"new value"});
}

void testDeletion() {
    Map<QString, QString> map{};
    map.insert(QString{"new key"}, QString{"new value"});
    assert(map.getLength() == 1);
    map.remove(QString{"new key"});
    assert(map.getLength() == 0);
    map.insert(QString{"new key"}, QString{"new value"});
    assert(map.getLength() == 1);
    map.remove(QString{"another key"});
    assert(map.getLength() == 1);
}

void testCheckAfterDeletion() {
    Map<QString, QString> map{};
    QString key{"new key"};
    QString newKey{"new key 2"};
    map.insert(key, QString{"new value"});
    map.insert(newKey, QString{"new value"});
    assert(map.check(key) == true);
    map.remove(key);
    assert(map.check(key) == false);
}

void testMapClear() {
    Map<QString, QString> map{};
    map.insert(QString{"new key"}, QString{"new value"});
    map.clean();
    Map<QString, QString> newMap{};
    assert(map == newMap);
}

void testMapCopy() {
    Map<QString, QString> map{};
    map.insert(QString{"new key 1"}, QString{"new value"});
    map.insert(QString{"new key 2"}, QString{"new value"});
    map.insert(QString{"new key 0"}, QString{"new value"});
    Map<QString, QString> newMap{map};
    assert(map == newMap);
    newMap.insert(QString{"new key 2"}, QString{"another value"});
    assert(!(map == newMap));
}

int main(int argc, char *argv[])
{
    testMapConstructor();
    testInsertion();
    testChecking();
    testBracketsOperator();
    testDeletion();
    testCheckAfterDeletion();
    testMapClear();
    testMapCopy();
    QApplication a(argc, argv);
    QMessageBox messageBox;
    messageBox.information(0,"Tests", "Tests passed!");
    messageBox.setFixedSize(700,400);
    MainWindow w;
    w.show();
    return a.exec();
}
