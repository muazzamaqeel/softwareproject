#ifndef PARENTBOARD_H
#define PARENTBOARD_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidget> // Include QTableWidget header

namespace Ui {
class parentboard;
}

class parentboard : public QWidget
{
    Q_OBJECT

public:
    explicit parentboard(QWidget *parent = nullptr);
    ~parentboard();

private slots:
    void on_taskboardButton_clicked();
    void on_backlogButton_clicked();
    void on_sprintsButton_clicked();
    void on_confluenceButton_clicked();
    void on_createtask_sprint_clicked();
    void goBackToMainWindow();
    void addTask(const QString& taskName, const QString& description); // Renamed slot

private:
    Ui::parentboard *ui;
    QStandardItemModel *taskModel;
};

#endif // PARENTBOARD_H
