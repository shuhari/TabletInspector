#include "stdafx.h"
#include "SettingsDialog.h"
#include "App.h"


GeneralPage::GeneralPage(QWidget* parent)
    : QWidget(parent) {
    _languageCbo = new QComboBox();

    auto form = new QFormLayout();
    form->addRow(tr("Language"), _languageCbo);
    setLayout(form);

    auto app = qobject_cast<App*>(qApp);
    auto& settings = app->settings();
    QStringList locales = app->enumLocales();
    int iIndex = 0, iSelection = -1;
    for (QString localeName : locales) {
        QLocale locale(localeName);
        QString name = QString("%1 [%2]").arg(locale.nativeLanguageName())
            .arg(locale.nativeCountryName());
        _languageCbo->addItem(name, QVariant(localeName));
        if (localeName == settings.language())
            iSelection = iIndex;
        iIndex++;
    }
    if (iSelection >= 0)
        _languageCbo->setCurrentIndex(iSelection);
}


void GeneralPage::saveSettings() {
    auto app = qobject_cast<App*>(qApp);
    auto& settings = app->settings();
    if (_languageCbo->currentIndex() >= 0) {
        settings.setLanguage(_languageCbo->currentData().toString());
    }

}


SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {

    setWindowTitle(tr("Settings"));

    _categoryList = new QListWidget();
    _categoryList->setFixedWidth(200);
    _generalPage = new GeneralPage();

    auto btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    
    _pageBox = new QStackedLayout();
    
    auto hbox = new QHBoxLayout();
    hbox->addWidget(_categoryList);
    hbox->addLayout(_pageBox, 1);

    auto vbox = new QVBoxLayout();
    vbox->addLayout(hbox, 1);
    vbox->addWidget(btnBox);
    setLayout(vbox);

    connect(btnBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onOk);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(_categoryList, &QListWidget::currentItemChanged, this, &SettingsDialog::onCategoryCurrentItemChanged);

    addCategory(tr("General"), _generalPage);
    _categoryList->setCurrentRow(0);
}


void SettingsDialog::addCategory(const QString& title, QWidget* widget) {
    auto pItem = new QListWidgetItem(title);
    pItem->setData(Qt::UserRole, QVariant::fromValue(widget));
    _pageBox->addWidget(widget);
    _categoryList->addItem(pItem);
}


void SettingsDialog::onOk() {
    _generalPage->saveSettings();

    auto app = qobject_cast<App*>(qApp);
    app->settings().save();
    accept();
}


void SettingsDialog::onCategoryCurrentItemChanged(QListWidgetItem* current,
    QListWidgetItem* previous) {
    if (current) {
        QWidget* widget = qvariant_cast<QWidget*>(current->data(Qt::UserRole));
        if (widget) {
            _pageBox->setCurrentWidget(widget);
        }
    }
}