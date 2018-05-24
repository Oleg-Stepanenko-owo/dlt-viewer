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
