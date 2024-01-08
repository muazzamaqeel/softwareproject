#ifndef ADMINSTRATOR_H
#define ADMINSTRATOR_H

#include "qlineedit.h"
#include "qtextedit.h"
#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class adminstrator;
}

class adminstrator : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminstrator(QWidget *parent = nullptr);

    ~adminstrator();


public slots:
    void previousWindow();
    void projectSelection();
    void storeInputValues();
    void checkCredentials();


private:
    Ui::adminstrator *ui;
};

#endif // ADMINSTRATOR_H
