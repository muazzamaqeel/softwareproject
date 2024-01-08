#include "TableAnimator.h"
#include <QPalette>

TableAnimator::TableAnimator(QTableWidget* table) : tableWidget(table) {}

void TableAnimator::animateRowAddition(int row) {
    if(row < 0 || row >= tableWidget->rowCount()) return;

    // Create a custom widget to place in the table cell
    QWidget* widget = new QWidget();
    widget->setStyleSheet("background-color: white;"); // Set initial background color using stylesheet
    tableWidget->setCellWidget(row, 0, widget);

    // Animate the background color of the widget
    QPropertyAnimation* animation = new QPropertyAnimation(widget, "styleSheet");
    animation->setDuration(1000);
    animation->setStartValue("background-color: white;");
    animation->setEndValue("background-color: yellow;");
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
