#ifndef QJSONCONTAINER_H
#define QJSONCONTAINER_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QToolTip>
#include <QToolBar>

#include "qjsonmodel.h"

class QJsonContainer : public QWidget
{
    Q_OBJECT
public:
    explicit QJsonContainer(QWidget *parent=0);
    ~QJsonContainer();

    QStringList extractStringsFromModel(QJsonModel *model, const QModelIndex &parent);

    void loadJson(QJsonDocument data);
    void loadJson(QString data);

private:
    QJsonModel *model;
    QTreeView *treeview;
    QPlainTextEdit *viewjson_plaintext;
    QCheckBox *expandAll_Checkbox;

    QVBoxLayout *treeview_layout;
    QGroupBox *treeview_groupbox;
    QVBoxLayout *obj_layout;
    QLineEdit *find_lineEdit;
    QGridLayout* tools_layout;
    QToolBar *toolbar;
    QWidget *spacer;
    QPushButton *showjson_pushbutton;
    QToolButton *findNext_toolbutton;
    QToolButton *findPrevious_toolbutton;
    QToolButton *findCaseSensitivity_toolbutton;

    //variables to handle serach nodes in tree
    QList<QModelIndex> currentFindIndexesList;
    QString currentFindText;
    int currentFindIndexId;

    QList<QModelIndex> findModelText(QJsonModel *model, const QModelIndex &parent);
    void resetCurrentFind();
    void findTextJsonIndexHandler(bool direction);
    QTreeView* getTreeView() { return treeview; }
    QJsonModel* getJsonModel() { return model; }

private slots:
    void on_expandAll_checkbox_marked();
    void on_treeview_item_expanded();
    void on_showjson_pushbutton_clicked();
    void on_findNext_toolbutton_clicked();
    void on_findPrevious_toolbutton_clicked();
    void on_findCaseSensitivity_toolbutton_clicked();

    void on_find_lineEdit_textChanged(QString text);
    void on_model_dataUpdated();

public slots:
    void findText();

};

#endif // QJSONCONTAINER_H
