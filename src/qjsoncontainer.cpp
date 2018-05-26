/***********************************************
    Copyright (C) 2014  Schutz Sacha
    This file is part of QJsonModel (https://github.com/dridk/QJsonmodel).

    QJsonModel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QJsonModel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QJsonModel.  If not, see <http://www.gnu.org/licenses/>.

**********************************************/

#include "qjsoncontainer.h"
constexpr int FIRST_COL_W = 100;

QJsonContainer::QJsonContainer(QWidget *parent):
    QWidget(parent)
{
    obj_layout=new QVBoxLayout(parent);
    obj_layout->setContentsMargins(QMargins(5,5,5,5));

    treeview_groupbox=new QGroupBox(parent);
    treeview_groupbox->setStyleSheet("QGroupBox {border:0;}");

    treeview_layout = new QVBoxLayout(treeview_groupbox);

    treeview = new QTreeView(treeview_groupbox);

    QString datastr="";
    model = new QJsonModel(parent);
    model->loadJson(datastr.toUtf8());

    treeview->setModel(model);

    currentFindIndexId=-1;

    treeview_layout->addWidget(treeview);

    treeview->setColumnWidth(0,FIRST_COL_W);
    treeview->setColumnWidth(1,60);
    treeview->setColumnWidth(2,100);

    treeview->autoScrollMargin();
    treeview->setAlternatingRowColors(true);
    treeview->setAnimated(true);
    treeview->setRootIsDecorated(true);

    QFile file(":/qss/qjsontreeview.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QString styleSheet = file.readAll();
        file.close();
        treeview->setStyleSheet(styleSheet);
    }
    treeview->ensurePolished();
    treeview->setContextMenuPolicy(Qt::CustomContextMenu);

    obj_layout->addWidget(treeview_groupbox);
    parent->setLayout(obj_layout);

    connect(treeview, SIGNAL(expanded(const QModelIndex& )),this, SLOT(on_treeview_item_expanded()));
    connect(model,SIGNAL(dataUpdated()),this,SLOT(on_model_dataUpdated()));
}

QJsonContainer::~QJsonContainer()
{
    model->deleteLater();
    treeview->deleteLater();
    treeview_layout->deleteLater();
    treeview_groupbox->deleteLater();
}

/* Move trough json items that contain text
 * Very stupid logic but it works
 * input bool value:
 *      true - forward;
 *      false - backward;
 */
void QJsonContainer::findTextJsonIndexHandler(bool direction)
{
    if(currentFindIndexesList.count()>0 && currentFindIndexId<currentFindIndexesList.count()-1 && direction)
    {
        currentFindIndexId++;
    }
    else if(currentFindIndexesList.count()>0 && currentFindIndexId>0 && currentFindIndexId<=currentFindIndexesList.count()-1 && !direction)
    {
        currentFindIndexId--;
    }

    else if(currentFindIndexesList.count()>0 && (currentFindIndexId==currentFindIndexesList.count()-1 || currentFindIndexId==0))
    {
        QRect widgetRect = treeview->visualRect(currentFindIndexesList[currentFindIndexId]);
        QToolTip::showText( treeview->mapToGlobal( QPoint(widgetRect.center().x(),widgetRect.center().y()) ), (!direction)?tr("The last match! Next item will be from the end"):tr("The last match! Next item will be from the start"),treeview,QRect(100, 200, 11, 16),3000 );
        currentFindIndexId=-1;
    }
    else if(currentFindIndexesList.count()>0 && currentFindIndexId==-1 && !direction)
    {
        currentFindIndexId=currentFindIndexesList.count()-1;
    }

    if(currentFindIndexId>=0)
    {
        treeview->setCurrentIndex(currentFindIndexesList[currentFindIndexId]);
    }
}

void QJsonContainer::loadJson(QJsonDocument data)
{
    plainJSONText = data.toJson();
    model->loadJson(plainJSONText.toUtf8());
    expandAll();
}

void QJsonContainer::loadJson(QString data)
{
    plainJSONText = data;
    model->loadJson(data.toUtf8());
    expandAll();
}

void QJsonContainer::expandAll( )
{
    if ( isExpandAll )
    {
        treeview->expandAll();
        treeview->resizeColumnToContents(0);
    }
    else
    {
        treeview->collapseAll();
        treeview->setColumnWidth(0,FIRST_COL_W);
    }
}

void QJsonContainer::on_treeview_item_expanded()
{
    if( !isExpandAll )
    {
        QTreeView *treeviewcall = (QTreeView *)sender();
        treeviewcall->resizeColumnToContents(0);
    }
}

QStringList QJsonContainer::extractStringsFromModel(QJsonModel *model, const QModelIndex &parent)
{
    QStringList retval;
    int rowCount = model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx0 = model->index(i, 0, parent);
        //        QModelIndex idx1 = model->index(i, 1, parent);
        QModelIndex idx2 = model->index(i, 2, parent);
        //        qDebug()<<idx0.data(Qt::DisplayRole).toString()<<idx1.data(Qt::DisplayRole).toString()<<idx2.data(Qt::DisplayRole).toString();
        //        qDebug()<<static_cast<QJsonTreeItem*>(idx0.internalPointer())->typeName();
        //        QJsonTreeItem *item = static_cast<QJsonTreeItem*>(idx0.internalPointer());

        if(idx0.isValid())
        {
            retval << idx0.data(Qt::DisplayRole).toString() +QString("|")+idx2.data(Qt::DisplayRole).toString();
            //qDebug()<<idx0.data(Qt::DisplayRole).toString();
            retval << extractStringsFromModel(model, idx0);
        }
    }

    return retval;
}

QList<QModelIndex> QJsonContainer::findModelText(QJsonModel *model, const QModelIndex &parent)
{
    QList<QModelIndex> retindex;

    QString stringToSearch = currentFindText;
    if( !mCaseSensitivity )
    {
        stringToSearch=stringToSearch.toLower();
    }

    int rowCount = model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx0 = model->index(i, 0, parent);
        QModelIndex idx2 = model->index(i, 2, parent);

        if(idx0.isValid())
        {
            QString itemText = QString(idx0.data(Qt::DisplayRole).toString() +
                                     QString("|")+idx2.data(Qt::DisplayRole).toString());

            if( !mCaseSensitivity)
            {
                itemText=itemText.toLower();
            }

            if(itemText.contains(stringToSearch))
            {
                retindex << idx0;
            }
            retindex<<findModelText(model, idx0);
        }
    }

    return retindex;
}

void QJsonContainer::resetCurrentFind()
{
    currentFindIndexesList.clear();
    currentFindIndexId=-1;
    currentFindText="";
}

void QJsonContainer::on_model_dataUpdated()
{
    resetCurrentFind();
}

void QJsonContainer::search( const QString& text, bool caseSensitivity, bool direction )
{
    mCaseSensitivity = caseSensitivity;

    if(currentFindText.isEmpty() || currentFindText != text )
    {
        currentFindIndexesList.clear();
        currentFindText = text;
        currentFindIndexesList = findModelText(model, QModelIndex());
        currentFindIndexId=-1;
    }
    findTextJsonIndexHandler( direction );
}
