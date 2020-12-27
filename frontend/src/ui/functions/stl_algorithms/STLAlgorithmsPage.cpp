#include "STLAlgorithmsPage.h"
#include "ui_STLAlgorithmsPage.h"

using namespace templex;
using namespace templex::frontend;

STLAlgorithmsPage::STLAlgorithmsPage() : ui_(new Ui::STLAlgorithmsPage()) {}

void STLAlgorithmsPage::initialize()
{
    ui_->setupUi(this);
    ui_->pushButton->setText(pageName());
}

void STLAlgorithmsPage::deinitialize() {}

QString STLAlgorithmsPage::pageName()
{
    return "STL Algorithms";
}
