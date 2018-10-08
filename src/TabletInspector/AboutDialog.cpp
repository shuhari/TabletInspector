#include "stdafx.h"
#include "AboutDialog.h"
#include "QtHelper.h"
#include "AppVersion.h"
#include "Strings.h"
#include "Resources.h"


AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent) {

    setupUi();
    setWindowTitle(Strings::aboutTitle().arg(Strings::appTitle()));
}


void AboutDialog::setupUi() {

    auto iconLabel = new QLabel();
    iconLabel->setPixmap(Resources::appIconPixmap());

    auto versionLabel = new QLabel();
    versionLabel->setText(QString("%1 %2")
        .arg(Strings::appTitle())
        .arg(APP_VERSION));

    auto btnBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto hbox = Layout::hbox()
        .add(iconLabel)
        .add(versionLabel);

    Layout::vbox()
        .add(hbox.layout())
        .add(btnBox)
        .apply(this);
}