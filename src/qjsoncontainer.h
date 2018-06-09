#ifndef QJSONCONTAINER_H
#define QJSONCONTAINER_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QToolTip>
#include "qjsonmodel.h"

class QJsonContainer : public QWidget
{
    Q_OBJECT
public:
    explicit QJsonContainer(QWidget *parent=0);
    ~QJsonContainer();

    QStringList extractStringsFromModel(QJsonModel *model, const QModelIndex &parent);

    bool loadJson(QJsonDocument data);
    bool loadJson(QString data);
    QString getJSONText() { return plainJSONText; }
    void search( const QString& text, bool caseSensitivity = false, bool direction = true );
    void resetCurrentFind();
    void expandAll( );
    void setExpandAll( bool val ){ if( isExpandAll != val ){ isExpandAll=val; expandAll(); } }
    void setRegExp( bool val ){ isRegExp = val; }
    QVariant getValue( const QString& key ) { return model->getValue( key ); }

private:
    bool isCaseSensitivity{false};
    bool isExpandAll{false};
    bool isRegExp{false};
    QJsonModel *model;
    QTreeView *treeview;

    QVBoxLayout *treeview_layout;
    QGroupBox *treeview_groupbox;
    QVBoxLayout *obj_layout;
    QWidget *spacer;

    //variables to handle serach nodes in tree
    QList<QModelIndex> currentFindIndexesList;
    QString currentFindText;
    QString plainJSONText;
    int currentFindIndexId;

    QList<QModelIndex> findModelText(QJsonModel *model, const QModelIndex &parent);
    void findTextJsonIndexHandler(bool direction);
    QTreeView* getTreeView() { return treeview; }
    QJsonModel* getJsonModel() { return model; }

private slots:
    void on_treeview_item_expanded();
    void on_model_dataUpdated();
};

#endif // QJSONCONTAINER_H
