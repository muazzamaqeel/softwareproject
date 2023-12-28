#ifndef PB_CONFLUENCE_IMPLEMENATION_H
#define PB_CONFLUENCE_IMPLEMENATION_H

#include <QObject>
#include "parentboard.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidget>
#include "databasemanager.h"
#include "parentboard.h"
#include <QSqlRelationalTableModel>
#include <QMap>

class PB_Confluence_Implemenation : public QObject {
    Q_OBJECT

public:
    PB_Confluence_Implemenation(parentboard* parentBoardInstance);
    parentboard* parentBoard; // Reference to the parent board
    QString Title;
    QString Descrption;
    QString Group;

public slots:
    void HideCreationSection();
    void ShowCreateWindow();
    void CreatePageFunction();
    void ConfluenceTableChange_Dectection(QTreeWidgetItem *item, int column);
};

#endif // PB_CONFLUENCE_IMPLEMENATION_H
