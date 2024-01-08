#ifndef LAUNCHSCREEN_H
#define LAUNCHSCREEN_H

#include <QWidget>

namespace Ui {
class LaunchScreen;
}

class LaunchScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LaunchScreen(QWidget *parent = nullptr);
    ~LaunchScreen();

public slots:
    void centerOnScreen();

private:
    Ui::LaunchScreen *ui;
};

#endif // LAUNCHSCREEN_H
