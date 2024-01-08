#ifndef PB_CONFLUENCE_IMPLEMENATION_H
#define PB_CONFLUENCE_IMPLEMENATION_H

#include <QObject>

class PB_Confluence_Implemenation : public QObject
{
    Q_OBJECT

public:
    PB_Confluence_Implemenation();

public slots:
    void backbutton();
};

#endif // PB_CONFLUENCE_IMPLEMENATION_H
