#ifndef PROJECTCREATION_H
#define PROJECTCREATION_H

#include <QMainWindow>

namespace Ui {
class ProjectCreation;
}

class ProjectCreation : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectCreation(QWidget *parent = nullptr);
    ~ProjectCreation();

public slots:
    void openProjectsAdmin();

private:
    Ui::ProjectCreation *ui;



};



#endif // PROJECTCREATION_H
