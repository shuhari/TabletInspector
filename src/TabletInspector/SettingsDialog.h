#pragma once


#include "AppSettings.h"


class GeneralPage : public QWidget {
    Q_OBJECT
public:
    GeneralPage(QWidget* parent = nullptr);
    virtual ~GeneralPage() = default;

    void loadSettings(AppSettings* settings);
    void saveSettings(AppSettings* settings);

private:
    QComboBox*      _languageCbo;

    void            setupUi();
};


class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget* parent = nullptr);
    virtual ~SettingsDialog() = default;

private:
    QListWidget*            _categoryList;
    QStackedLayout*         _pageLayout;
    GeneralPage*            _generalPage;

    void                    setupUi();
    void                    addCategory(const QString& title, QWidget* page);
    void                    loadSettings();
    void                    saveSettings();

private slots:
    void                    onOk();
    void                    onCategoryCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
};

