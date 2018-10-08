#pragma once


class AboutDialog : public QDialog {
public:
    AboutDialog(QWidget* parent = nullptr);
    virtual ~AboutDialog() = default;

private:
    void        setupUi();
};

