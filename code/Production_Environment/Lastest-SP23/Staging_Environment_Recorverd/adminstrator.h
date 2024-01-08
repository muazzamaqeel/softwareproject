#ifndef ADMINSTRATOR_H
#define ADMINSTRATOR_H

#include <QMainWindow>

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

private:
    Ui::adminstrator *ui;
};

#endif // ADMINSTRATOR_H
