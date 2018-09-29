#include "stdafx.h"
#include "SettingsDialog.h"
#include "App.h"
#include "Strings.h"
#include "QtHelper.h"


GeneralPage::GeneralPage(QWidget* parent)
    : QWidget(parent) {
    setupUi();
}


void GeneralPage::setupUi() {
    _languageCbo = new QComboBox();
    _languageCbo->setMinimumWidth(200);

    auto form = new QFormLayout();
    form->addRow(Strings::language(), _languageCbo);
    setLayout(form);
}


void GeneralPage::loadSettings(AppSettings* settings) {
    QStringList locales;
    locales << "en" << "zh";
    int iIndex = 0, iSelected = -1;
    for (QString localeName : locales) {
        QLocale locale(localeName);
        QString displayName = QString("%1 (%2)")
            .arg(locale.nativeLanguageName())
            .arg(locale.nativeCountryName());
        _languageCbo->insertItem(iIndex, displayName, QVariant(localeName));
        if (localeName == settings->language())
            iSelected = iIndex;
        iIndex++;
    }
    if (iSelected >= 0)
        _languageCbo->setCurrentIndex(iSelected);
}


void GeneralPage::saveSettings(AppSettings* settings) {
    if (_languageCbo->currentIndex() >= 0) {
        settings->setLanguage(_languageCbo->currentData().toString());
    }
}


SettingsDialog::SettingsDialog(QWidget* parent) :
    QDialog(parent) {

    setWindowTitle(Strings::settings());

    setupUi();
    loadSettings();

    _categoryList->setCurrentRow(0);
}


void SettingsDialog::setupUi() {
    _categoryList = new QListWidget();
    _categoryList->setFixedWidth(200);
    
    _generalPage = new GeneralPage();

    auto btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    _pageLayout = new QStackedLayout();
    
    auto hbox = Layout::hbox()
        .add(_categoryList)
        .add(_pageLayout, 1)
        .layout();

    auto vbox = Layout::vbox()
        .add(hbox, 1)
        .add(btnBox)
        .apply(this);

    connect(btnBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onOk);
    connect(btnBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    connect(_categoryList, &QListWidget::currentItemChanged, this,
        &SettingsDialog::onCategoryCurrentItemChanged);

    addCategory(Strings::general(), _generalPage);
}


void SettingsDialog::addCategory(const QString& title, QWidget* widget) {
    auto item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, QVariant::fromValue(widget));
    _categoryList->addItem(item);
    _pageLayout->addWidget(widget);
}


void SettingsDialog::onOk() {
    saveSettings();
    accept();
}


void SettingsDialog::loadSettings() {
    auto app = qobject_cast<App*>(qApp);
    auto settings = app->settings();
    
    _generalPage->loadSettings(settings);
}


void SettingsDialog::saveSettings() {
    auto app = qobject_cast<App*>(qApp);
    auto settings = app->settings();

    _generalPage->saveSettings(settings);

    settings->save();
}


void SettingsDialog::onCategoryCurrentItemChanged(QListWidgetItem* current,
    QListWidgetItem* previous) {
    if (current) {
        auto widget = qvariant_cast<QWidget*>(current->data(Qt::UserRole));
        if (widget) {
            _pageLayout->setCurrentWidget(widget);
        }
    }
}
