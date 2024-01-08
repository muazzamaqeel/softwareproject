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
#include <QWidget>


class PB_Confluence_Implemenation : public QObject {
    Q_OBJECT

public:
    explicit PB_Confluence_Implemenation(QWidget *parent = nullptr);
    parentboard* parentBoard; // Reference to the parent board
    QString Title;
    QString GroupName;
    parentboard* getParentBoard();

public slots:
    void HideCreationSection();
    void ShowCreateWindow();
    void CreateGroupFunction();
    void ShowCreateWindow_Page();
    void CreatePageFunction();
    void onTreeItemClicked(QTreeWidgetItem* item, int column);


};

#endif // PB_CONFLUENCE_IMPLEMENATION_H
