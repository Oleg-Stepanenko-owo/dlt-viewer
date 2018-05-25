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

QJsonContainer::QJsonContainer(QWidget *parent):
    QWidget(parent)
{
    qDebug()<<"obj_layout parent";
    obj_layout=new QVBoxLayout(parent);
    obj_layout->setContentsMargins(QMargins(5,5,5,5));


    qDebug()<<"treeview_groupbox parent";
    treeview_groupbox=new QGroupBox(parent);
    treeview_groupbox->setStyleSheet("QGroupBox {  border:0;}");

    qDebug()<<"treeview_layout treeview_groupbox";
    treeview_layout = new QVBoxLayout(treeview_groupbox);

    qDebug()<<"treeview_layout treeview_groupbox";
    treeview = new QTreeView(treeview_groupbox);
    viewjson_plaintext=new QPlainTextEdit(treeview_groupbox);
    viewjson_plaintext->setVisible(false);

    QPalette p = viewjson_plaintext->palette();
    p.setColor(QPalette::Highlight, QColor(Qt::blue));
    p.setColor(QPalette::HighlightedText, QColor(Qt::lightGray));
    viewjson_plaintext->setPalette(p);



    showjson_pushbutton=new QPushButton(treeview_groupbox);
    showjson_pushbutton->setText(tr("Show Json Text"));
    showjson_pushbutton->setCheckable(true);

    QString datastr="";
    model = new QJsonModel(parent);
    model->loadJson(datastr.toUtf8());

    treeview->setModel(model);

    toolbar=new QToolBar();
    //toolbar->setGeometry(0,21,509,458);
    toolbar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    toolbar->setMovable(true);
    toolbar->setFloatable(true);
    toolbar->setAllowedAreas(Qt::AllToolBarAreas);

    qDebug()<<"expandAll_Checkbox treeview_groupbox";
    expandAll_Checkbox = new QCheckBox(toolbar);
    expandAll_Checkbox->setText(QString(tr("Expand")));

    tools_layout=new QGridLayout(toolbar);
    tools_layout->setContentsMargins(0,0,0,0);
    find_lineEdit=new QLineEdit();
    find_lineEdit->setPlaceholderText(tr("Serach for..."));
    find_lineEdit->setToolTip(tr("Enter text and press enter to search"));
    find_lineEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    find_lineEdit->setFocus();
    findNext_toolbutton=new QToolButton(toolbar);
    findNext_toolbutton->setText(">>");
    findNext_toolbutton->setToolTip("Find Next");
    findPrevious_toolbutton=new QToolButton(toolbar);
    findPrevious_toolbutton->setText("<<");
    findPrevious_toolbutton->setToolTip("Find Previous");
    findCaseSensitivity_toolbutton=new QToolButton(toolbar);
    findCaseSensitivity_toolbutton->setCheckable(true);
    findCaseSensitivity_toolbutton->setIcon(QIcon(QPixmap(":/icons/png/casesensitivity.png")));
    findCaseSensitivity_toolbutton->setToolTip("Check to make case sensetive");

    toolbar->addWidget(expandAll_Checkbox);
    toolbar->addSeparator();
    toolbar->addWidget(find_lineEdit);
    toolbar->addWidget(findPrevious_toolbutton);
    toolbar->addWidget(findNext_toolbutton);
    toolbar->addWidget(findCaseSensitivity_toolbutton);
    toolbar->addSeparator();

    spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    tools_layout->addWidget(toolbar,0,0);

    currentFindText=find_lineEdit->text();
    currentFindIndexId=-1;

    treeview_layout->addLayout(tools_layout);
    treeview_layout->addWidget(treeview);
    treeview_layout->addWidget(viewjson_plaintext);
    treeview_layout->addWidget(showjson_pushbutton);

    treeview->resizeColumnToContents(0);
    treeview->resizeColumnToContents(1);
    treeview->resizeColumnToContents(2);
    //treeview->setColumnWidth(2,300);
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
    connect(expandAll_Checkbox, SIGNAL(stateChanged(int)), this, SLOT(on_expandAll_checkbox_marked()));
    connect(showjson_pushbutton,SIGNAL(clicked()),this,SLOT(on_showjson_pushbutton_clicked()));
    connect(find_lineEdit,SIGNAL(returnPressed()),this,SLOT(findText()));
    connect(find_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(on_find_lineEdit_textChanged(QString)));
    connect(findNext_toolbutton,SIGNAL(clicked()),this,SLOT(on_findNext_toolbutton_clicked()));
    connect(findPrevious_toolbutton,SIGNAL(clicked()),this,SLOT(on_findPrevious_toolbutton_clicked()));
    connect(findCaseSensitivity_toolbutton,SIGNAL(clicked()),this,SLOT(on_findCaseSensitivity_toolbutton_clicked()));
    connect(model,SIGNAL(dataUpdated()),this,SLOT(on_model_dataUpdated()));
}

QJsonContainer::~QJsonContainer()
{
    model->deleteLater();
    treeview->deleteLater();
    expandAll_Checkbox->deleteLater();
    treeview_layout->deleteLater();
    treeview_groupbox->deleteLater();
}

void QJsonContainer::findText()
{
    if(viewjson_plaintext->isVisible())
    {
        if(!findCaseSensitivity_toolbutton->isChecked())
        {
            viewjson_plaintext->find(find_lineEdit->text());
        }
        else
        {
            viewjson_plaintext->find(find_lineEdit->text(),QTextDocument::FindCaseSensitively);
        }
    }
    else
    {
        //works all values to stringlist
        //QStringList strings = extractStringsFromModel(model, QModelIndex());
        //QModelIndex idx=treeview->currentIndex();
        //QStringList strings = extractStringsFromModel(model, idx);

        //qDebug()<<"##################"<<strings;
        findTextJsonIndexHandler(true);

    }
}

void QJsonContainer::on_findNext_toolbutton_clicked()
{
    findText();
}

void QJsonContainer::on_findPrevious_toolbutton_clicked()
{
    if(viewjson_plaintext->isVisible())
    {

        if(!findCaseSensitivity_toolbutton->isChecked())
        {
            viewjson_plaintext->find(find_lineEdit->text(),QTextDocument::FindBackward);
        }
        else
        {
            viewjson_plaintext->find(find_lineEdit->text(),QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
        }
    }
    else
    {
        findTextJsonIndexHandler(false);
    }
}

void QJsonContainer::on_findCaseSensitivity_toolbutton_clicked()
{
    currentFindIndexesList.clear();
    currentFindText=find_lineEdit->text();
    qDebug() << currentFindText;
    currentFindIndexesList=findModelText(model, QModelIndex());
    currentFindIndexId=-1;
}

/* Move trough json items that contain text
 * Very stupid logic but it works
 * input bool value:
 *      true - forward;
 *      false - backward;
 *
 *  TODO: make search cursor moving start from selection
 */
void QJsonContainer::findTextJsonIndexHandler(bool direction)
{

    if(currentFindText.isEmpty() || currentFindText!=find_lineEdit->text())
    {
        currentFindIndexesList.clear();
        currentFindText=find_lineEdit->text();
        currentFindIndexesList=findModelText(model, QModelIndex());
        currentFindIndexId=-1;
    }

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

    if(currentFindIndexesList.count()==0)
    {
        QRect widgetRect = find_lineEdit->contentsRect();
        QToolTip::showText( find_lineEdit->mapToGlobal( QPoint(widgetRect.center().x(),widgetRect.center().y()) ), tr("<b><font \"color\"=red>Text Not Found!</font></b>"),0,QRect(100, 200, 11, 16),3000 );
    }
    if(currentFindIndexId>=0)
    {
        treeview->setCurrentIndex(currentFindIndexesList[currentFindIndexId]);
    }
}

void QJsonContainer::on_showjson_pushbutton_clicked()
{
    if(viewjson_plaintext->isVisible())
    {
        viewjson_plaintext->setVisible(false);
        treeview->setVisible(true);
        loadJson(viewjson_plaintext->toPlainText());
        showjson_pushbutton->setText(tr("Show Json Text"));
    }
    else
    {
        viewjson_plaintext->setVisible(true);
        treeview->setVisible(false);
        showjson_pushbutton->setText(tr("Show Json View"));
    }
}

void QJsonContainer::loadJson(QJsonDocument data)
{
    QString datastr=data.toJson();
    viewjson_plaintext->setPlainText((QJsonDocument::fromJson(datastr.toUtf8())).toJson(QJsonDocument::Indented));
    model->loadJson(datastr.toUtf8());
    on_expandAll_checkbox_marked();
    //treeview->setColumnWidth(2,300);
}

void QJsonContainer::loadJson(QString data)
{
    viewjson_plaintext->setPlainText((QJsonDocument::fromJson(data.toUtf8())).toJson(QJsonDocument::Indented));
    model->loadJson(data.toUtf8());
    on_expandAll_checkbox_marked();
    //treeview->setColumnWidth(2,300);
}

void QJsonContainer::on_expandAll_checkbox_marked()
{
    if (expandAll_Checkbox->isChecked())
    {
        treeview->expandAll();
        treeview->resizeColumnToContents(0);
        treeview->resizeColumnToContents(1);
        treeview->resizeColumnToContents(2);
        //treeview->setColumnWidth(2,300);
    }
    else
    {
        treeview->collapseAll();
    }
}

void QJsonContainer::on_treeview_item_expanded()
{
    //qDebug()<<"expanded";
    if(!expandAll_Checkbox->isChecked())
    {
        QTreeView *treeviewcall = (QTreeView *)sender();
        treeviewcall->resizeColumnToContents(0);
        treeviewcall->resizeColumnToContents(1);
        treeviewcall->resizeColumnToContents(2);
        //treeviewcall->setColumnWidth(2,300);
    }
}

QStringList QJsonContainer::extractStringsFromModel(QJsonModel *model, const QModelIndex &parent)
{
    QStringList retval;


    int rowCount = model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx0 = model->index(i, 0, parent);
        QModelIndex idx1 = model->index(i, 1, parent);
        QModelIndex idx2 = model->index(i, 2, parent);
        qDebug()<<idx0.data(Qt::DisplayRole).toString()<<idx1.data(Qt::DisplayRole).toString()<<idx2.data(Qt::DisplayRole).toString();
        qDebug()<<static_cast<QJsonTreeItem*>(idx0.internalPointer())->typeName();
        //QJsonTreeItem *item = static_cast<QJsonTreeItem*>(idx0.internalPointer());

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

    QString stringToSearch=find_lineEdit->text();
    if(!findCaseSensitivity_toolbutton->isChecked())
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
            QString itemText=QString(idx0.data(Qt::DisplayRole).toString() +QString("|")+idx2.data(Qt::DisplayRole).toString());

            if(!findCaseSensitivity_toolbutton->isChecked())
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

void QJsonContainer::on_find_lineEdit_textChanged(QString /*text*/)
{
    resetCurrentFind();

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
