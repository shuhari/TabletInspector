#include "propertywidget.h"


PropertyWidget::PropertyWidget() {
    _pTabletInfoWidget = new TabletInfoWidget();

    addTab(_pTabletInfoWidget, tr("Tablet"));
}
