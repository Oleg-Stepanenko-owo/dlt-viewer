#include "parse_json.h"
#include "ui_parse_json.h"

Parse_JSON::Parse_JSON(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::Parse_JSON)
  , parseJson( nullptr )

{
    ui->setupUi(this);
    parseJson = std::unique_ptr<QJsonContainer>( new  QJsonContainer(ui->tab_parse) );
}

Parse_JSON::~Parse_JSON()
{
    delete ui;
}

void Parse_JSON::loadJson( const QString& val )
{
    if( parseJson )
    {
        parseJson->loadJson(val);
        ui->plainTextEdit->setPlainText( (QJsonDocument::fromJson(parseJson.get()->
                                                                  getJSONText().toUtf8())).toJson(QJsonDocument::Indented) );
    }
}

void Parse_JSON::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}

void Parse_JSON::on_leSearch_textChanged(const QString& /*arg1*/)
{
    parseJson->resetCurrentFind();
}

void Parse_JSON::on_leSearch_returnPressed()
{
    if( ui->leSearch->text().isEmpty() ) return;

    if( !ui->tab_json_text->isVisible() )
    {
        parseJson->search( ui->leSearch->text(), ui->tbCaseSens->isChecked() );
    }
    else
    {
        if( !ui->tbCaseSens->isChecked() )
        {
            ui->plainTextEdit->find(ui->leSearch->text());
        }
        else
        {
            ui->plainTextEdit->find(ui->leSearch->text(),QTextDocument::FindCaseSensitively);
        }
    }
}

void Parse_JSON::on_pbSearchNext_clicked()
{
    on_leSearch_returnPressed();
}

void Parse_JSON::on_pbSearchPrev_clicked()
{
    if( !ui->tab_json_text->isVisible() )
    {
        parseJson->search( ui->leSearch->text(), ui->tbCaseSens->isChecked(), false );
    }
    else
    {
        if( !ui->tbCaseSens->isChecked() )
        {
            ui->plainTextEdit->find(ui->leSearch->text(), QTextDocument::FindBackward );
        }
        else
        {
            ui->plainTextEdit->find(ui->leSearch->text(),QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
        }
    }
}

void Parse_JSON::on_tbCaseSens_clicked()
{
   parseJson->resetCurrentFind();
   on_leSearch_returnPressed();
}

void Parse_JSON::on_tbExpandAll_clicked()
{
    parseJson->setExpandAll( ui->tbExpandAll->isChecked() );
}
