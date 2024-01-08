#include "launchscreen.h"
#include "ui_launchscreen.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPixmap>
#include <QScreen>
#include <QApplication>

// include header for the window that is opened after the animation
#include "mainwindow.h"

#include <QDebug>  // testing

LaunchScreen::LaunchScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LaunchScreen)
{
    ui->setupUi(this);

    // Remove title bar and frame
    setWindowFlags(Qt::FramelessWindowHint);

    // Center the window on the screen
    centerOnScreen();

    //qDebug() << "Window centered on screen.";

    //    // Create a QLabel with a logo (replace "logo.png" with your image file)
    //    QLabel *logoLabel = new QLabel(this);
    //    logoLabel->setPixmap(QPixmap("logo.png"));
    //    logoLabel->setAlignment(Qt::AlignCenter);
    //    logoLabel->setGeometry(0, 0, 200, 200); // Adjust dimensions as needed

    //qDebug() << "Logo label created.";

    // Create a QGraphicsOpacityEffect for fading
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);

    //qDebug() << "Opacity effect created.";

    // Create a QPropertyAnimation for fading in and out
    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity", this);
    animation->setDuration(2000);  // Set the duration of the fade effect in milliseconds
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);

    //qDebug() << "Animation created.";

    // Connect the animation finished signal to open the main window
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
        // Open the main window after the fade-in animation finishes
        // Replace MainWindow with the name of your main window class
        //qDebug() << "LaunchScreen: Animation finished. Opening MainWindow.";
        MainWindow *mainWindow = new MainWindow;
        hide();
        mainWindow->show();

        ui->~LaunchScreen();
    });

    //qDebug() << "Animation signal connected.";

    // Start the animation
    //qDebug() << "Starting animation.";
    animation->start();
}

void LaunchScreen::centerOnScreen()
{
    // Center the window on the screen
    QScreen *screen = QApplication::primaryScreen();
    QRect mainScreenGeometry = screen->availableGeometry();
    move(mainScreenGeometry.center() - rect().center());

    //qDebug() << "Centering on screen.";
}

LaunchScreen::~LaunchScreen()
{
    delete ui;
}
