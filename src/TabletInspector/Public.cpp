#include "stdafx.h"
#include "Public.h"


// {62F12D4C-3431-4EFD-8DD7-8E9AAB18D30C}
GUID GUID_DEVINTERFACE_TABLET_WINUSB =
    { 0x62F12D4C, 0x3431, 0x4EFD, {0x8D, 0xD7, 0x8E, 0x9A, 0xAB, 0x18, 0xD3, 0x0C} };


QString TabletInfo::tabletName() {
    return QString("%1 %2").arg(oem).arg(model);
}