#ifndef DIALOGADDNODE_H
#define DIALOGADDNODE_H

#include <QDialog>

namespace Ui {
class DialogAddNode;
}

class DialogAddNode : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddNode(QWidget *parent = 0);
    ~DialogAddNode();
    int NodeAddr;
    int NodeType;
private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogAddNode *ui;
};

#endif // DIALOGADDNODE_H
