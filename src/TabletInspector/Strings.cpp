#include "stdafx.h"
#include "Strings.h"


QString Strings::appTitle() { return tr("Tablet Inspector"); }

QString Strings::menuFile() { return tr("&File"); }
QString Strings::menuEdit() { return tr("&Edit"); }
QString Strings::menuView() { return tr("&View"); }
QString Strings::menuHelp() { return tr("&Help"); }
QString Strings::menuTool() { return tr("&Tools"); }

QString Strings::actionFileExit()      { return tr("E&xit"); }
QString Strings::actionEditClear() { return tr("&Clear"); }
QString Strings::actionViewToolBar()   { return tr("&Toolbar"); }
QString Strings::actionViewStatusBar() { return tr("&Status Bar"); }
QString Strings::actionToolSettings()  { return tr("&Settings..."); }

QString Strings::actionHelpAbout()     { return tr("&About"); }

QString Strings::settings() { return tr("Settings"); }
QString Strings::general() { return tr("General"); }
QString Strings::language() { return tr("Language"); }
QString Strings::disconnected() { return tr("Disconnected"); }
QString Strings::clear() { return tr("Clear"); }
QString Strings::logs() { return tr("Logs"); }
QString Strings::properties() { return tr("Properties"); }
QString Strings::tabletInfo() { return tr("Tablet Information"); }
QString Strings::data() { return tr("Data"); }
QString Strings::oem() { return tr("OEM"); }
QString Strings::model() { return tr("Model"); }
QString Strings::fixtureDate() { return tr("Fixture Date"); }
QString Strings::company() { return tr("Company"); }
QString Strings::size() { return tr("Size"); }
QString Strings::maxPressure() { return tr("Max Pressure"); }
QString Strings::pBtnNum() { return tr("Pen Buttons"); }
QString Strings::hBtnNum() { return tr("Hard Buttons"); }
QString Strings::sBtnNum() { return tr("Soft Buttons"); }
QString Strings::lpi() { return tr("LPI"); }
QString Strings::rate() { return tr("Rate"); }
QString Strings::isMonitor() { return tr("Is Monitor"); }
QString Strings::isPassive() { return tr("Is Passive"); }
QString Strings::trueStr() { return tr("True"); }
QString Strings::falseStr() { return tr("False"); }
QString Strings::hex() { return tr("Hexadecimal"); }
QString Strings::realTime() { return tr("Realtime"); }
QString Strings::x() { return "X"; }
QString Strings::y() { return "Y"; }
QString Strings::pressure() { return tr("Pressure"); }
QString Strings::passive() { return tr("Passive"); }
QString Strings::unPassive() { return tr("Unpassive"); }
QString Strings::pen() { return tr("Pen"); }
QString Strings::penBtn() { return tr("Pen Button"); }
QString Strings::penDown() { return tr("Pen down"); }
QString Strings::penBtnDown() { return tr("Button %1 down"); }
QString Strings::touchBtn() { return tr("Touch Button"); }
QString Strings::touchBtnDown() { return tr("Button %1 down"); }
QString Strings::sliderPress() { return tr("Slider %1 pressed"); }
QString Strings::slider() { return tr("Slider"); }
QString Strings::analytics() { return tr("Analytics"); }
QString Strings::rawData() { return tr("Raw Data"); }
// Not sure how to translate, temp keep untranslated
QString Strings::ax() { return "ax"; }
QString Strings::ay() { return "ay"; }
QString Strings::altitude() { return "Altitude"; }
QString Strings::azimuth() { return "Azimuth"; }
QString Strings::aboutTitle() { return tr("About %1"); }

QString Strings::msg_tabletConnected() { 
    return tr("Tablet connected, path: %1."); 
}
QString Strings::msg_tabletDisconnected() { 
    return tr("Tablet disconnected, path: %1."); 
}
QString Strings::msg_tabletReadError() {
    return tr("Tablet read error, code: %1.");
}
QString Strings::msg_tabletOpenFailed() {
    return tr("Open tablet failed, error: %1.\n\nIf Other programs such as driver is running, please close it first to use TabletInspector.");
}