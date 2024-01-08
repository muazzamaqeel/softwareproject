#ifndef PB_TEAM_IMPLEMENATION_H
#define PB_TEAM_IMPLEMENATION_H
#include <string>
#include <QString>
#include <QLineEdit>
#include "parentboard.h"
#include <QWidget>
#include <QStandardItemModel>

class pb_team_implemenation: public QObject
{
    Q_OBJECT
public:
    QStandardItemModel *model;
    pb_team_implemenation(parentboard* parentBoardInstance);
    QString GetUserFirstName(int UserId);
public slots:
    void on_teamTab_opened();
    void on_createuser_clicked();
    void UserRetrieval();
    void AddRowUser(const QString& firstNameInput, const QString& emailInput, const QString& roleInput);
private:
        parentboard* parentBoard;
};

#endif // PB_TEAM_IMPLEMENATION_H
