#ifndef TABLEANIMATOR_H
#define TABLEANIMATOR_H
#include <QTableWidget>
#include <QPropertyAnimation>

class TableAnimator {
public:
    // Constructor
    explicit TableAnimator(QTableWidget* table);

    // Public methods for animations
    void animateRowAddition(int row);
    // You can add more animation methods here

private:
    QTableWidget* tableWidget;  // Reference to the QTableWidget
};

#endif // TABLEANIMATOR_H
