#include "tabletinfowidget.h"


TabletInfoWidget::TabletInfoWidget() {
    setColumnCount(2);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}
