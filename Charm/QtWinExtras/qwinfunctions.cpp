/****************************************************************************
**
** Copyright (C) 2016 Ivan Vizir <define-true-false@yandex.com>
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWinExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#if defined(NTDDI_VERSION) && NTDDI_VERSION < 0x06010000 // NTDDI_WIN7
#  undef NTDDI_VERSION
#endif
#if !defined(NTDDI_VERSION)
#  define NTDDI_VERSION 0x06010000 // Enable functions for MinGW
#endif

#include "qwinfunctions.h"
#include "qwinfunctions_p.h"
#include "qwineventfilter_p.h"
#include "windowsguidsdefs_p.h"

#include <QtGui/qguiapplication.h>
#include <QtGui/qwindow.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qimage.h>
#include <QtGui/qcolor.h>
#include <QtGui/qregion.h>
#include <QtCore/qmargins.h>
#include <QtCore/qsettings.h>

#include <comdef.h>
#include "winshobjidl_p.h"

using namespace Qt::Literals::StringLiterals;

QT_BEGIN_NAMESPACE

Q_GUI_EXPORT HBITMAP qt_createIconMask(const QBitmap &bitmap);
Q_GUI_EXPORT HBITMAP qt_pixmapToWinHBITMAP(const QPixmap &p, int hbitmapFormat = 0);
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat = 0);
Q_GUI_EXPORT HICON   qt_pixmapToWinHICON(const QPixmap &p);
Q_GUI_EXPORT HBITMAP qt_imageToWinHBITMAP(const QImage &imageIn, int hbitmapFormat = 0);
Q_GUI_EXPORT QImage qt_imageFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat = 0);
Q_GUI_EXPORT QImage  qt_imageFromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h);
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHICON(HICON icon);

/*!
    \namespace QtWin
    \inmodule QtWinExtras
    \brief The QtWin namespace contains miscellaneous Windows-specific functions.
    \inheaderfile QtWin

 */

/*!
    \since 5.2

    Creates a \c HBITMAP equivalent of the QBitmap \a bitmap.

    It is the caller's responsibility to free the \c HBITMAP data after use.

    \obsolete

    Use image.convertToFormat(QImage::Format_Mono).invertPixels().toHBITMAP() instead.

    \sa toHBITMAP()
*/
HBITMAP QtWin::createMask(const QBitmap &bitmap)
{
    return qt_createIconMask(bitmap);
}

/*!
    \since 5.2

    Creates a \c HBITMAP equivalent of the QPixmap \a p,
    based on the given \a format. Returns the \c HBITMAP handle.

    It is the caller's responsibility to free the \c HBITMAP data
    after use.

    \obsolete Use QImage::toHBITMAP() instead.

    \sa fromHBITMAP()
*/
HBITMAP QtWin::toHBITMAP(const QPixmap &p, QtWin::HBitmapFormat format)
{
    return qt_pixmapToWinHBITMAP(p, format);
}

/*!
    \since 5.2

    Returns a QPixmap that is equivalent to the
    given \a bitmap. The conversion is based on the specified \a format.

    \obsolete Use QImage::fromHBITMAP() instead.

    \sa toHBITMAP()
*/
QPixmap QtWin::fromHBITMAP(HBITMAP bitmap, QtWin::HBitmapFormat format)
{
    return qt_pixmapFromWinHBITMAP(bitmap, format);
}

/*!
    \since 5.2

    Creates a \c HICON equivalent of the QPixmap \a p.
    Returns the \c HICON handle.

    It is the caller's responsibility to free the \c HICON data after use.

    \obsolete Use QImage::toHICON() instead.

    \sa fromHICON()
*/
HICON QtWin::toHICON(const QPixmap &p)
{
    return qt_pixmapToWinHICON(p);
}

/*!
    \since 5.12

    Creates a \c HBITMAP equivalent of the QImage \a image,
    based on the given \a format. Returns the \c HBITMAP handle.

    It is the caller's responsibility to free the \c HBITMAP data
    after use.

    \obsolete Use QImage::toHBITMAP() instead.

    \sa imageFromHBITMAP()
*/
HBITMAP QtWin::imageToHBITMAP(const QImage &image, QtWin::HBitmapFormat format)
{
    return qt_imageToWinHBITMAP(image, format);
}

/*!
    \since 5.2

    Returns a QImage that is equivalent to the
    given \a bitmap. The conversion is based on the specified \c HDC context \a hdc
    using the specified \a width and \a height.

    \obsolete Use QImage::fromHBITMAP() instead.

    \sa toHBITMAP()
*/
QImage QtWin::imageFromHBITMAP(HDC hdc, HBITMAP bitmap, int width, int height)
{
    return qt_imageFromWinHBITMAP(hdc, bitmap, width, height);
}

/*!
    \since 5.12

    Returns a QImage that is equivalent to the
    given \a bitmap.  The conversion is based on the specified \a format.

    \obsolete Use QImage::fromHBITMAP() instead.

    \sa imageToHBITMAP()
*/
QImage QtWin::imageFromHBITMAP(HBITMAP bitmap, QtWin::HBitmapFormat format)
{
    return qt_imageFromWinHBITMAP(bitmap, format);
}

/*!
    \since 5.2

    Returns a QPixmap that is equivalent to the given \a icon.

    \obsolete Use QImage::fromHICON() instead.

    \sa toHICON()
*/
QPixmap QtWin::fromHICON(HICON icon)
{
    return qt_pixmapFromWinHICON(icon);
}

HRGN qt_RectToHRGN(const QRect &rc)
{
    return CreateRectRgn(rc.left(), rc.top(), rc.right()+1, rc.bottom()+1);
}

/*!
    \since 5.2

    Returns a HRGN that is equivalent to the given \a region.

    \obsolete Use QRegion::toHRGN() instead.
 */
HRGN QtWin::toHRGN(const QRegion &region)
{
    const int size = region.rectCount();
    if (size == 0)
        return nullptr;

    HRGN resultRgn = nullptr;
    const auto rects = region.begin();
    resultRgn = qt_RectToHRGN(rects[0]);
    for (int i = 1; i < size; i++) {
        HRGN tmpRgn = qt_RectToHRGN(rects[i]);
        int err = CombineRgn(resultRgn, resultRgn, tmpRgn, RGN_OR);
        if (err == ERROR)
            qWarning("Error combining HRGNs.");
        DeleteObject(tmpRgn);
    }
    return resultRgn;
}

/*!
    \since 5.2

    Returns a QRegion that is equivalent to the given \a hrgn.

    \obsolete Use QRegion::fromHRGN() instead.
 */
QRegion QtWin::fromHRGN(HRGN hrgn)
{
    DWORD regionDataSize = GetRegionData(hrgn, 0, nullptr);
    if (regionDataSize == 0)
        return QRegion();

    auto regionData = reinterpret_cast<LPRGNDATA>(malloc(regionDataSize));
    if (!regionData)
        return QRegion();

    QRegion region;
    if (GetRegionData(hrgn, regionDataSize, regionData) == regionDataSize) {
        auto pRect = reinterpret_cast<LPRECT>(regionData->Buffer);
        for (DWORD i = 0; i < regionData->rdh.nCount; ++i)
            region += QRect(pRect[i].left, pRect[i].top,
                            pRect[i].right - pRect[i].left,
                            pRect[i].bottom - pRect[i].top);
    }

    free(regionData);
    return region;
}

/*!
    \since 5.2

    Returns a message string that explains the \a hresult error id specified or
    an empty string if the explanation cannot be found.

    \obsolete Use \c qt_error_string() instead.
 */
QString QtWin::stringFromHresult(HRESULT hresult)
{
    _com_error error(hresult);
    return QString::fromWCharArray(error.ErrorMessage());
}

/*!
    \since 5.2

    Returns the code name of the \a hresult error id specified (usually the name
    of the WinAPI macro) or an empty string if the message is unknown.

    \obsolete
 */
QString QtWin::errorStringFromHresult(HRESULT hresult)
{
    switch (hresult) {
    case HRESULT(0x8000FFFF) : return u"E_UNEXPECTED"_s;
    case HRESULT(0x80004001) : return u"E_NOTIMPL"_s;
    case HRESULT(0x8007000E) : return u"E_OUTOFMEMORY"_s;
    case HRESULT(0x80070057) : return u"E_INVALIDARG"_s;
    case HRESULT(0x80004002) : return u"E_NOINTERFACE"_s;
    case HRESULT(0x80004003) : return u"E_POINTER"_s;
    case HRESULT(0x80070006) : return u"E_HANDLE"_s;
    case HRESULT(0x80004004) : return u"E_ABORT"_s;
    case HRESULT(0x80004005) : return u"E_FAIL"_s;
    case HRESULT(0x80070005) : return u"E_ACCESSDENIED"_s;
    case HRESULT(0x8000000A) : return u"E_PENDING"_s;
    case HRESULT(0x80004006) : return u"CO_E_INIT_TLS"_s;
    case HRESULT(0x80004007) : return u"CO_E_INIT_SHARED_ALLOCATOR"_s;
    case HRESULT(0x80004008) : return u"CO_E_INIT_MEMORY_ALLOCATOR"_s;
    case HRESULT(0x80004009) : return u"CO_E_INIT_CLASS_CACHE"_s;
    case HRESULT(0x8000400A) : return u"CO_E_INIT_RPC_CHANNEL"_s;
    case HRESULT(0x8000400B) : return u"CO_E_INIT_TLS_SET_CHANNEL_CONTROL"_s;
    case HRESULT(0x8000400C) : return u"CO_E_INIT_TLS_CHANNEL_CONTROL"_s;
    case HRESULT(0x8000400D) : return u"CO_E_INIT_UNACCEPTED_USER_ALLOCATOR"_s;
    case HRESULT(0x8000400E) : return u"CO_E_INIT_SCM_MUTEX_EXISTS"_s;
    case HRESULT(0x8000400F) : return u"CO_E_INIT_SCM_FILE_MAPPING_EXISTS"_s;
    case HRESULT(0x80004010) : return u"CO_E_INIT_SCM_MAP_VIEW_OF_FILE"_s;
    case HRESULT(0x80004011) : return u"CO_E_INIT_SCM_EXEC_FAILURE"_s;
    case HRESULT(0x80004012) : return u"CO_E_INIT_ONLY_SINGLE_THREADED"_s;
    case HRESULT(0x80004013) : return u"CO_E_CANT_REMOTE"_s;
    case HRESULT(0x80004014) : return u"CO_E_BAD_SERVER_NAME"_s;
    case HRESULT(0x80004015) : return u"CO_E_WRONG_SERVER_IDENTITY"_s;
    case HRESULT(0x80004016) : return u"CO_E_OLE1DDE_DISABLED"_s;
    case HRESULT(0x80004017) : return u"CO_E_RUNAS_SYNTAX"_s;
    case HRESULT(0x80004018) : return u"CO_E_CREATEPROCESS_FAILURE"_s;
    case HRESULT(0x80004019) : return u"CO_E_RUNAS_CREATEPROCESS_FAILURE"_s;
    case HRESULT(0x8000401A) : return u"CO_E_RUNAS_LOGON_FAILURE"_s;
    case HRESULT(0x8000401B) : return u"CO_E_LAUNCH_PERMSSION_DENIED"_s;
    case HRESULT(0x8000401C) : return u"CO_E_START_SERVICE_FAILURE"_s;
    case HRESULT(0x8000401D) : return u"CO_E_REMOTE_COMMUNICATION_FAILURE"_s;
    case HRESULT(0x8000401E) : return u"CO_E_SERVER_START_TIMEOUT"_s;
    case HRESULT(0x8000401F) : return u"CO_E_CLSREG_INCONSISTENT"_s;
    case HRESULT(0x80004020) : return u"CO_E_IIDREG_INCONSISTENT"_s;
    case HRESULT(0x80004021) : return u"CO_E_NOT_SUPPORTED"_s;
    case HRESULT(0x80004022) : return u"CO_E_RELOAD_DLL"_s;
    case HRESULT(0x80004023) : return u"CO_E_MSI_ERROR"_s;
    case HRESULT(0x80004024) : return u"CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT"_s;
    case HRESULT(0x80004025) : return u"CO_E_SERVER_PAUSED"_s;
    case HRESULT(0x80004026) : return u"CO_E_SERVER_NOT_PAUSED"_s;
    case HRESULT(0x80004027) : return u"CO_E_CLASS_DISABLED"_s;
    case HRESULT(0x80004028) : return u"CO_E_CLRNOTAVAILABLE"_s;
    case HRESULT(0x80004029) : return u"CO_E_ASYNC_WORK_REJECTED"_s;
    case HRESULT(0x8000402A) : return u"CO_E_SERVER_INIT_TIMEOUT"_s;
    case HRESULT(0x8000402B) : return u"CO_E_NO_SECCTX_IN_ACTIVATE"_s;
    case HRESULT(0x80004030) : return u"CO_E_TRACKER_CONFIG"_s;
    case HRESULT(0x80004031) : return u"CO_E_THREADPOOL_CONFIG"_s;
    case HRESULT(0x80004032) : return u"CO_E_SXS_CONFIG"_s;
    case HRESULT(0x80004033) : return u"CO_E_MALFORMED_SPN"_s;
    case HRESULT(0x80040000) : return u"OLE_E_OLEVERB"_s;
    case HRESULT(0x80040001) : return u"OLE_E_ADVF"_s;
    case HRESULT(0x80040002) : return u"OLE_E_ENUM_NOMORE"_s;
    case HRESULT(0x80040003) : return u"OLE_E_ADVISENOTSUPPORTED"_s;
    case HRESULT(0x80040004) : return u"OLE_E_NOCONNECTION"_s;
    case HRESULT(0x80040005) : return u"OLE_E_NOTRUNNING"_s;
    case HRESULT(0x80040006) : return u"OLE_E_NOCACHE"_s;
    case HRESULT(0x80040007) : return u"OLE_E_BLANK"_s;
    case HRESULT(0x80040008) : return u"OLE_E_CLASSDIFF"_s;
    case HRESULT(0x80040009) : return u"OLE_E_CANT_GETMONIKER"_s;
    case HRESULT(0x8004000A) : return u"OLE_E_CANT_BINDTOSOURCE"_s;
    case HRESULT(0x8004000B) : return u"OLE_E_STATIC"_s;
    case HRESULT(0x8004000C) : return u"OLE_E_PROMPTSAVECANCELLED"_s;
    case HRESULT(0x8004000D) : return u"OLE_E_INVALIDRECT"_s;
    case HRESULT(0x8004000E) : return u"OLE_E_WRONGCOMPOBJ"_s;
    case HRESULT(0x8004000F) : return u"OLE_E_INVALIDHWND"_s;
    case HRESULT(0x80040010) : return u"OLE_E_NOT_INPLACEACTIVE"_s;
    case HRESULT(0x80040011) : return u"OLE_E_CANTCONVERT"_s;
    case HRESULT(0x80040012) : return u"OLE_E_NOSTORAGE"_s;
    case HRESULT(0x80040064) : return u"DV_E_FORMATETC"_s;
    case HRESULT(0x80040065) : return u"DV_E_DVTARGETDEVICE"_s;
    case HRESULT(0x80040066) : return u"DV_E_STGMEDIUM"_s;
    case HRESULT(0x80040067) : return u"DV_E_STATDATA"_s;
    case HRESULT(0x80040068) : return u"DV_E_LINDEX"_s;
    case HRESULT(0x80040069) : return u"DV_E_TYMED"_s;
    case HRESULT(0x8004006A) : return u"DV_E_CLIPFORMAT"_s;
    case HRESULT(0x8004006B) : return u"DV_E_DVASPECT"_s;
    case HRESULT(0x8004006C) : return u"DV_E_DVTARGETDEVICE_SIZE"_s;
    case HRESULT(0x8004006D) : return u"DV_E_NOIVIEWOBJECT"_s;
    case HRESULT(0x80040100) : return u"DRAGDROP_E_NOTREGISTERED"_s;
    case HRESULT(0x80040101) : return u"DRAGDROP_E_ALREADYREGISTERED"_s;
    case HRESULT(0x80040102) : return u"DRAGDROP_E_INVALIDHWND"_s;
    case HRESULT(0x80040110) : return u"CLASS_E_NOAGGREGATION"_s;
    case HRESULT(0x80040111) : return u"CLASS_E_CLASSNOTAVAILABLE"_s;
    case HRESULT(0x80040112) : return u"CLASS_E_NOTLICENSED"_s;
    case HRESULT(0x80040140) : return u"VIEW_E_DRAW"_s;
    case HRESULT(0x80040150) : return u"REGDB_E_READREGDB"_s;
    case HRESULT(0x80040151) : return u"REGDB_E_WRITEREGDB"_s;
    case HRESULT(0x80040152) : return u"REGDB_E_KEYMISSING"_s;
    case HRESULT(0x80040153) : return u"REGDB_E_INVALIDVALUE"_s;
    case HRESULT(0x80040154) : return u"REGDB_E_CLASSNOTREG"_s;
    case HRESULT(0x80040155) : return u"REGDB_E_IIDNOTREG"_s;
    case HRESULT(0x80040156) : return u"REGDB_E_BADTHREADINGMODEL"_s;
    case HRESULT(0x80040160) : return u"CAT_E_CATIDNOEXIST"_s;
    case HRESULT(0x80040161) : return u"CAT_E_NODESCRIPTION"_s;
    case HRESULT(0x80040164) : return u"CS_E_PACKAGE_NOTFOUND"_s;
    case HRESULT(0x80040165) : return u"CS_E_NOT_DELETABLE"_s;
    case HRESULT(0x80040166) : return u"CS_E_CLASS_NOTFOUND"_s;
    case HRESULT(0x80040167) : return u"CS_E_INVALID_VERSION"_s;
    case HRESULT(0x80040168) : return u"CS_E_NO_CLASSSTORE"_s;
    case HRESULT(0x80040169) : return u"CS_E_OBJECT_NOTFOUND"_s;
    case HRESULT(0x8004016A) : return u"CS_E_OBJECT_ALREADY_EXISTS"_s;
    case HRESULT(0x8004016B) : return u"CS_E_INVALID_PATH"_s;
    case HRESULT(0x8004016C) : return u"CS_E_NETWORK_ERROR"_s;
    case HRESULT(0x8004016D) : return u"CS_E_ADMIN_LIMIT_EXCEEDED"_s;
    case HRESULT(0x8004016E) : return u"CS_E_SCHEMA_MISMATCH"_s;
    case HRESULT(0x8004016F) : return u"CS_E_INTERNAL_ERROR"_s;
    case HRESULT(0x80040170) : return u"CACHE_E_NOCACHE_UPDATED"_s;
    case HRESULT(0x80040180) : return u"OLEOBJ_E_NOVERBS"_s;
    case HRESULT(0x80040181) : return u"OLEOBJ_E_INVALIDVERB"_s;
    case HRESULT(0x800401A0) : return u"INPLACE_E_NOTUNDOABLE"_s;
    case HRESULT(0x800401A1) : return u"INPLACE_E_NOTOOLSPACE"_s;
    case HRESULT(0x800401C0) : return u"CONVERT10_E_OLESTREAM_GET"_s;
    case HRESULT(0x800401C1) : return u"CONVERT10_E_OLESTREAM_PUT"_s;
    case HRESULT(0x800401C2) : return u"CONVERT10_E_OLESTREAM_FMT"_s;
    case HRESULT(0x800401C3) : return u"CONVERT10_E_OLESTREAM_BITMAP_TO_DIB"_s;
    case HRESULT(0x800401C4) : return u"CONVERT10_E_STG_FMT"_s;
    case HRESULT(0x800401C5) : return u"CONVERT10_E_STG_NO_STD_STREAM"_s;
    case HRESULT(0x800401C6) : return u"CONVERT10_E_STG_DIB_TO_BITMAP"_s;
    case HRESULT(0x800401D0) : return u"CLIPBRD_E_CANT_OPEN"_s;
    case HRESULT(0x800401D1) : return u"CLIPBRD_E_CANT_EMPTY"_s;
    case HRESULT(0x800401D2) : return u"CLIPBRD_E_CANT_SET"_s;
    case HRESULT(0x800401D3) : return u"CLIPBRD_E_BAD_DATA"_s;
    case HRESULT(0x800401D4) : return u"CLIPBRD_E_CANT_CLOSE"_s;
    case HRESULT(0x800401E0) : return u"MK_E_CONNECTMANUALLY"_s;
    case HRESULT(0x800401E1) : return u"MK_E_EXCEEDEDDEADLINE"_s;
    case HRESULT(0x800401E2) : return u"MK_E_NEEDGENERIC"_s;
    case HRESULT(0x800401E3) : return u"MK_E_UNAVAILABLE"_s;
    case HRESULT(0x800401E4) : return u"MK_E_SYNTAX"_s;
    case HRESULT(0x800401E5) : return u"MK_E_NOOBJECT"_s;
    case HRESULT(0x800401E6) : return u"MK_E_INVALIDEXTENSION"_s;
    case HRESULT(0x800401E7) : return u"MK_E_INTERMEDIATEINTERFACENOTSUPPORTED"_s;
    case HRESULT(0x800401E8) : return u"MK_E_NOTBINDABLE"_s;
    case HRESULT(0x800401E9) : return u"MK_E_NOTBOUND"_s;
    case HRESULT(0x800401EA) : return u"MK_E_CANTOPENFILE"_s;
    case HRESULT(0x800401EB) : return u"MK_E_MUSTBOTHERUSER"_s;
    case HRESULT(0x800401EC) : return u"MK_E_NOINVERSE"_s;
    case HRESULT(0x800401ED) : return u"MK_E_NOSTORAGE"_s;
    case HRESULT(0x800401EE) : return u"MK_E_NOPREFIX"_s;
    case HRESULT(0x800401EF) : return u"MK_E_ENUMERATION_FAILED"_s;
    case HRESULT(0x800401F0) : return u"CO_E_NOTINITIALIZED"_s;
    case HRESULT(0x800401F1) : return u"CO_E_ALREADYINITIALIZED"_s;
    case HRESULT(0x800401F2) : return u"CO_E_CANTDETERMINECLASS"_s;
    case HRESULT(0x800401F3) : return u"CO_E_CLASSSTRING"_s;
    case HRESULT(0x800401F4) : return u"CO_E_IIDSTRING"_s;
    case HRESULT(0x800401F5) : return u"CO_E_APPNOTFOUND"_s;
    case HRESULT(0x800401F6) : return u"CO_E_APPSINGLEUSE"_s;
    case HRESULT(0x800401F7) : return u"CO_E_ERRORINAPP"_s;
    case HRESULT(0x800401F8) : return u"CO_E_DLLNOTFOUND"_s;
    case HRESULT(0x800401F9) : return u"CO_E_ERRORINDLL"_s;
    case HRESULT(0x800401FA) : return u"CO_E_WRONGOSFORAPP"_s;
    case HRESULT(0x800401FB) : return u"CO_E_OBJNOTREG"_s;
    case HRESULT(0x800401FC) : return u"CO_E_OBJISREG"_s;
    case HRESULT(0x800401FD) : return u"CO_E_OBJNOTCONNECTED"_s;
    case HRESULT(0x800401FE) : return u"CO_E_APPDIDNTREG"_s;
    case HRESULT(0x800401FF) : return u"CO_E_RELEASED"_s;
    case HRESULT(0x00040200) : return u"EVENT_S_SOME_SUBSCRIBERS_FAILED"_s;
    case HRESULT(0x80040201) : return u"EVENT_E_ALL_SUBSCRIBERS_FAILED"_s;
    case HRESULT(0x00040202) : return u"EVENT_S_NOSUBSCRIBERS"_s;
    case HRESULT(0x80040203) : return u"EVENT_E_QUERYSYNTAX"_s;
    case HRESULT(0x80040204) : return u"EVENT_E_QUERYFIELD"_s;
    case HRESULT(0x80040205) : return u"EVENT_E_INTERNALEXCEPTION"_s;
    case HRESULT(0x80040206) : return u"EVENT_E_INTERNALERROR"_s;
    case HRESULT(0x80040207) : return u"EVENT_E_INVALID_PER_USER_SID"_s;
    case HRESULT(0x80040208) : return u"EVENT_E_USER_EXCEPTION"_s;
    case HRESULT(0x80040209) : return u"EVENT_E_TOO_MANY_METHODS"_s;
    case HRESULT(0x8004020A) : return u"EVENT_E_MISSING_EVENTCLASS"_s;
    case HRESULT(0x8004020B) : return u"EVENT_E_NOT_ALL_REMOVED"_s;
    case HRESULT(0x8004020C) : return u"EVENT_E_COMPLUS_NOT_INSTALLED"_s;
    case HRESULT(0x8004020D) : return u"EVENT_E_CANT_MODIFY_OR_DELETE_UNCONFIGURED_OBJECT"_s;
    case HRESULT(0x8004020E) : return u"EVENT_E_CANT_MODIFY_OR_DELETE_CONFIGURED_OBJECT"_s;
    case HRESULT(0x8004020F) : return u"EVENT_E_INVALID_EVENT_CLASS_PARTITION"_s;
    case HRESULT(0x80040210) : return u"EVENT_E_PER_USER_SID_NOT_LOGGED_ON"_s;
    case HRESULT(0x8004D000) : return u"XACT_E_ALREADYOTHERSINGLEPHASE"_s;
    case HRESULT(0x8004D001) : return u"XACT_E_CANTRETAIN"_s;
    case HRESULT(0x8004D002) : return u"XACT_E_COMMITFAILED"_s;
    case HRESULT(0x8004D003) : return u"XACT_E_COMMITPREVENTED"_s;
    case HRESULT(0x8004D004) : return u"XACT_E_HEURISTICABORT"_s;
    case HRESULT(0x8004D005) : return u"XACT_E_HEURISTICCOMMIT"_s;
    case HRESULT(0x8004D006) : return u"XACT_E_HEURISTICDAMAGE"_s;
    case HRESULT(0x8004D007) : return u"XACT_E_HEURISTICDANGER"_s;
    case HRESULT(0x8004D008) : return u"XACT_E_ISOLATIONLEVEL"_s;
    case HRESULT(0x8004D009) : return u"XACT_E_NOASYNC"_s;
    case HRESULT(0x8004D00A) : return u"XACT_E_NOENLIST"_s;
    case HRESULT(0x8004D00B) : return u"XACT_E_NOISORETAIN"_s;
    case HRESULT(0x8004D00C) : return u"XACT_E_NORESOURCE"_s;
    case HRESULT(0x8004D00D) : return u"XACT_E_NOTCURRENT"_s;
    case HRESULT(0x8004D00E) : return u"XACT_E_NOTRANSACTION"_s;
    case HRESULT(0x8004D00F) : return u"XACT_E_NOTSUPPORTED"_s;
    case HRESULT(0x8004D010) : return u"XACT_E_UNKNOWNRMGRID"_s;
    case HRESULT(0x8004D011) : return u"XACT_E_WRONGSTATE"_s;
    case HRESULT(0x8004D012) : return u"XACT_E_WRONGUOW"_s;
    case HRESULT(0x8004D013) : return u"XACT_E_XTIONEXISTS"_s;
    case HRESULT(0x8004D014) : return u"XACT_E_NOIMPORTOBJECT"_s;
    case HRESULT(0x8004D015) : return u"XACT_E_INVALIDCOOKIE"_s;
    case HRESULT(0x8004D016) : return u"XACT_E_INDOUBT"_s;
    case HRESULT(0x8004D017) : return u"XACT_E_NOTIMEOUT"_s;
    case HRESULT(0x8004D018) : return u"XACT_E_ALREADYINPROGRESS"_s;
    case HRESULT(0x8004D019) : return u"XACT_E_ABORTED"_s;
    case HRESULT(0x8004D01A) : return u"XACT_E_LOGFULL"_s;
    case HRESULT(0x8004D01B) : return u"XACT_E_TMNOTAVAILABLE"_s;
    case HRESULT(0x8004D01C) : return u"XACT_E_CONNECTION_DOWN"_s;
    case HRESULT(0x8004D01D) : return u"XACT_E_CONNECTION_DENIED"_s;
    case HRESULT(0x8004D01E) : return u"XACT_E_REENLISTTIMEOUT"_s;
    case HRESULT(0x8004D01F) : return u"XACT_E_TIP_CONNECT_FAILED"_s;
    case HRESULT(0x8004D020) : return u"XACT_E_TIP_PROTOCOL_ERROR"_s;
    case HRESULT(0x8004D021) : return u"XACT_E_TIP_PULL_FAILED"_s;
    case HRESULT(0x8004D022) : return u"XACT_E_DEST_TMNOTAVAILABLE"_s;
    case HRESULT(0x8004D023) : return u"XACT_E_TIP_DISABLED"_s;
    case HRESULT(0x8004D024) : return u"XACT_E_NETWORK_TX_DISABLED"_s;
    case HRESULT(0x8004D025) : return u"XACT_E_PARTNER_NETWORK_TX_DISABLED"_s;
    case HRESULT(0x8004D026) : return u"XACT_E_XA_TX_DISABLED"_s;
    case HRESULT(0x8004D027) : return u"XACT_E_UNABLE_TO_READ_DTC_CONFIG"_s;
    case HRESULT(0x8004D028) : return u"XACT_E_UNABLE_TO_LOAD_DTC_PROXY"_s;
    case HRESULT(0x8004D029) : return u"XACT_E_ABORTING"_s;
    case HRESULT(0x8004D080) : return u"XACT_E_CLERKNOTFOUND"_s;
    case HRESULT(0x8004D081) : return u"XACT_E_CLERKEXISTS"_s;
    case HRESULT(0x8004D082) : return u"XACT_E_RECOVERYINPROGRESS"_s;
    case HRESULT(0x8004D083) : return u"XACT_E_TRANSACTIONCLOSED"_s;
    case HRESULT(0x8004D084) : return u"XACT_E_INVALIDLSN"_s;
    case HRESULT(0x8004D085) : return u"XACT_E_REPLAYREQUEST"_s;
    case HRESULT(0x0004D000) : return u"XACT_S_ASYNC"_s;
    case HRESULT(0x0004D001) : return u"XACT_S_DEFECT"_s;
    case HRESULT(0x0004D002) : return u"XACT_S_READONLY"_s;
    case HRESULT(0x0004D003) : return u"XACT_S_SOMENORETAIN"_s;
    case HRESULT(0x0004D004) : return u"XACT_S_OKINFORM"_s;
    case HRESULT(0x0004D005) : return u"XACT_S_MADECHANGESCONTENT"_s;
    case HRESULT(0x0004D006) : return u"XACT_S_MADECHANGESINFORM"_s;
    case HRESULT(0x0004D007) : return u"XACT_S_ALLNORETAIN"_s;
    case HRESULT(0x0004D008) : return u"XACT_S_ABORTING"_s;
    case HRESULT(0x0004D009) : return u"XACT_S_SINGLEPHASE"_s;
    case HRESULT(0x0004D00A) : return u"XACT_S_LOCALLY_OK"_s;
    case HRESULT(0x0004D010) : return u"XACT_S_LASTRESOURCEMANAGER"_s;
    case HRESULT(0x8004E002) : return u"CONTEXT_E_ABORTED"_s;
    case HRESULT(0x8004E003) : return u"CONTEXT_E_ABORTING"_s;
    case HRESULT(0x8004E004) : return u"CONTEXT_E_NOCONTEXT"_s;
    case HRESULT(0x8004E005) : return u"CONTEXT_E_WOULD_DEADLOCK"_s;
    case HRESULT(0x8004E006) : return u"CONTEXT_E_SYNCH_TIMEOUT"_s;
    case HRESULT(0x8004E007) : return u"CONTEXT_E_OLDREF"_s;
    case HRESULT(0x8004E00C) : return u"CONTEXT_E_ROLENOTFOUND"_s;
    case HRESULT(0x8004E00F) : return u"CONTEXT_E_TMNOTAVAILABLE"_s;
    case HRESULT(0x8004E021) : return u"CO_E_ACTIVATIONFAILED"_s;
    case HRESULT(0x8004E022) : return u"CO_E_ACTIVATIONFAILED_EVENTLOGGED"_s;
    case HRESULT(0x8004E023) : return u"CO_E_ACTIVATIONFAILED_CATALOGERROR"_s;
    case HRESULT(0x8004E024) : return u"CO_E_ACTIVATIONFAILED_TIMEOUT"_s;
    case HRESULT(0x8004E025) : return u"CO_E_INITIALIZATIONFAILED"_s;
    case HRESULT(0x8004E026) : return u"CONTEXT_E_NOJIT"_s;
    case HRESULT(0x8004E027) : return u"CONTEXT_E_NOTRANSACTION"_s;
    case HRESULT(0x8004E028) : return u"CO_E_THREADINGMODEL_CHANGED"_s;
    case HRESULT(0x8004E029) : return u"CO_E_NOIISINTRINSICS"_s;
    case HRESULT(0x8004E02A) : return u"CO_E_NOCOOKIES"_s;
    case HRESULT(0x8004E02B) : return u"CO_E_DBERROR"_s;
    case HRESULT(0x8004E02C) : return u"CO_E_NOTPOOLED"_s;
    case HRESULT(0x8004E02D) : return u"CO_E_NOTCONSTRUCTED"_s;
    case HRESULT(0x8004E02E) : return u"CO_E_NOSYNCHRONIZATION"_s;
    case HRESULT(0x8004E02F) : return u"CO_E_ISOLEVELMISMATCH"_s;
    case HRESULT(0x00040000) : return u"OLE_S_USEREG"_s;
    case HRESULT(0x00040001) : return u"OLE_S_STATIC"_s;
    case HRESULT(0x00040002) : return u"OLE_S_MAC_CLIPFORMAT"_s;
    case HRESULT(0x00040100) : return u"DRAGDROP_S_DROP"_s;
    case HRESULT(0x00040101) : return u"DRAGDROP_S_CANCEL"_s;
    case HRESULT(0x00040102) : return u"DRAGDROP_S_USEDEFAULTCURSORS"_s;
    case HRESULT(0x00040130) : return u"DATA_S_SAMEFORMATETC"_s;
    case HRESULT(0x00040140) : return u"VIEW_S_ALREADY_FROZEN"_s;
    case HRESULT(0x00040170) : return u"CACHE_S_FORMATETC_NOTSUPPORTED"_s;
    case HRESULT(0x00040171) : return u"CACHE_S_SAMECACHE"_s;
    case HRESULT(0x00040172) : return u"CACHE_S_SOMECACHES_NOTUPDATED"_s;
    case HRESULT(0x00040180) : return u"OLEOBJ_S_INVALIDVERB"_s;
    case HRESULT(0x00040181) : return u"OLEOBJ_S_CANNOT_DOVERB_NOW"_s;
    case HRESULT(0x00040182) : return u"OLEOBJ_S_INVALIDHWND"_s;
    case HRESULT(0x000401A0) : return u"INPLACE_S_TRUNCATED"_s;
    case HRESULT(0x000401C0) : return u"CONVERT10_S_NO_PRESENTATION"_s;
    case HRESULT(0x000401E2) : return u"MK_S_REDUCED_TO_SELF"_s;
    case HRESULT(0x000401E4) : return u"MK_S_ME"_s;
    case HRESULT(0x000401E5) : return u"MK_S_HIM"_s;
    case HRESULT(0x000401E6) : return u"MK_S_US"_s;
    case HRESULT(0x000401E7) : return u"MK_S_MONIKERALREADYREGISTERED"_s;
    case HRESULT(0x00041300) : return u"SCHED_S_TASK_READY"_s;
    case HRESULT(0x00041301) : return u"SCHED_S_TASK_RUNNING"_s;
    case HRESULT(0x00041302) : return u"SCHED_S_TASK_DISABLED"_s;
    case HRESULT(0x00041303) : return u"SCHED_S_TASK_HAS_NOT_RUN"_s;
    case HRESULT(0x00041304) : return u"SCHED_S_TASK_NO_MORE_RUNS"_s;
    case HRESULT(0x00041305) : return u"SCHED_S_TASK_NOT_SCHEDULED"_s;
    case HRESULT(0x00041306) : return u"SCHED_S_TASK_TERMINATED"_s;
    case HRESULT(0x00041307) : return u"SCHED_S_TASK_NO_VALID_TRIGGERS"_s;
    case HRESULT(0x00041308) : return u"SCHED_S_EVENT_TRIGGER"_s;
    case HRESULT(0x80041309) : return u"SCHED_E_TRIGGER_NOT_FOUND"_s;
    case HRESULT(0x8004130A) : return u"SCHED_E_TASK_NOT_READY"_s;
    case HRESULT(0x8004130B) : return u"SCHED_E_TASK_NOT_RUNNING"_s;
    case HRESULT(0x8004130C) : return u"SCHED_E_SERVICE_NOT_INSTALLED"_s;
    case HRESULT(0x8004130D) : return u"SCHED_E_CANNOT_OPEN_TASK"_s;
    case HRESULT(0x8004130E) : return u"SCHED_E_INVALID_TASK"_s;
    case HRESULT(0x8004130F) : return u"SCHED_E_ACCOUNT_INFORMATION_NOT_SET"_s;
    case HRESULT(0x80041310) : return u"SCHED_E_ACCOUNT_NAME_NOT_FOUND"_s;
    case HRESULT(0x80041311) : return u"SCHED_E_ACCOUNT_DBASE_CORRUPT"_s;
    case HRESULT(0x80041312) : return u"SCHED_E_NO_SECURITY_SERVICES"_s;
    case HRESULT(0x80041313) : return u"SCHED_E_UNKNOWN_OBJECT_VERSION"_s;
    case HRESULT(0x80041314) : return u"SCHED_E_UNSUPPORTED_ACCOUNT_OPTION"_s;
    case HRESULT(0x80041315) : return u"SCHED_E_SERVICE_NOT_RUNNING"_s;
    case HRESULT(0x80080001) : return u"CO_E_CLASS_CREATE_FAILED"_s;
    case HRESULT(0x80080002) : return u"CO_E_SCM_ERROR"_s;
    case HRESULT(0x80080003) : return u"CO_E_SCM_RPC_FAILURE"_s;
    case HRESULT(0x80080004) : return u"CO_E_BAD_PATH"_s;
    case HRESULT(0x80080005) : return u"CO_E_SERVER_EXEC_FAILURE"_s;
    case HRESULT(0x80080006) : return u"CO_E_OBJSRV_RPC_FAILURE"_s;
    case HRESULT(0x80080007) : return u"MK_E_NO_NORMALIZED"_s;
    case HRESULT(0x80080008) : return u"CO_E_SERVER_STOPPING"_s;
    case HRESULT(0x80080009) : return u"MEM_E_INVALID_ROOT"_s;
    case HRESULT(0x80080010) : return u"MEM_E_INVALID_LINK"_s;
    case HRESULT(0x80080011) : return u"MEM_E_INVALID_SIZE"_s;
    case HRESULT(0x00080012) : return u"CO_S_NOTALLINTERFACES"_s;
    case HRESULT(0x00080013) : return u"CO_S_MACHINENAMENOTFOUND"_s;
    case HRESULT(0x80020001) : return u"DISP_E_UNKNOWNINTERFACE"_s;
    case HRESULT(0x80020003) : return u"DISP_E_MEMBERNOTFOUND"_s;
    case HRESULT(0x80020004) : return u"DISP_E_PARAMNOTFOUND"_s;
    case HRESULT(0x80020005) : return u"DISP_E_TYPEMISMATCH"_s;
    case HRESULT(0x80020006) : return u"DISP_E_UNKNOWNNAME"_s;
    case HRESULT(0x80020007) : return u"DISP_E_NONAMEDARGS"_s;
    case HRESULT(0x80020008) : return u"DISP_E_BADVARTYPE"_s;
    case HRESULT(0x80020009) : return u"DISP_E_EXCEPTION"_s;
    case HRESULT(0x8002000A) : return u"DISP_E_OVERFLOW"_s;
    case HRESULT(0x8002000B) : return u"DISP_E_BADINDEX"_s;
    case HRESULT(0x8002000C) : return u"DISP_E_UNKNOWNLCID"_s;
    case HRESULT(0x8002000D) : return u"DISP_E_ARRAYISLOCKED"_s;
    case HRESULT(0x8002000E) : return u"DISP_E_BADPARAMCOUNT"_s;
    case HRESULT(0x8002000F) : return u"DISP_E_PARAMNOTOPTIONAL"_s;
    case HRESULT(0x80020010) : return u"DISP_E_BADCALLEE"_s;
    case HRESULT(0x80020011) : return u"DISP_E_NOTACOLLECTION"_s;
    case HRESULT(0x80020012) : return u"DISP_E_DIVBYZERO"_s;
    case HRESULT(0x80020013) : return u"DISP_E_BUFFERTOOSMALL"_s;
    case HRESULT(0x80028016) : return u"TYPE_E_BUFFERTOOSMALL"_s;
    case HRESULT(0x80028017) : return u"TYPE_E_FIELDNOTFOUND"_s;
    case HRESULT(0x80028018) : return u"TYPE_E_INVDATAREAD"_s;
    case HRESULT(0x80028019) : return u"TYPE_E_UNSUPFORMAT"_s;
    case HRESULT(0x8002801C) : return u"TYPE_E_REGISTRYACCESS"_s;
    case HRESULT(0x8002801D) : return u"TYPE_E_LIBNOTREGISTERED"_s;
    case HRESULT(0x80028027) : return u"TYPE_E_UNDEFINEDTYPE"_s;
    case HRESULT(0x80028028) : return u"TYPE_E_QUALIFIEDNAMEDISALLOWED"_s;
    case HRESULT(0x80028029) : return u"TYPE_E_INVALIDSTATE"_s;
    case HRESULT(0x8002802A) : return u"TYPE_E_WRONGTYPEKIND"_s;
    case HRESULT(0x8002802B) : return u"TYPE_E_ELEMENTNOTFOUND"_s;
    case HRESULT(0x8002802C) : return u"TYPE_E_AMBIGUOUSNAME"_s;
    case HRESULT(0x8002802D) : return u"TYPE_E_NAMECONFLICT"_s;
    case HRESULT(0x8002802E) : return u"TYPE_E_UNKNOWNLCID"_s;
    case HRESULT(0x8002802F) : return u"TYPE_E_DLLFUNCTIONNOTFOUND"_s;
    case HRESULT(0x800288BD) : return u"TYPE_E_BADMODULEKIND"_s;
    case HRESULT(0x800288C5) : return u"TYPE_E_SIZETOOBIG"_s;
    case HRESULT(0x800288C6) : return u"TYPE_E_DUPLICATEID"_s;
    case HRESULT(0x800288CF) : return u"TYPE_E_INVALIDID"_s;
    case HRESULT(0x80028CA0) : return u"TYPE_E_TYPEMISMATCH"_s;
    case HRESULT(0x80028CA1) : return u"TYPE_E_OUTOFBOUNDS"_s;
    case HRESULT(0x80028CA2) : return u"TYPE_E_IOERROR"_s;
    case HRESULT(0x80028CA3) : return u"TYPE_E_CANTCREATETMPFILE"_s;
    case HRESULT(0x80029C4A) : return u"TYPE_E_CANTLOADLIBRARY"_s;
    case HRESULT(0x80029C83) : return u"TYPE_E_INCONSISTENTPROPFUNCS"_s;
    case HRESULT(0x80029C84) : return u"TYPE_E_CIRCULARTYPE"_s;
    case HRESULT(0x80030001) : return u"STG_E_INVALIDFUNCTION"_s;
    case HRESULT(0x80030002) : return u"STG_E_FILENOTFOUND"_s;
    case HRESULT(0x80030003) : return u"STG_E_PATHNOTFOUND"_s;
    case HRESULT(0x80030004) : return u"STG_E_TOOMANYOPENFILES"_s;
    case HRESULT(0x80030005) : return u"STG_E_ACCESSDENIED"_s;
    case HRESULT(0x80030006) : return u"STG_E_INVALIDHANDLE"_s;
    case HRESULT(0x80030008) : return u"STG_E_INSUFFICIENTMEMORY"_s;
    case HRESULT(0x80030009) : return u"STG_E_INVALIDPOINTER"_s;
    case HRESULT(0x80030012) : return u"STG_E_NOMOREFILES"_s;
    case HRESULT(0x80030013) : return u"STG_E_DISKISWRITEPROTECTED"_s;
    case HRESULT(0x80030019) : return u"STG_E_SEEKERROR"_s;
    case HRESULT(0x8003001D) : return u"STG_E_WRITEFAULT"_s;
    case HRESULT(0x8003001E) : return u"STG_E_READFAULT"_s;
    case HRESULT(0x80030020) : return u"STG_E_SHAREVIOLATION"_s;
    case HRESULT(0x80030021) : return u"STG_E_LOCKVIOLATION"_s;
    case HRESULT(0x80030050) : return u"STG_E_FILEALREADYEXISTS"_s;
    case HRESULT(0x80030057) : return u"STG_E_INVALIDPARAMETER"_s;
    case HRESULT(0x80030070) : return u"STG_E_MEDIUMFULL"_s;
    case HRESULT(0x800300F0) : return u"STG_E_PROPSETMISMATCHED"_s;
    case HRESULT(0x800300FA) : return u"STG_E_ABNORMALAPIEXIT"_s;
    case HRESULT(0x800300FB) : return u"STG_E_INVALIDHEADER"_s;
    case HRESULT(0x800300FC) : return u"STG_E_INVALIDNAME"_s;
    case HRESULT(0x800300FD) : return u"STG_E_UNKNOWN"_s;
    case HRESULT(0x800300FE) : return u"STG_E_UNIMPLEMENTEDFUNCTION"_s;
    case HRESULT(0x800300FF) : return u"STG_E_INVALIDFLAG"_s;
    case HRESULT(0x80030100) : return u"STG_E_INUSE"_s;
    case HRESULT(0x80030101) : return u"STG_E_NOTCURRENT"_s;
    case HRESULT(0x80030102) : return u"STG_E_REVERTED"_s;
    case HRESULT(0x80030103) : return u"STG_E_CANTSAVE"_s;
    case HRESULT(0x80030104) : return u"STG_E_OLDFORMAT"_s;
    case HRESULT(0x80030105) : return u"STG_E_OLDDLL"_s;
    case HRESULT(0x80030106) : return u"STG_E_SHAREREQUIRED"_s;
    case HRESULT(0x80030107) : return u"STG_E_NOTFILEBASEDSTORAGE"_s;
    case HRESULT(0x80030108) : return u"STG_E_EXTANTMARSHALLINGS"_s;
    case HRESULT(0x80030109) : return u"STG_E_DOCFILECORRUPT"_s;
    case HRESULT(0x80030110) : return u"STG_E_BADBASEADDRESS"_s;
    case HRESULT(0x80030111) : return u"STG_E_DOCFILETOOLARGE"_s;
    case HRESULT(0x80030112) : return u"STG_E_NOTSIMPLEFORMAT"_s;
    case HRESULT(0x80030201) : return u"STG_E_INCOMPLETE"_s;
    case HRESULT(0x80030202) : return u"STG_E_TERMINATED"_s;
    case HRESULT(0x00030200) : return u"STG_S_CONVERTED"_s;
    case HRESULT(0x00030201) : return u"STG_S_BLOCK"_s;
    case HRESULT(0x00030202) : return u"STG_S_RETRYNOW"_s;
    case HRESULT(0x00030203) : return u"STG_S_MONITORING"_s;
    case HRESULT(0x00030204) : return u"STG_S_MULTIPLEOPENS"_s;
    case HRESULT(0x00030205) : return u"STG_S_CONSOLIDATIONFAILED"_s;
    case HRESULT(0x00030206) : return u"STG_S_CANNOTCONSOLIDATE"_s;
    case HRESULT(0x80030305) : return u"STG_E_STATUS_COPY_PROTECTION_FAILURE"_s;
    case HRESULT(0x80030306) : return u"STG_E_CSS_AUTHENTICATION_FAILURE"_s;
    case HRESULT(0x80030307) : return u"STG_E_CSS_KEY_NOT_PRESENT"_s;
    case HRESULT(0x80030308) : return u"STG_E_CSS_KEY_NOT_ESTABLISHED"_s;
    case HRESULT(0x80030309) : return u"STG_E_CSS_SCRAMBLED_SECTOR"_s;
    case HRESULT(0x8003030A) : return u"STG_E_CSS_REGION_MISMATCH"_s;
    case HRESULT(0x8003030B) : return u"STG_E_RESETS_EXHAUSTED"_s;
    case HRESULT(0x80010001) : return u"RPC_E_CALL_REJECTED"_s;
    case HRESULT(0x80010002) : return u"RPC_E_CALL_CANCELED"_s;
    case HRESULT(0x80010003) : return u"RPC_E_CANTPOST_INSENDCALL"_s;
    case HRESULT(0x80010004) : return u"RPC_E_CANTCALLOUT_INASYNCCALL"_s;
    case HRESULT(0x80010005) : return u"RPC_E_CANTCALLOUT_INEXTERNALCALL"_s;
    case HRESULT(0x80010006) : return u"RPC_E_CONNECTION_TERMINATED"_s;
    case HRESULT(0x80010007) : return u"RPC_E_SERVER_DIED"_s;
    case HRESULT(0x80010008) : return u"RPC_E_CLIENT_DIED"_s;
    case HRESULT(0x80010009) : return u"RPC_E_INVALID_DATAPACKET"_s;
    case HRESULT(0x8001000A) : return u"RPC_E_CANTTRANSMIT_CALL"_s;
    case HRESULT(0x8001000B) : return u"RPC_E_CLIENT_CANTMARSHAL_DATA"_s;
    case HRESULT(0x8001000C) : return u"RPC_E_CLIENT_CANTUNMARSHAL_DATA"_s;
    case HRESULT(0x8001000D) : return u"RPC_E_SERVER_CANTMARSHAL_DATA"_s;
    case HRESULT(0x8001000E) : return u"RPC_E_SERVER_CANTUNMARSHAL_DATA"_s;
    case HRESULT(0x8001000F) : return u"RPC_E_INVALID_DATA"_s;
    case HRESULT(0x80010010) : return u"RPC_E_INVALID_PARAMETER"_s;
    case HRESULT(0x80010011) : return u"RPC_E_CANTCALLOUT_AGAIN"_s;
    case HRESULT(0x80010012) : return u"RPC_E_SERVER_DIED_DNE"_s;
    case HRESULT(0x80010100) : return u"RPC_E_SYS_CALL_FAILED"_s;
    case HRESULT(0x80010101) : return u"RPC_E_OUT_OF_RESOURCES"_s;
    case HRESULT(0x80010102) : return u"RPC_E_ATTEMPTED_MULTITHREAD"_s;
    case HRESULT(0x80010103) : return u"RPC_E_NOT_REGISTERED"_s;
    case HRESULT(0x80010104) : return u"RPC_E_FAULT"_s;
    case HRESULT(0x80010105) : return u"RPC_E_SERVERFAULT"_s;
    case HRESULT(0x80010106) : return u"RPC_E_CHANGED_MODE"_s;
    case HRESULT(0x80010107) : return u"RPC_E_INVALIDMETHOD"_s;
    case HRESULT(0x80010108) : return u"RPC_E_DISCONNECTED"_s;
    case HRESULT(0x80010109) : return u"RPC_E_RETRY"_s;
    case HRESULT(0x8001010A) : return u"RPC_E_SERVERCALL_RETRYLATER"_s;
    case HRESULT(0x8001010B) : return u"RPC_E_SERVERCALL_REJECTED"_s;
    case HRESULT(0x8001010C) : return u"RPC_E_INVALID_CALLDATA"_s;
    case HRESULT(0x8001010D) : return u"RPC_E_CANTCALLOUT_ININPUTSYNCCALL"_s;
    case HRESULT(0x8001010E) : return u"RPC_E_WRONG_THREAD"_s;
    case HRESULT(0x8001010F) : return u"RPC_E_THREAD_NOT_INIT"_s;
    case HRESULT(0x80010110) : return u"RPC_E_VERSION_MISMATCH"_s;
    case HRESULT(0x80010111) : return u"RPC_E_INVALID_HEADER"_s;
    case HRESULT(0x80010112) : return u"RPC_E_INVALID_EXTENSION"_s;
    case HRESULT(0x80010113) : return u"RPC_E_INVALID_IPID"_s;
    case HRESULT(0x80010114) : return u"RPC_E_INVALID_OBJECT"_s;
    case HRESULT(0x80010115) : return u"RPC_S_CALLPENDING"_s;
    case HRESULT(0x80010116) : return u"RPC_S_WAITONTIMER"_s;
    case HRESULT(0x80010117) : return u"RPC_E_CALL_COMPLETE"_s;
    case HRESULT(0x80010118) : return u"RPC_E_UNSECURE_CALL"_s;
    case HRESULT(0x80010119) : return u"RPC_E_TOO_LATE"_s;
    case HRESULT(0x8001011A) : return u"RPC_E_NO_GOOD_SECURITY_PACKAGES"_s;
    case HRESULT(0x8001011B) : return u"RPC_E_ACCESS_DENIED"_s;
    case HRESULT(0x8001011C) : return u"RPC_E_REMOTE_DISABLED"_s;
    case HRESULT(0x8001011D) : return u"RPC_E_INVALID_OBJREF"_s;
    case HRESULT(0x8001011E) : return u"RPC_E_NO_CONTEXT"_s;
    case HRESULT(0x8001011F) : return u"RPC_E_TIMEOUT"_s;
    case HRESULT(0x80010120) : return u"RPC_E_NO_SYNC"_s;
    case HRESULT(0x80010121) : return u"RPC_E_FULLSIC_REQUIRED"_s;
    case HRESULT(0x80010122) : return u"RPC_E_INVALID_STD_NAME"_s;
    case HRESULT(0x80010123) : return u"CO_E_FAILEDTOIMPERSONATE"_s;
    case HRESULT(0x80010124) : return u"CO_E_FAILEDTOGETSECCTX"_s;
    case HRESULT(0x80010125) : return u"CO_E_FAILEDTOOPENTHREADTOKEN"_s;
    case HRESULT(0x80010126) : return u"CO_E_FAILEDTOGETTOKENINFO"_s;
    case HRESULT(0x80010127) : return u"CO_E_TRUSTEEDOESNTMATCHCLIENT"_s;
    case HRESULT(0x80010128) : return u"CO_E_FAILEDTOQUERYCLIENTBLANKET"_s;
    case HRESULT(0x80010129) : return u"CO_E_FAILEDTOSETDACL"_s;
    case HRESULT(0x8001012A) : return u"CO_E_ACCESSCHECKFAILED"_s;
    case HRESULT(0x8001012B) : return u"CO_E_NETACCESSAPIFAILED"_s;
    case HRESULT(0x8001012C) : return u"CO_E_WRONGTRUSTEENAMESYNTAX"_s;
    case HRESULT(0x8001012D) : return u"CO_E_INVALIDSID"_s;
    case HRESULT(0x8001012E) : return u"CO_E_CONVERSIONFAILED"_s;
    case HRESULT(0x8001012F) : return u"CO_E_NOMATCHINGSIDFOUND"_s;
    case HRESULT(0x80010130) : return u"CO_E_LOOKUPACCSIDFAILED"_s;
    case HRESULT(0x80010131) : return u"CO_E_NOMATCHINGNAMEFOUND"_s;
    case HRESULT(0x80010132) : return u"CO_E_LOOKUPACCNAMEFAILED"_s;
    case HRESULT(0x80010133) : return u"CO_E_SETSERLHNDLFAILED"_s;
    case HRESULT(0x80010134) : return u"CO_E_FAILEDTOGETWINDIR"_s;
    case HRESULT(0x80010135) : return u"CO_E_PATHTOOLONG"_s;
    case HRESULT(0x80010136) : return u"CO_E_FAILEDTOGENUUID"_s;
    case HRESULT(0x80010137) : return u"CO_E_FAILEDTOCREATEFILE"_s;
    case HRESULT(0x80010138) : return u"CO_E_FAILEDTOCLOSEHANDLE"_s;
    case HRESULT(0x80010139) : return u"CO_E_EXCEEDSYSACLLIMIT"_s;
    case HRESULT(0x8001013A) : return u"CO_E_ACESINWRONGORDER"_s;
    case HRESULT(0x8001013B) : return u"CO_E_INCOMPATIBLESTREAMVERSION"_s;
    case HRESULT(0x8001013C) : return u"CO_E_FAILEDTOOPENPROCESSTOKEN"_s;
    case HRESULT(0x8001013D) : return u"CO_E_DECODEFAILED"_s;
    case HRESULT(0x8001013F) : return u"CO_E_ACNOTINITIALIZED"_s;
    case HRESULT(0x80010140) : return u"CO_E_CANCEL_DISABLED"_s;
    case HRESULT(0x8001FFFF) : return u"RPC_E_UNEXPECTED"_s;
    case HRESULT(0xC0090001) : return u"ERROR_AUDITING_DISABLED"_s;
    case HRESULT(0xC0090002) : return u"ERROR_ALL_SIDS_FILTERED"_s;
    case HRESULT(0x80090001) : return u"NTE_BAD_UID"_s;
    case HRESULT(0x80090002) : return u"NTE_BAD_HASH"_s;
    case HRESULT(0x80090003) : return u"NTE_BAD_KEY"_s;
    case HRESULT(0x80090004) : return u"NTE_BAD_LEN"_s;
    case HRESULT(0x80090005) : return u"NTE_BAD_DATA"_s;
    case HRESULT(0x80090006) : return u"NTE_BAD_SIGNATURE"_s;
    case HRESULT(0x80090007) : return u"NTE_BAD_VER"_s;
    case HRESULT(0x80090008) : return u"NTE_BAD_ALGID"_s;
    case HRESULT(0x80090009) : return u"NTE_BAD_FLAGS"_s;
    case HRESULT(0x8009000A) : return u"NTE_BAD_TYPE"_s;
    case HRESULT(0x8009000B) : return u"NTE_BAD_KEY_STATE"_s;
    case HRESULT(0x8009000C) : return u"NTE_BAD_HASH_STATE"_s;
    case HRESULT(0x8009000D) : return u"NTE_NO_KEY"_s;
    case HRESULT(0x8009000E) : return u"NTE_NO_MEMORY"_s;
    case HRESULT(0x8009000F) : return u"NTE_EXISTS"_s;
    case HRESULT(0x80090010) : return u"NTE_PERM"_s;
    case HRESULT(0x80090011) : return u"NTE_NOT_FOUND"_s;
    case HRESULT(0x80090012) : return u"NTE_DOUBLE_ENCRYPT"_s;
    case HRESULT(0x80090013) : return u"NTE_BAD_PROVIDER"_s;
    case HRESULT(0x80090014) : return u"NTE_BAD_PROV_TYPE"_s;
    case HRESULT(0x80090015) : return u"NTE_BAD_PUBLIC_KEY"_s;
    case HRESULT(0x80090016) : return u"NTE_BAD_KEYSET"_s;
    case HRESULT(0x80090017) : return u"NTE_PROV_TYPE_NOT_DEF"_s;
    case HRESULT(0x80090018) : return u"NTE_PROV_TYPE_ENTRY_BAD"_s;
    case HRESULT(0x80090019) : return u"NTE_KEYSET_NOT_DEF"_s;
    case HRESULT(0x8009001A) : return u"NTE_KEYSET_ENTRY_BAD"_s;
    case HRESULT(0x8009001B) : return u"NTE_PROV_TYPE_NO_MATCH"_s;
    case HRESULT(0x8009001C) : return u"NTE_SIGNATURE_FILE_BAD"_s;
    case HRESULT(0x8009001D) : return u"NTE_PROVIDER_DLL_FAIL"_s;
    case HRESULT(0x8009001E) : return u"NTE_PROV_DLL_NOT_FOUND"_s;
    case HRESULT(0x8009001F) : return u"NTE_BAD_KEYSET_PARAM"_s;
    case HRESULT(0x80090020) : return u"NTE_FAIL"_s;
    case HRESULT(0x80090021) : return u"NTE_SYS_ERR"_s;
    case HRESULT(0x80090022) : return u"NTE_SILENT_CONTEXT"_s;
    case HRESULT(0x80090023) : return u"NTE_TOKEN_KEYSET_STORAGE_FULL"_s;
    case HRESULT(0x80090024) : return u"NTE_TEMPORARY_PROFILE"_s;
    case HRESULT(0x80090025) : return u"NTE_FIXEDPARAMETER"_s;
    case HRESULT(0x80090300) : return u"SEC_E_INSUFFICIENT_MEMORY"_s;
    case HRESULT(0x80090301) : return u"SEC_E_INVALID_HANDLE"_s;
    case HRESULT(0x80090302) : return u"SEC_E_UNSUPPORTED_FUNCTION"_s;
    case HRESULT(0x80090303) : return u"SEC_E_TARGET_UNKNOWN"_s;
    case HRESULT(0x80090304) : return u"SEC_E_INTERNAL_ERROR"_s;
    case HRESULT(0x80090305) : return u"SEC_E_SECPKG_NOT_FOUND"_s;
    case HRESULT(0x80090306) : return u"SEC_E_NOT_OWNER"_s;
    case HRESULT(0x80090307) : return u"SEC_E_CANNOT_INSTALL"_s;
    case HRESULT(0x80090308) : return u"SEC_E_INVALID_TOKEN"_s;
    case HRESULT(0x80090309) : return u"SEC_E_CANNOT_PACK"_s;
    case HRESULT(0x8009030A) : return u"SEC_E_QOP_NOT_SUPPORTED"_s;
    case HRESULT(0x8009030B) : return u"SEC_E_NO_IMPERSONATION"_s;
    case HRESULT(0x8009030C) : return u"SEC_E_LOGON_DENIED"_s;
    case HRESULT(0x8009030D) : return u"SEC_E_UNKNOWN_CREDENTIALS"_s;
    case HRESULT(0x8009030E) : return u"SEC_E_NO_CREDENTIALS"_s;
    case HRESULT(0x8009030F) : return u"SEC_E_MESSAGE_ALTERED"_s;
    case HRESULT(0x80090310) : return u"SEC_E_OUT_OF_SEQUENCE"_s;
    case HRESULT(0x80090311) : return u"SEC_E_NO_AUTHENTICATING_AUTHORITY"_s;
    case HRESULT(0x00090312) : return u"SEC_I_CONTINUE_NEEDED"_s;
    case HRESULT(0x00090313) : return u"SEC_I_COMPLETE_NEEDED"_s;
    case HRESULT(0x00090314) : return u"SEC_I_COMPLETE_AND_CONTINUE"_s;
    case HRESULT(0x00090315) : return u"SEC_I_LOCAL_LOGON"_s;
    case HRESULT(0x80090316) : return u"SEC_E_BAD_PKGID"_s;
    case HRESULT(0x80090317) : return u"SEC_E_CONTEXT_EXPIRED"_s;
    case HRESULT(0x00090317) : return u"SEC_I_CONTEXT_EXPIRED"_s;
    case HRESULT(0x80090318) : return u"SEC_E_INCOMPLETE_MESSAGE"_s;
    case HRESULT(0x80090320) : return u"SEC_E_INCOMPLETE_CREDENTIALS"_s;
    case HRESULT(0x80090321) : return u"SEC_E_BUFFER_TOO_SMALL"_s;
    case HRESULT(0x00090320) : return u"SEC_I_INCOMPLETE_CREDENTIALS"_s;
    case HRESULT(0x00090321) : return u"SEC_I_RENEGOTIATE"_s;
    case HRESULT(0x80090322) : return u"SEC_E_WRONG_PRINCIPAL"_s;
    case HRESULT(0x00090323) : return u"SEC_I_NO_LSA_CONTEXT"_s;
    case HRESULT(0x80090324) : return u"SEC_E_TIME_SKEW"_s;
    case HRESULT(0x80090325) : return u"SEC_E_UNTRUSTED_ROOT"_s;
    case HRESULT(0x80090326) : return u"SEC_E_ILLEGAL_MESSAGE"_s;
    case HRESULT(0x80090327) : return u"SEC_E_CERT_UNKNOWN"_s;
    case HRESULT(0x80090328) : return u"SEC_E_CERT_EXPIRED"_s;
    case HRESULT(0x80090329) : return u"SEC_E_ENCRYPT_FAILURE"_s;
    case HRESULT(0x80090330) : return u"SEC_E_DECRYPT_FAILURE"_s;
    case HRESULT(0x80090331) : return u"SEC_E_ALGORITHM_MISMATCH"_s;
    case HRESULT(0x80090332) : return u"SEC_E_SECURITY_QOS_FAILED"_s;
    case HRESULT(0x80090333) : return u"SEC_E_UNFINISHED_CONTEXT_DELETED"_s;
    case HRESULT(0x80090334) : return u"SEC_E_NO_TGT_REPLY"_s;
    case HRESULT(0x80090335) : return u"SEC_E_NO_IP_ADDRESSES"_s;
    case HRESULT(0x80090336) : return u"SEC_E_WRONG_CREDENTIAL_HANDLE"_s;
    case HRESULT(0x80090337) : return u"SEC_E_CRYPTO_SYSTEM_INVALID"_s;
    case HRESULT(0x80090338) : return u"SEC_E_MAX_REFERRALS_EXCEEDED"_s;
    case HRESULT(0x80090339) : return u"SEC_E_MUST_BE_KDC"_s;
    case HRESULT(0x8009033A) : return u"SEC_E_STRONG_CRYPTO_NOT_SUPPORTED"_s;
    case HRESULT(0x8009033B) : return u"SEC_E_TOO_MANY_PRINCIPALS"_s;
    case HRESULT(0x8009033C) : return u"SEC_E_NO_PA_DATA"_s;
    case HRESULT(0x8009033D) : return u"SEC_E_PKINIT_NAME_MISMATCH"_s;
    case HRESULT(0x8009033E) : return u"SEC_E_SMARTCARD_LOGON_REQUIRED"_s;
    case HRESULT(0x8009033F) : return u"SEC_E_SHUTDOWN_IN_PROGRESS"_s;
    case HRESULT(0x80090340) : return u"SEC_E_KDC_INVALID_REQUEST"_s;
    case HRESULT(0x80090341) : return u"SEC_E_KDC_UNABLE_TO_REFER"_s;
    case HRESULT(0x80090342) : return u"SEC_E_KDC_UNKNOWN_ETYPE"_s;
    case HRESULT(0x80090343) : return u"SEC_E_UNSUPPORTED_PREAUTH"_s;
    case HRESULT(0x80090345) : return u"SEC_E_DELEGATION_REQUIRED"_s;
    case HRESULT(0x80090346) : return u"SEC_E_BAD_BINDINGS"_s;
    case HRESULT(0x80090347) : return u"SEC_E_MULTIPLE_ACCOUNTS"_s;
    case HRESULT(0x80090348) : return u"SEC_E_NO_KERB_KEY"_s;
    case HRESULT(0x80090349) : return u"SEC_E_CERT_WRONG_USAGE"_s;
    case HRESULT(0x80090350) : return u"SEC_E_DOWNGRADE_DETECTED"_s;
    case HRESULT(0x80090351) : return u"SEC_E_SMARTCARD_CERT_REVOKED"_s;
    case HRESULT(0x80090352) : return u"SEC_E_ISSUING_CA_UNTRUSTED"_s;
    case HRESULT(0x80090353) : return u"SEC_E_REVOCATION_OFFLINE_C"_s;
    case HRESULT(0x80090354) : return u"SEC_E_PKINIT_CLIENT_FAILURE"_s;
    case HRESULT(0x80090355) : return u"SEC_E_SMARTCARD_CERT_EXPIRED"_s;
    case HRESULT(0x80090356) : return u"SEC_E_NO_S4U_PROT_SUPPORT"_s;
    case HRESULT(0x80090357) : return u"SEC_E_CROSSREALM_DELEGATION_FAILURE"_s;
    case HRESULT(0x80090358) : return u"SEC_E_REVOCATION_OFFLINE_KDC"_s;
    case HRESULT(0x80090359) : return u"SEC_E_ISSUING_CA_UNTRUSTED_KDC"_s;
    case HRESULT(0x8009035A) : return u"SEC_E_KDC_CERT_EXPIRED"_s;
    case HRESULT(0x8009035B) : return u"SEC_E_KDC_CERT_REVOKED"_s;
    case HRESULT(0x80091001) : return u"CRYPT_E_MSG_ERROR"_s;
    case HRESULT(0x80091002) : return u"CRYPT_E_UNKNOWN_ALGO"_s;
    case HRESULT(0x80091003) : return u"CRYPT_E_OID_FORMAT"_s;
    case HRESULT(0x80091004) : return u"CRYPT_E_INVALID_MSG_TYPE"_s;
    case HRESULT(0x80091005) : return u"CRYPT_E_UNEXPECTED_ENCODING"_s;
    case HRESULT(0x80091006) : return u"CRYPT_E_AUTH_ATTR_MISSING"_s;
    case HRESULT(0x80091007) : return u"CRYPT_E_HASH_VALUE"_s;
    case HRESULT(0x80091008) : return u"CRYPT_E_INVALID_INDEX"_s;
    case HRESULT(0x80091009) : return u"CRYPT_E_ALREADY_DECRYPTED"_s;
    case HRESULT(0x8009100A) : return u"CRYPT_E_NOT_DECRYPTED"_s;
    case HRESULT(0x8009100B) : return u"CRYPT_E_RECIPIENT_NOT_FOUND"_s;
    case HRESULT(0x8009100C) : return u"CRYPT_E_CONTROL_TYPE"_s;
    case HRESULT(0x8009100D) : return u"CRYPT_E_ISSUER_SERIALNUMBER"_s;
    case HRESULT(0x8009100E) : return u"CRYPT_E_SIGNER_NOT_FOUND"_s;
    case HRESULT(0x8009100F) : return u"CRYPT_E_ATTRIBUTES_MISSING"_s;
    case HRESULT(0x80091010) : return u"CRYPT_E_STREAM_MSG_NOT_READY"_s;
    case HRESULT(0x80091011) : return u"CRYPT_E_STREAM_INSUFFICIENT_DATA"_s;
    case HRESULT(0x00091012) : return u"CRYPT_I_NEW_PROTECTION_REQUIRED"_s;
    case HRESULT(0x80092001) : return u"CRYPT_E_BAD_LEN"_s;
    case HRESULT(0x80092002) : return u"CRYPT_E_BAD_ENCODE"_s;
    case HRESULT(0x80092003) : return u"CRYPT_E_FILE_ERROR"_s;
    case HRESULT(0x80092004) : return u"CRYPT_E_NOT_FOUND"_s;
    case HRESULT(0x80092005) : return u"CRYPT_E_EXISTS"_s;
    case HRESULT(0x80092006) : return u"CRYPT_E_NO_PROVIDER"_s;
    case HRESULT(0x80092007) : return u"CRYPT_E_SELF_SIGNED"_s;
    case HRESULT(0x80092008) : return u"CRYPT_E_DELETED_PREV"_s;
    case HRESULT(0x80092009) : return u"CRYPT_E_NO_MATCH"_s;
    case HRESULT(0x8009200A) : return u"CRYPT_E_UNEXPECTED_MSG_TYPE"_s;
    case HRESULT(0x8009200B) : return u"CRYPT_E_NO_KEY_PROPERTY"_s;
    case HRESULT(0x8009200C) : return u"CRYPT_E_NO_DECRYPT_CERT"_s;
    case HRESULT(0x8009200D) : return u"CRYPT_E_BAD_MSG"_s;
    case HRESULT(0x8009200E) : return u"CRYPT_E_NO_SIGNER"_s;
    case HRESULT(0x8009200F) : return u"CRYPT_E_PENDING_CLOSE"_s;
    case HRESULT(0x80092010) : return u"CRYPT_E_REVOKED"_s;
    case HRESULT(0x80092011) : return u"CRYPT_E_NO_REVOCATION_DLL"_s;
    case HRESULT(0x80092012) : return u"CRYPT_E_NO_REVOCATION_CHECK"_s;
    case HRESULT(0x80092013) : return u"CRYPT_E_REVOCATION_OFFLINE"_s;
    case HRESULT(0x80092014) : return u"CRYPT_E_NOT_IN_REVOCATION_DATABASE"_s;
    case HRESULT(0x80092020) : return u"CRYPT_E_INVALID_NUMERIC_STRING"_s;
    case HRESULT(0x80092021) : return u"CRYPT_E_INVALID_PRINTABLE_STRING"_s;
    case HRESULT(0x80092022) : return u"CRYPT_E_INVALID_IA5_STRING"_s;
    case HRESULT(0x80092023) : return u"CRYPT_E_INVALID_X500_STRING"_s;
    case HRESULT(0x80092024) : return u"CRYPT_E_NOT_CHAR_STRING"_s;
    case HRESULT(0x80092025) : return u"CRYPT_E_FILERESIZED"_s;
    case HRESULT(0x80092026) : return u"CRYPT_E_SECURITY_SETTINGS"_s;
    case HRESULT(0x80092027) : return u"CRYPT_E_NO_VERIFY_USAGE_DLL"_s;
    case HRESULT(0x80092028) : return u"CRYPT_E_NO_VERIFY_USAGE_CHECK"_s;
    case HRESULT(0x80092029) : return u"CRYPT_E_VERIFY_USAGE_OFFLINE"_s;
    case HRESULT(0x8009202A) : return u"CRYPT_E_NOT_IN_CTL"_s;
    case HRESULT(0x8009202B) : return u"CRYPT_E_NO_TRUSTED_SIGNER"_s;
    case HRESULT(0x8009202C) : return u"CRYPT_E_MISSING_PUBKEY_PARA"_s;
    case HRESULT(0x80093000) : return u"CRYPT_E_OSS_ERROR"_s;
    case HRESULT(0x80093001) : return u"OSS_MORE_BUF"_s;
    case HRESULT(0x80093002) : return u"OSS_NEGATIVE_UINTEGER"_s;
    case HRESULT(0x80093003) : return u"OSS_PDU_RANGE"_s;
    case HRESULT(0x80093004) : return u"OSS_MORE_INPUT"_s;
    case HRESULT(0x80093005) : return u"OSS_DATA_ERROR"_s;
    case HRESULT(0x80093006) : return u"OSS_BAD_ARG"_s;
    case HRESULT(0x80093007) : return u"OSS_BAD_VERSION"_s;
    case HRESULT(0x80093008) : return u"OSS_OUT_MEMORY"_s;
    case HRESULT(0x80093009) : return u"OSS_PDU_MISMATCH"_s;
    case HRESULT(0x8009300A) : return u"OSS_LIMITED"_s;
    case HRESULT(0x8009300B) : return u"OSS_BAD_PTR"_s;
    case HRESULT(0x8009300C) : return u"OSS_BAD_TIME"_s;
    case HRESULT(0x8009300D) : return u"OSS_INDEFINITE_NOT_SUPPORTED"_s;
    case HRESULT(0x8009300E) : return u"OSS_MEM_ERROR"_s;
    case HRESULT(0x8009300F) : return u"OSS_BAD_TABLE"_s;
    case HRESULT(0x80093010) : return u"OSS_TOO_LONG"_s;
    case HRESULT(0x80093011) : return u"OSS_CONSTRAINT_VIOLATED"_s;
    case HRESULT(0x80093012) : return u"OSS_FATAL_ERROR"_s;
    case HRESULT(0x80093013) : return u"OSS_ACCESS_SERIALIZATION_ERROR"_s;
    case HRESULT(0x80093014) : return u"OSS_NULL_TBL"_s;
    case HRESULT(0x80093015) : return u"OSS_NULL_FCN"_s;
    case HRESULT(0x80093016) : return u"OSS_BAD_ENCRULES"_s;
    case HRESULT(0x80093017) : return u"OSS_UNAVAIL_ENCRULES"_s;
    case HRESULT(0x80093018) : return u"OSS_CANT_OPEN_TRACE_WINDOW"_s;
    case HRESULT(0x80093019) : return u"OSS_UNIMPLEMENTED"_s;
    case HRESULT(0x8009301A) : return u"OSS_OID_DLL_NOT_LINKED"_s;
    case HRESULT(0x8009301B) : return u"OSS_CANT_OPEN_TRACE_FILE"_s;
    case HRESULT(0x8009301C) : return u"OSS_TRACE_FILE_ALREADY_OPEN"_s;
    case HRESULT(0x8009301D) : return u"OSS_TABLE_MISMATCH"_s;
    case HRESULT(0x8009301E) : return u"OSS_TYPE_NOT_SUPPORTED"_s;
    case HRESULT(0x8009301F) : return u"OSS_REAL_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093020) : return u"OSS_REAL_CODE_NOT_LINKED"_s;
    case HRESULT(0x80093021) : return u"OSS_OUT_OF_RANGE"_s;
    case HRESULT(0x80093022) : return u"OSS_COPIER_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093023) : return u"OSS_CONSTRAINT_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093024) : return u"OSS_COMPARATOR_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093025) : return u"OSS_COMPARATOR_CODE_NOT_LINKED"_s;
    case HRESULT(0x80093026) : return u"OSS_MEM_MGR_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093027) : return u"OSS_PDV_DLL_NOT_LINKED"_s;
    case HRESULT(0x80093028) : return u"OSS_PDV_CODE_NOT_LINKED"_s;
    case HRESULT(0x80093029) : return u"OSS_API_DLL_NOT_LINKED"_s;
    case HRESULT(0x8009302A) : return u"OSS_BERDER_DLL_NOT_LINKED"_s;
    case HRESULT(0x8009302B) : return u"OSS_PER_DLL_NOT_LINKED"_s;
    case HRESULT(0x8009302C) : return u"OSS_OPEN_TYPE_ERROR"_s;
    case HRESULT(0x8009302D) : return u"OSS_MUTEX_NOT_CREATED"_s;
    case HRESULT(0x8009302E) : return u"OSS_CANT_CLOSE_TRACE_FILE"_s;
    case HRESULT(0x80093100) : return u"CRYPT_E_ASN1_ERROR"_s;
    case HRESULT(0x80093101) : return u"CRYPT_E_ASN1_INTERNAL"_s;
    case HRESULT(0x80093102) : return u"CRYPT_E_ASN1_EOD"_s;
    case HRESULT(0x80093103) : return u"CRYPT_E_ASN1_CORRUPT"_s;
    case HRESULT(0x80093104) : return u"CRYPT_E_ASN1_LARGE"_s;
    case HRESULT(0x80093105) : return u"CRYPT_E_ASN1_CONSTRAINT"_s;
    case HRESULT(0x80093106) : return u"CRYPT_E_ASN1_MEMORY"_s;
    case HRESULT(0x80093107) : return u"CRYPT_E_ASN1_OVERFLOW"_s;
    case HRESULT(0x80093108) : return u"CRYPT_E_ASN1_BADPDU"_s;
    case HRESULT(0x80093109) : return u"CRYPT_E_ASN1_BADARGS"_s;
    case HRESULT(0x8009310A) : return u"CRYPT_E_ASN1_BADREAL"_s;
    case HRESULT(0x8009310B) : return u"CRYPT_E_ASN1_BADTAG"_s;
    case HRESULT(0x8009310C) : return u"CRYPT_E_ASN1_CHOICE"_s;
    case HRESULT(0x8009310D) : return u"CRYPT_E_ASN1_RULE"_s;
    case HRESULT(0x8009310E) : return u"CRYPT_E_ASN1_UTF8"_s;
    case HRESULT(0x80093133) : return u"CRYPT_E_ASN1_PDU_TYPE"_s;
    case HRESULT(0x80093134) : return u"CRYPT_E_ASN1_NYI"_s;
    case HRESULT(0x80093201) : return u"CRYPT_E_ASN1_EXTENDED"_s;
    case HRESULT(0x80093202) : return u"CRYPT_E_ASN1_NOEOD"_s;
    case HRESULT(0x80094001) : return u"CERTSRV_E_BAD_REQUESTSUBJECT"_s;
    case HRESULT(0x80094002) : return u"CERTSRV_E_NO_REQUEST"_s;
    case HRESULT(0x80094003) : return u"CERTSRV_E_BAD_REQUESTSTATUS"_s;
    case HRESULT(0x80094004) : return u"CERTSRV_E_PROPERTY_EMPTY"_s;
    case HRESULT(0x80094005) : return u"CERTSRV_E_INVALID_CA_CERTIFICATE"_s;
    case HRESULT(0x80094006) : return u"CERTSRV_E_SERVER_SUSPENDED"_s;
    case HRESULT(0x80094007) : return u"CERTSRV_E_ENCODING_LENGTH"_s;
    case HRESULT(0x80094008) : return u"CERTSRV_E_ROLECONFLICT"_s;
    case HRESULT(0x80094009) : return u"CERTSRV_E_RESTRICTEDOFFICER"_s;
    case HRESULT(0x8009400A) : return u"CERTSRV_E_KEY_ARCHIVAL_NOT_CONFIGURED"_s;
    case HRESULT(0x8009400B) : return u"CERTSRV_E_NO_VALID_KRA"_s;
    case HRESULT(0x8009400C) : return u"CERTSRV_E_BAD_REQUEST_KEY_ARCHIVAL"_s;
    case HRESULT(0x8009400D) : return u"CERTSRV_E_NO_CAADMIN_DEFINED"_s;
    case HRESULT(0x8009400E) : return u"CERTSRV_E_BAD_RENEWAL_CERT_ATTRIBUTE"_s;
    case HRESULT(0x8009400F) : return u"CERTSRV_E_NO_DB_SESSIONS"_s;
    case HRESULT(0x80094010) : return u"CERTSRV_E_ALIGNMENT_FAULT"_s;
    case HRESULT(0x80094011) : return u"CERTSRV_E_ENROLL_DENIED"_s;
    case HRESULT(0x80094012) : return u"CERTSRV_E_TEMPLATE_DENIED"_s;
    case HRESULT(0x80094013) : return u"CERTSRV_E_DOWNLEVEL_DC_SSL_OR_UPGRADE"_s;
    case HRESULT(0x80094800) : return u"CERTSRV_E_UNSUPPORTED_CERT_TYPE"_s;
    case HRESULT(0x80094801) : return u"CERTSRV_E_NO_CERT_TYPE"_s;
    case HRESULT(0x80094802) : return u"CERTSRV_E_TEMPLATE_CONFLICT"_s;
    case HRESULT(0x80094803) : return u"CERTSRV_E_SUBJECT_ALT_NAME_REQUIRED"_s;
    case HRESULT(0x80094804) : return u"CERTSRV_E_ARCHIVED_KEY_REQUIRED"_s;
    case HRESULT(0x80094805) : return u"CERTSRV_E_SMIME_REQUIRED"_s;
    case HRESULT(0x80094806) : return u"CERTSRV_E_BAD_RENEWAL_SUBJECT"_s;
    case HRESULT(0x80094807) : return u"CERTSRV_E_BAD_TEMPLATE_VERSION"_s;
    case HRESULT(0x80094808) : return u"CERTSRV_E_TEMPLATE_POLICY_REQUIRED"_s;
    case HRESULT(0x80094809) : return u"CERTSRV_E_SIGNATURE_POLICY_REQUIRED"_s;
    case HRESULT(0x8009480A) : return u"CERTSRV_E_SIGNATURE_COUNT"_s;
    case HRESULT(0x8009480B) : return u"CERTSRV_E_SIGNATURE_REJECTED"_s;
    case HRESULT(0x8009480C) : return u"CERTSRV_E_ISSUANCE_POLICY_REQUIRED"_s;
    case HRESULT(0x8009480D) : return u"CERTSRV_E_SUBJECT_UPN_REQUIRED"_s;
    case HRESULT(0x8009480E) : return u"CERTSRV_E_SUBJECT_DIRECTORY_GUID_REQUIRED"_s;
    case HRESULT(0x8009480F) : return u"CERTSRV_E_SUBJECT_DNS_REQUIRED"_s;
    case HRESULT(0x80094810) : return u"CERTSRV_E_ARCHIVED_KEY_UNEXPECTED"_s;
    case HRESULT(0x80094811) : return u"CERTSRV_E_KEY_LENGTH"_s;
    case HRESULT(0x80094812) : return u"CERTSRV_E_SUBJECT_EMAIL_REQUIRED"_s;
    case HRESULT(0x80094813) : return u"CERTSRV_E_UNKNOWN_CERT_TYPE"_s;
    case HRESULT(0x80094814) : return u"CERTSRV_E_CERT_TYPE_OVERLAP"_s;
    case HRESULT(0x80095000) : return u"XENROLL_E_KEY_NOT_EXPORTABLE"_s;
    case HRESULT(0x80095001) : return u"XENROLL_E_CANNOT_ADD_ROOT_CERT"_s;
    case HRESULT(0x80095002) : return u"XENROLL_E_RESPONSE_KA_HASH_NOT_FOUND"_s;
    case HRESULT(0x80095003) : return u"XENROLL_E_RESPONSE_UNEXPECTED_KA_HASH"_s;
    case HRESULT(0x80095004) : return u"XENROLL_E_RESPONSE_KA_HASH_MISMATCH"_s;
    case HRESULT(0x80095005) : return u"XENROLL_E_KEYSPEC_SMIME_MISMATCH"_s;
    case HRESULT(0x80096001) : return u"TRUST_E_SYSTEM_ERROR"_s;
    case HRESULT(0x80096002) : return u"TRUST_E_NO_SIGNER_CERT"_s;
    case HRESULT(0x80096003) : return u"TRUST_E_COUNTER_SIGNER"_s;
    case HRESULT(0x80096004) : return u"TRUST_E_CERT_SIGNATURE"_s;
    case HRESULT(0x80096005) : return u"TRUST_E_TIME_STAMP"_s;
    case HRESULT(0x80096010) : return u"TRUST_E_BAD_DIGEST"_s;
    case HRESULT(0x80096019) : return u"TRUST_E_BASIC_CONSTRAINTS"_s;
    case HRESULT(0x8009601E) : return u"TRUST_E_FINANCIAL_CRITERIA"_s;
    case HRESULT(0x80097001) : return u"MSSIPOTF_E_OUTOFMEMRANGE"_s;
    case HRESULT(0x80097002) : return u"MSSIPOTF_E_CANTGETOBJECT"_s;
    case HRESULT(0x80097003) : return u"MSSIPOTF_E_NOHEADTABLE"_s;
    case HRESULT(0x80097004) : return u"MSSIPOTF_E_BAD_MAGICNUMBER"_s;
    case HRESULT(0x80097005) : return u"MSSIPOTF_E_BAD_OFFSET_TABLE"_s;
    case HRESULT(0x80097006) : return u"MSSIPOTF_E_TABLE_TAGORDER"_s;
    case HRESULT(0x80097007) : return u"MSSIPOTF_E_TABLE_LONGWORD"_s;
    case HRESULT(0x80097008) : return u"MSSIPOTF_E_BAD_FIRST_TABLE_PLACEMENT"_s;
    case HRESULT(0x80097009) : return u"MSSIPOTF_E_TABLES_OVERLAP"_s;
    case HRESULT(0x8009700A) : return u"MSSIPOTF_E_TABLE_PADBYTES"_s;
    case HRESULT(0x8009700B) : return u"MSSIPOTF_E_FILETOOSMALL"_s;
    case HRESULT(0x8009700C) : return u"MSSIPOTF_E_TABLE_CHECKSUM"_s;
    case HRESULT(0x8009700D) : return u"MSSIPOTF_E_FILE_CHECKSUM"_s;
    case HRESULT(0x80097010) : return u"MSSIPOTF_E_FAILED_POLICY"_s;
    case HRESULT(0x80097011) : return u"MSSIPOTF_E_FAILED_HINTS_CHECK"_s;
    case HRESULT(0x80097012) : return u"MSSIPOTF_E_NOT_OPENTYPE"_s;
    case HRESULT(0x80097013) : return u"MSSIPOTF_E_FILE"_s;
    case HRESULT(0x80097014) : return u"MSSIPOTF_E_CRYPT"_s;
    case HRESULT(0x80097015) : return u"MSSIPOTF_E_BADVERSION"_s;
    case HRESULT(0x80097016) : return u"MSSIPOTF_E_DSIG_STRUCTURE"_s;
    case HRESULT(0x80097017) : return u"MSSIPOTF_E_PCONST_CHECK"_s;
    case HRESULT(0x80097018) : return u"MSSIPOTF_E_STRUCTURE"_s;
    case HRESULT(0x800B0001) : return u"TRUST_E_PROVIDER_UNKNOWN"_s;
    case HRESULT(0x800B0002) : return u"TRUST_E_ACTION_UNKNOWN"_s;
    case HRESULT(0x800B0003) : return u"TRUST_E_SUBJECT_FORM_UNKNOWN"_s;
    case HRESULT(0x800B0004) : return u"TRUST_E_SUBJECT_NOT_TRUSTED"_s;
    case HRESULT(0x800B0005) : return u"DIGSIG_E_ENCODE"_s;
    case HRESULT(0x800B0006) : return u"DIGSIG_E_DECODE"_s;
    case HRESULT(0x800B0007) : return u"DIGSIG_E_EXTENSIBILITY"_s;
    case HRESULT(0x800B0008) : return u"DIGSIG_E_CRYPTO"_s;
    case HRESULT(0x800B0009) : return u"PERSIST_E_SIZEDEFINITE"_s;
    case HRESULT(0x800B000A) : return u"PERSIST_E_SIZEINDEFINITE"_s;
    case HRESULT(0x800B000B) : return u"PERSIST_E_NOTSELFSIZING"_s;
    case HRESULT(0x800B0100) : return u"TRUST_E_NOSIGNATURE"_s;
    case HRESULT(0x800B0101) : return u"CERT_E_EXPIRED"_s;
    case HRESULT(0x800B0102) : return u"CERT_E_VALIDITYPERIODNESTING"_s;
    case HRESULT(0x800B0103) : return u"CERT_E_ROLE"_s;
    case HRESULT(0x800B0104) : return u"CERT_E_PATHLENCONST"_s;
    case HRESULT(0x800B0105) : return u"CERT_E_CRITICAL"_s;
    case HRESULT(0x800B0106) : return u"CERT_E_PURPOSE"_s;
    case HRESULT(0x800B0107) : return u"CERT_E_ISSUERCHAINING"_s;
    case HRESULT(0x800B0108) : return u"CERT_E_MALFORMED"_s;
    case HRESULT(0x800B0109) : return u"CERT_E_UNTRUSTEDROOT"_s;
    case HRESULT(0x800B010A) : return u"CERT_E_CHAINING"_s;
    case HRESULT(0x800B010B) : return u"TRUST_E_FAIL"_s;
    case HRESULT(0x800B010C) : return u"CERT_E_REVOKED"_s;
    case HRESULT(0x800B010D) : return u"CERT_E_UNTRUSTEDTESTROOT"_s;
    case HRESULT(0x800B010E) : return u"CERT_E_REVOCATION_FAILURE"_s;
    case HRESULT(0x800B010F) : return u"CERT_E_CN_NO_MATCH"_s;
    case HRESULT(0x800B0110) : return u"CERT_E_WRONG_USAGE"_s;
    case HRESULT(0x800B0111) : return u"TRUST_E_EXPLICIT_DISTRUST"_s;
    case HRESULT(0x800B0112) : return u"CERT_E_UNTRUSTEDCA"_s;
    case HRESULT(0x800B0113) : return u"CERT_E_INVALID_POLICY"_s;
    case HRESULT(0x800B0114) : return u"CERT_E_INVALID_NAME"_s;
    case HRESULT(0x800F0000) : return u"SPAPI_E_EXPECTED_SECTION_NAME"_s;
    case HRESULT(0x800F0001) : return u"SPAPI_E_BAD_SECTION_NAME_LINE"_s;
    case HRESULT(0x800F0002) : return u"SPAPI_E_SECTION_NAME_TOO_LONG"_s;
    case HRESULT(0x800F0003) : return u"SPAPI_E_GENERAL_SYNTAX"_s;
    case HRESULT(0x800F0100) : return u"SPAPI_E_WRONG_INF_STYLE"_s;
    case HRESULT(0x800F0101) : return u"SPAPI_E_SECTION_NOT_FOUND"_s;
    case HRESULT(0x800F0102) : return u"SPAPI_E_LINE_NOT_FOUND"_s;
    case HRESULT(0x800F0103) : return u"SPAPI_E_NO_BACKUP"_s;
    case HRESULT(0x800F0200) : return u"SPAPI_E_NO_ASSOCIATED_CLASS"_s;
    case HRESULT(0x800F0201) : return u"SPAPI_E_CLASS_MISMATCH"_s;
    case HRESULT(0x800F0202) : return u"SPAPI_E_DUPLICATE_FOUND"_s;
    case HRESULT(0x800F0203) : return u"SPAPI_E_NO_DRIVER_SELECTED"_s;
    case HRESULT(0x800F0204) : return u"SPAPI_E_KEY_DOES_NOT_EXIST"_s;
    case HRESULT(0x800F0205) : return u"SPAPI_E_INVALID_DEVINST_NAME"_s;
    case HRESULT(0x800F0206) : return u"SPAPI_E_INVALID_CLASS"_s;
    case HRESULT(0x800F0207) : return u"SPAPI_E_DEVINST_ALREADY_EXISTS"_s;
    case HRESULT(0x800F0208) : return u"SPAPI_E_DEVINFO_NOT_REGISTERED"_s;
    case HRESULT(0x800F0209) : return u"SPAPI_E_INVALID_REG_PROPERTY"_s;
    case HRESULT(0x800F020A) : return u"SPAPI_E_NO_INF"_s;
    case HRESULT(0x800F020B) : return u"SPAPI_E_NO_SUCH_DEVINST"_s;
    case HRESULT(0x800F020C) : return u"SPAPI_E_CANT_LOAD_CLASS_ICON"_s;
    case HRESULT(0x800F020D) : return u"SPAPI_E_INVALID_CLASS_INSTALLER"_s;
    case HRESULT(0x800F020E) : return u"SPAPI_E_DI_DO_DEFAULT"_s;
    case HRESULT(0x800F020F) : return u"SPAPI_E_DI_NOFILECOPY"_s;
    case HRESULT(0x800F0210) : return u"SPAPI_E_INVALID_HWPROFILE"_s;
    case HRESULT(0x800F0211) : return u"SPAPI_E_NO_DEVICE_SELECTED"_s;
    case HRESULT(0x800F0212) : return u"SPAPI_E_DEVINFO_LIST_LOCKED"_s;
    case HRESULT(0x800F0213) : return u"SPAPI_E_DEVINFO_DATA_LOCKED"_s;
    case HRESULT(0x800F0214) : return u"SPAPI_E_DI_BAD_PATH"_s;
    case HRESULT(0x800F0215) : return u"SPAPI_E_NO_CLASSINSTALL_PARAMS"_s;
    case HRESULT(0x800F0216) : return u"SPAPI_E_FILEQUEUE_LOCKED"_s;
    case HRESULT(0x800F0217) : return u"SPAPI_E_BAD_SERVICE_INSTALLSECT"_s;
    case HRESULT(0x800F0218) : return u"SPAPI_E_NO_CLASS_DRIVER_LIST"_s;
    case HRESULT(0x800F0219) : return u"SPAPI_E_NO_ASSOCIATED_SERVICE"_s;
    case HRESULT(0x800F021A) : return u"SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE"_s;
    case HRESULT(0x800F021B) : return u"SPAPI_E_DEVICE_INTERFACE_ACTIVE"_s;
    case HRESULT(0x800F021C) : return u"SPAPI_E_DEVICE_INTERFACE_REMOVED"_s;
    case HRESULT(0x800F021D) : return u"SPAPI_E_BAD_INTERFACE_INSTALLSECT"_s;
    case HRESULT(0x800F021E) : return u"SPAPI_E_NO_SUCH_INTERFACE_CLASS"_s;
    case HRESULT(0x800F021F) : return u"SPAPI_E_INVALID_REFERENCE_STRING"_s;
    case HRESULT(0x800F0220) : return u"SPAPI_E_INVALID_MACHINENAME"_s;
    case HRESULT(0x800F0221) : return u"SPAPI_E_REMOTE_COMM_FAILURE"_s;
    case HRESULT(0x800F0222) : return u"SPAPI_E_MACHINE_UNAVAILABLE"_s;
    case HRESULT(0x800F0223) : return u"SPAPI_E_NO_CONFIGMGR_SERVICES"_s;
    case HRESULT(0x800F0224) : return u"SPAPI_E_INVALID_PROPPAGE_PROVIDER"_s;
    case HRESULT(0x800F0225) : return u"SPAPI_E_NO_SUCH_DEVICE_INTERFACE"_s;
    case HRESULT(0x800F0226) : return u"SPAPI_E_DI_POSTPROCESSING_REQUIRED"_s;
    case HRESULT(0x800F0227) : return u"SPAPI_E_INVALID_COINSTALLER"_s;
    case HRESULT(0x800F0228) : return u"SPAPI_E_NO_COMPAT_DRIVERS"_s;
    case HRESULT(0x800F0229) : return u"SPAPI_E_NO_DEVICE_ICON"_s;
    case HRESULT(0x800F022A) : return u"SPAPI_E_INVALID_INF_LOGCONFIG"_s;
    case HRESULT(0x800F022B) : return u"SPAPI_E_DI_DONT_INSTALL"_s;
    case HRESULT(0x800F022C) : return u"SPAPI_E_INVALID_FILTER_DRIVER"_s;
    case HRESULT(0x800F022D) : return u"SPAPI_E_NON_WINDOWS_NT_DRIVER"_s;
    case HRESULT(0x800F022E) : return u"SPAPI_E_NON_WINDOWS_DRIVER"_s;
    case HRESULT(0x800F022F) : return u"SPAPI_E_NO_CATALOG_FOR_OEM_INF"_s;
    case HRESULT(0x800F0230) : return u"SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE"_s;
    case HRESULT(0x800F0231) : return u"SPAPI_E_NOT_DISABLEABLE"_s;
    case HRESULT(0x800F0232) : return u"SPAPI_E_CANT_REMOVE_DEVINST"_s;
    case HRESULT(0x800F0233) : return u"SPAPI_E_INVALID_TARGET"_s;
    case HRESULT(0x800F0234) : return u"SPAPI_E_DRIVER_NONNATIVE"_s;
    case HRESULT(0x800F0235) : return u"SPAPI_E_IN_WOW64"_s;
    case HRESULT(0x800F0236) : return u"SPAPI_E_SET_SYSTEM_RESTORE_POINT"_s;
    case HRESULT(0x800F0237) : return u"SPAPI_E_INCORRECTLY_COPIED_INF"_s;
    case HRESULT(0x800F0238) : return u"SPAPI_E_SCE_DISABLED"_s;
    case HRESULT(0x800F0239) : return u"SPAPI_E_UNKNOWN_EXCEPTION"_s;
    case HRESULT(0x800F023A) : return u"SPAPI_E_PNP_REGISTRY_ERROR"_s;
    case HRESULT(0x800F023B) : return u"SPAPI_E_REMOTE_REQUEST_UNSUPPORTED"_s;
    case HRESULT(0x800F023C) : return u"SPAPI_E_NOT_AN_INSTALLED_OEM_INF"_s;
    case HRESULT(0x800F023D) : return u"SPAPI_E_INF_IN_USE_BY_DEVICES"_s;
    case HRESULT(0x800F023E) : return u"SPAPI_E_DI_FUNCTION_OBSOLETE"_s;
    case HRESULT(0x800F023F) : return u"SPAPI_E_NO_AUTHENTICODE_CATALOG"_s;
    case HRESULT(0x800F0240) : return u"SPAPI_E_AUTHENTICODE_DISALLOWED"_s;
    case HRESULT(0x800F0241) : return u"SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER"_s;
    case HRESULT(0x800F0242) : return u"SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED"_s;
    case HRESULT(0x800F0243) : return u"SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED"_s;
    case HRESULT(0x800F0244) : return u"SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH"_s;
    case HRESULT(0x800F0245) : return u"SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE"_s;
    case HRESULT(0x800F0300) : return u"SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW"_s;
    case HRESULT(0x800F1000) : return u"SPAPI_E_ERROR_NOT_INSTALLED"_s;
    case HRESULT(0x80100001) : return u"SCARD_F_INTERNAL_ERROR"_s;
    case HRESULT(0x80100002) : return u"SCARD_E_CANCELLED"_s;
    case HRESULT(0x80100003) : return u"SCARD_E_INVALID_HANDLE"_s;
    case HRESULT(0x80100004) : return u"SCARD_E_INVALID_PARAMETER"_s;
    case HRESULT(0x80100005) : return u"SCARD_E_INVALID_TARGET"_s;
    case HRESULT(0x80100006) : return u"SCARD_E_NO_MEMORY"_s;
    case HRESULT(0x80100007) : return u"SCARD_F_WAITED_TOO_LONG"_s;
    case HRESULT(0x80100008) : return u"SCARD_E_INSUFFICIENT_BUFFER"_s;
    case HRESULT(0x80100009) : return u"SCARD_E_UNKNOWN_READER"_s;
    case HRESULT(0x8010000A) : return u"SCARD_E_TIMEOUT"_s;
    case HRESULT(0x8010000B) : return u"SCARD_E_SHARING_VIOLATION"_s;
    case HRESULT(0x8010000C) : return u"SCARD_E_NO_SMARTCARD"_s;
    case HRESULT(0x8010000D) : return u"SCARD_E_UNKNOWN_CARD"_s;
    case HRESULT(0x8010000E) : return u"SCARD_E_CANT_DISPOSE"_s;
    case HRESULT(0x8010000F) : return u"SCARD_E_PROTO_MISMATCH"_s;
    case HRESULT(0x80100010) : return u"SCARD_E_NOT_READY"_s;
    case HRESULT(0x80100011) : return u"SCARD_E_INVALID_VALUE"_s;
    case HRESULT(0x80100012) : return u"SCARD_E_SYSTEM_CANCELLED"_s;
    case HRESULT(0x80100013) : return u"SCARD_F_COMM_ERROR"_s;
    case HRESULT(0x80100014) : return u"SCARD_F_UNKNOWN_ERROR"_s;
    case HRESULT(0x80100015) : return u"SCARD_E_INVALID_ATR"_s;
    case HRESULT(0x80100016) : return u"SCARD_E_NOT_TRANSACTED"_s;
    case HRESULT(0x80100017) : return u"SCARD_E_READER_UNAVAILABLE"_s;
    case HRESULT(0x80100018) : return u"SCARD_P_SHUTDOWN"_s;
    case HRESULT(0x80100019) : return u"SCARD_E_PCI_TOO_SMALL"_s;
    case HRESULT(0x8010001A) : return u"SCARD_E_READER_UNSUPPORTED"_s;
    case HRESULT(0x8010001B) : return u"SCARD_E_DUPLICATE_READER"_s;
    case HRESULT(0x8010001C) : return u"SCARD_E_CARD_UNSUPPORTED"_s;
    case HRESULT(0x8010001D) : return u"SCARD_E_NO_SERVICE"_s;
    case HRESULT(0x8010001E) : return u"SCARD_E_SERVICE_STOPPED"_s;
    case HRESULT(0x8010001F) : return u"SCARD_E_UNEXPECTED"_s;
    case HRESULT(0x80100020) : return u"SCARD_E_ICC_INSTALLATION"_s;
    case HRESULT(0x80100021) : return u"SCARD_E_ICC_CREATEORDER"_s;
    case HRESULT(0x80100022) : return u"SCARD_E_UNSUPPORTED_FEATURE"_s;
    case HRESULT(0x80100023) : return u"SCARD_E_DIR_NOT_FOUND"_s;
    case HRESULT(0x80100024) : return u"SCARD_E_FILE_NOT_FOUND"_s;
    case HRESULT(0x80100025) : return u"SCARD_E_NO_DIR"_s;
    case HRESULT(0x80100026) : return u"SCARD_E_NO_FILE"_s;
    case HRESULT(0x80100027) : return u"SCARD_E_NO_ACCESS"_s;
    case HRESULT(0x80100028) : return u"SCARD_E_WRITE_TOO_MANY"_s;
    case HRESULT(0x80100029) : return u"SCARD_E_BAD_SEEK"_s;
    case HRESULT(0x8010002A) : return u"SCARD_E_INVALID_CHV"_s;
    case HRESULT(0x8010002B) : return u"SCARD_E_UNKNOWN_RES_MNG"_s;
    case HRESULT(0x8010002C) : return u"SCARD_E_NO_SUCH_CERTIFICATE"_s;
    case HRESULT(0x8010002D) : return u"SCARD_E_CERTIFICATE_UNAVAILABLE"_s;
    case HRESULT(0x8010002E) : return u"SCARD_E_NO_READERS_AVAILABLE"_s;
    case HRESULT(0x8010002F) : return u"SCARD_E_COMM_DATA_LOST"_s;
    case HRESULT(0x80100030) : return u"SCARD_E_NO_KEY_CONTAINER"_s;
    case HRESULT(0x80100031) : return u"SCARD_E_SERVER_TOO_BUSY"_s;
    case HRESULT(0x80100065) : return u"SCARD_W_UNSUPPORTED_CARD"_s;
    case HRESULT(0x80100066) : return u"SCARD_W_UNRESPONSIVE_CARD"_s;
    case HRESULT(0x80100067) : return u"SCARD_W_UNPOWERED_CARD"_s;
    case HRESULT(0x80100068) : return u"SCARD_W_RESET_CARD"_s;
    case HRESULT(0x80100069) : return u"SCARD_W_REMOVED_CARD"_s;
    case HRESULT(0x8010006A) : return u"SCARD_W_SECURITY_VIOLATION"_s;
    case HRESULT(0x8010006B) : return u"SCARD_W_WRONG_CHV"_s;
    case HRESULT(0x8010006C) : return u"SCARD_W_CHV_BLOCKED"_s;
    case HRESULT(0x8010006D) : return u"SCARD_W_EOF"_s;
    case HRESULT(0x8010006E) : return u"SCARD_W_CANCELLED_BY_USER"_s;
    case HRESULT(0x8010006F) : return u"SCARD_W_CARD_NOT_AUTHENTICATED"_s;
    case HRESULT(0x80100070) : return u"SCARD_W_CACHE_ITEM_NOT_FOUND"_s;
    case HRESULT(0x80100071) : return u"SCARD_W_CACHE_ITEM_STALE"_s;
    case HRESULT(0x80110401) : return u"COMADMIN_E_OBJECTERRORS"_s;
    case HRESULT(0x80110402) : return u"COMADMIN_E_OBJECTINVALID"_s;
    case HRESULT(0x80110403) : return u"COMADMIN_E_KEYMISSING"_s;
    case HRESULT(0x80110404) : return u"COMADMIN_E_ALREADYINSTALLED"_s;
    case HRESULT(0x80110407) : return u"COMADMIN_E_APP_FILE_WRITEFAIL"_s;
    case HRESULT(0x80110408) : return u"COMADMIN_E_APP_FILE_READFAIL"_s;
    case HRESULT(0x80110409) : return u"COMADMIN_E_APP_FILE_VERSION"_s;
    case HRESULT(0x8011040A) : return u"COMADMIN_E_BADPATH"_s;
    case HRESULT(0x8011040B) : return u"COMADMIN_E_APPLICATIONEXISTS"_s;
    case HRESULT(0x8011040C) : return u"COMADMIN_E_ROLEEXISTS"_s;
    case HRESULT(0x8011040D) : return u"COMADMIN_E_CANTCOPYFILE"_s;
    case HRESULT(0x8011040F) : return u"COMADMIN_E_NOUSER"_s;
    case HRESULT(0x80110410) : return u"COMADMIN_E_INVALIDUSERIDS"_s;
    case HRESULT(0x80110411) : return u"COMADMIN_E_NOREGISTRYCLSID"_s;
    case HRESULT(0x80110412) : return u"COMADMIN_E_BADREGISTRYPROGID"_s;
    case HRESULT(0x80110413) : return u"COMADMIN_E_AUTHENTICATIONLEVEL"_s;
    case HRESULT(0x80110414) : return u"COMADMIN_E_USERPASSWDNOTVALID"_s;
    case HRESULT(0x80110418) : return u"COMADMIN_E_CLSIDORIIDMISMATCH"_s;
    case HRESULT(0x80110419) : return u"COMADMIN_E_REMOTEINTERFACE"_s;
    case HRESULT(0x8011041A) : return u"COMADMIN_E_DLLREGISTERSERVER"_s;
    case HRESULT(0x8011041B) : return u"COMADMIN_E_NOSERVERSHARE"_s;
    case HRESULT(0x8011041D) : return u"COMADMIN_E_DLLLOADFAILED"_s;
    case HRESULT(0x8011041E) : return u"COMADMIN_E_BADREGISTRYLIBID"_s;
    case HRESULT(0x8011041F) : return u"COMADMIN_E_APPDIRNOTFOUND"_s;
    case HRESULT(0x80110423) : return u"COMADMIN_E_REGISTRARFAILED"_s;
    case HRESULT(0x80110424) : return u"COMADMIN_E_COMPFILE_DOESNOTEXIST"_s;
    case HRESULT(0x80110425) : return u"COMADMIN_E_COMPFILE_LOADDLLFAIL"_s;
    case HRESULT(0x80110426) : return u"COMADMIN_E_COMPFILE_GETCLASSOBJ"_s;
    case HRESULT(0x80110427) : return u"COMADMIN_E_COMPFILE_CLASSNOTAVAIL"_s;
    case HRESULT(0x80110428) : return u"COMADMIN_E_COMPFILE_BADTLB"_s;
    case HRESULT(0x80110429) : return u"COMADMIN_E_COMPFILE_NOTINSTALLABLE"_s;
    case HRESULT(0x8011042A) : return u"COMADMIN_E_NOTCHANGEABLE"_s;
    case HRESULT(0x8011042B) : return u"COMADMIN_E_NOTDELETEABLE"_s;
    case HRESULT(0x8011042C) : return u"COMADMIN_E_SESSION"_s;
    case HRESULT(0x8011042D) : return u"COMADMIN_E_COMP_MOVE_LOCKED"_s;
    case HRESULT(0x8011042E) : return u"COMADMIN_E_COMP_MOVE_BAD_DEST"_s;
    case HRESULT(0x80110430) : return u"COMADMIN_E_REGISTERTLB"_s;
    case HRESULT(0x80110433) : return u"COMADMIN_E_SYSTEMAPP"_s;
    case HRESULT(0x80110434) : return u"COMADMIN_E_COMPFILE_NOREGISTRAR"_s;
    case HRESULT(0x80110435) : return u"COMADMIN_E_COREQCOMPINSTALLED"_s;
    case HRESULT(0x80110436) : return u"COMADMIN_E_SERVICENOTINSTALLED"_s;
    case HRESULT(0x80110437) : return u"COMADMIN_E_PROPERTYSAVEFAILED"_s;
    case HRESULT(0x80110438) : return u"COMADMIN_E_OBJECTEXISTS"_s;
    case HRESULT(0x80110439) : return u"COMADMIN_E_COMPONENTEXISTS"_s;
    case HRESULT(0x8011043B) : return u"COMADMIN_E_REGFILE_CORRUPT"_s;
    case HRESULT(0x8011043C) : return u"COMADMIN_E_PROPERTY_OVERFLOW"_s;
    case HRESULT(0x8011043E) : return u"COMADMIN_E_NOTINREGISTRY"_s;
    case HRESULT(0x8011043F) : return u"COMADMIN_E_OBJECTNOTPOOLABLE"_s;
    case HRESULT(0x80110446) : return u"COMADMIN_E_APPLID_MATCHES_CLSID"_s;
    case HRESULT(0x80110447) : return u"COMADMIN_E_ROLE_DOES_NOT_EXIST"_s;
    case HRESULT(0x80110448) : return u"COMADMIN_E_START_APP_NEEDS_COMPONENTS"_s;
    case HRESULT(0x80110449) : return u"COMADMIN_E_REQUIRES_DIFFERENT_PLATFORM"_s;
    case HRESULT(0x8011044A) : return u"COMADMIN_E_CAN_NOT_EXPORT_APP_PROXY"_s;
    case HRESULT(0x8011044B) : return u"COMADMIN_E_CAN_NOT_START_APP"_s;
    case HRESULT(0x8011044C) : return u"COMADMIN_E_CAN_NOT_EXPORT_SYS_APP"_s;
    case HRESULT(0x8011044D) : return u"COMADMIN_E_CANT_SUBSCRIBE_TO_COMPONENT"_s;
    case HRESULT(0x8011044E) : return u"COMADMIN_E_EVENTCLASS_CANT_BE_SUBSCRIBER"_s;
    case HRESULT(0x8011044F) : return u"COMADMIN_E_LIB_APP_PROXY_INCOMPATIBLE"_s;
    case HRESULT(0x80110450) : return u"COMADMIN_E_BASE_PARTITION_ONLY"_s;
    case HRESULT(0x80110451) : return u"COMADMIN_E_START_APP_DISABLED"_s;
    case HRESULT(0x80110457) : return u"COMADMIN_E_CAT_DUPLICATE_PARTITION_NAME"_s;
    case HRESULT(0x80110458) : return u"COMADMIN_E_CAT_INVALID_PARTITION_NAME"_s;
    case HRESULT(0x80110459) : return u"COMADMIN_E_CAT_PARTITION_IN_USE"_s;
    case HRESULT(0x8011045A) : return u"COMADMIN_E_FILE_PARTITION_DUPLICATE_FILES"_s;
    case HRESULT(0x8011045B) : return u"COMADMIN_E_CAT_IMPORTED_COMPONENTS_NOT_ALLOWED"_s;
    case HRESULT(0x8011045C) : return u"COMADMIN_E_AMBIGUOUS_APPLICATION_NAME"_s;
    case HRESULT(0x8011045D) : return u"COMADMIN_E_AMBIGUOUS_PARTITION_NAME"_s;
    case HRESULT(0x80110472) : return u"COMADMIN_E_REGDB_NOTINITIALIZED"_s;
    case HRESULT(0x80110473) : return u"COMADMIN_E_REGDB_NOTOPEN"_s;
    case HRESULT(0x80110474) : return u"COMADMIN_E_REGDB_SYSTEMERR"_s;
    case HRESULT(0x80110475) : return u"COMADMIN_E_REGDB_ALREADYRUNNING"_s;
    case HRESULT(0x80110480) : return u"COMADMIN_E_MIG_VERSIONNOTSUPPORTED"_s;
    case HRESULT(0x80110481) : return u"COMADMIN_E_MIG_SCHEMANOTFOUND"_s;
    case HRESULT(0x80110482) : return u"COMADMIN_E_CAT_BITNESSMISMATCH"_s;
    case HRESULT(0x80110483) : return u"COMADMIN_E_CAT_UNACCEPTABLEBITNESS"_s;
    case HRESULT(0x80110484) : return u"COMADMIN_E_CAT_WRONGAPPBITNESS"_s;
    case HRESULT(0x80110485) : return u"COMADMIN_E_CAT_PAUSE_RESUME_NOT_SUPPORTED"_s;
    case HRESULT(0x80110486) : return u"COMADMIN_E_CAT_SERVERFAULT"_s;
    case HRESULT(0x80110600) : return u"COMQC_E_APPLICATION_NOT_QUEUED"_s;
    case HRESULT(0x80110601) : return u"COMQC_E_NO_QUEUEABLE_INTERFACES"_s;
    case HRESULT(0x80110602) : return u"COMQC_E_QUEUING_SERVICE_NOT_AVAILABLE"_s;
    case HRESULT(0x80110603) : return u"COMQC_E_NO_IPERSISTSTREAM"_s;
    case HRESULT(0x80110604) : return u"COMQC_E_BAD_MESSAGE"_s;
    case HRESULT(0x80110605) : return u"COMQC_E_UNAUTHENTICATED"_s;
    case HRESULT(0x80110606) : return u"COMQC_E_UNTRUSTED_ENQUEUER"_s;
    case HRESULT(0x80110701) : return u"MSDTC_E_DUPLICATE_RESOURCE"_s;
    case HRESULT(0x80110808) : return u"COMADMIN_E_OBJECT_PARENT_MISSING"_s;
    case HRESULT(0x80110809) : return u"COMADMIN_E_OBJECT_DOES_NOT_EXIST"_s;
    case HRESULT(0x8011080A) : return u"COMADMIN_E_APP_NOT_RUNNING"_s;
    case HRESULT(0x8011080B) : return u"COMADMIN_E_INVALID_PARTITION"_s;
    case HRESULT(0x8011080D) : return u"COMADMIN_E_SVCAPP_NOT_POOLABLE_OR_RECYCLABLE"_s;
    case HRESULT(0x8011080E) : return u"COMADMIN_E_USER_IN_SET"_s;
    case HRESULT(0x8011080F) : return u"COMADMIN_E_CANTRECYCLELIBRARYAPPS"_s;
    case HRESULT(0x80110811) : return u"COMADMIN_E_CANTRECYCLESERVICEAPPS"_s;
    case HRESULT(0x80110812) : return u"COMADMIN_E_PROCESSALREADYRECYCLED"_s;
    case HRESULT(0x80110813) : return u"COMADMIN_E_PAUSEDPROCESSMAYNOTBERECYCLED"_s;
    case HRESULT(0x80110814) : return u"COMADMIN_E_CANTMAKEINPROCSERVICE"_s;
    case HRESULT(0x80110815) : return u"COMADMIN_E_PROGIDINUSEBYCLSID"_s;
    case HRESULT(0x80110816) : return u"COMADMIN_E_DEFAULT_PARTITION_NOT_IN_SET"_s;
    case HRESULT(0x80110817) : return u"COMADMIN_E_RECYCLEDPROCESSMAYNOTBEPAUSED"_s;
    case HRESULT(0x80110818) : return u"COMADMIN_E_PARTITION_ACCESSDENIED"_s;
    case HRESULT(0x80110819) : return u"COMADMIN_E_PARTITION_MSI_ONLY"_s;
    case HRESULT(0x8011081A) : return u"COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_1_0_FORMAT"_s;
    case HRESULT(0x8011081B) : return u"COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_NONBASE_PARTITIONS"_s;
    case HRESULT(0x8011081C) : return u"COMADMIN_E_COMP_MOVE_SOURCE"_s;
    case HRESULT(0x8011081D) : return u"COMADMIN_E_COMP_MOVE_DEST"_s;
    case HRESULT(0x8011081E) : return u"COMADMIN_E_COMP_MOVE_PRIVATE"_s;
    case HRESULT(0x8011081F) : return u"COMADMIN_E_BASEPARTITION_REQUIRED_IN_SET"_s;
    case HRESULT(0x80110820) : return u"COMADMIN_E_CANNOT_ALIAS_EVENTCLASS"_s;
    case HRESULT(0x80110821) : return u"COMADMIN_E_PRIVATE_ACCESSDENIED"_s;
    case HRESULT(0x80110822) : return u"COMADMIN_E_SAFERINVALID"_s;
    case HRESULT(0x80110823) : return u"COMADMIN_E_REGISTRY_ACCESSDENIED"_s;
    case HRESULT(0x80110824) : return u"COMADMIN_E_PARTITIONS_DISABLED"_s;
    case HRESULT(0x80042301) : return u"VSS_E_BAD_STATE"_s;
    case HRESULT(0x800423F7) : return u"VSS_E_LEGACY_PROVIDER"_s;
    case HRESULT(0x800423FF) : return u"VSS_E_RESYNC_IN_PROGRESS"_s;
    case HRESULT(0x8004232B) : return u"VSS_E_SNAPSHOT_NOT_IN_SET"_s;
    case HRESULT(0x80042312) : return u"VSS_E_MAXIMUM_NUMBER_OF_VOLUMES_REACHED"_s;
    case HRESULT(0x80042317) : return u"VSS_E_MAXIMUM_NUMBER_OF_SNAPSHOTS_REACHED"_s;
    case HRESULT(0x8004232C) : return u"VSS_E_NESTED_VOLUME_LIMIT"_s;
    case HRESULT(0x80042308) : return u"VSS_E_OBJECT_NOT_FOUND"_s;
    case HRESULT(0x80042304) : return u"VSS_E_PROVIDER_NOT_REGISTERED"_s;
    case HRESULT(0x80042306) : return u"VSS_E_PROVIDER_VETO"_s;
    case HRESULT(0x8004230C) : return u"VSS_E_VOLUME_NOT_SUPPORTED"_s;
    case HRESULT(0x8004230E) : return u"VSS_E_VOLUME_NOT_SUPPORTED_BY_PROVIDER"_s;
    case HRESULT(0x80042302) : return u"VSS_E_UNEXPECTED"_s;
    case HRESULT(0x8004230F) : return u"VSS_E_UNEXPECTED_PROVIDER_ERROR"_s;
    case HRESULT(0x8004232A) : return u"VSS_E_UNSELECTED_VOLUME"_s;
    case HRESULT(0x800423FE) : return u"VSS_E_CANNOT_REVERT_DISKID"_s;
    case HRESULT(0x80042311) : return u"VSS_E_INVALID_XML_DOCUMENT"_s;
    case HRESULT(0x8004230D) : return u"VSS_E_OBJECT_ALREADY_EXISTS"_s;
    case HRESULT(0x80284001) : return u"TBS_E_INTERNAL_ERROR"_s;
    case HRESULT(0x80284002) : return u"TBS_E_BAD_PARAMETER"_s;
    case HRESULT(0x80284003) : return u"TBS_E_INVALID_OUTPUT_POINTER"_s;
    case HRESULT(0x80284005) : return u"TBS_E_INSUFFICIENT_BUFFER"_s;
    case HRESULT(0x80284006) : return u"TBS_E_IOERROR"_s;
    case HRESULT(0x80284007) : return u"TBS_E_INVALID_CONTEXT_PARAM"_s;
    case HRESULT(0x80284008) : return u"TBS_E_SERVICE_NOT_RUNNING"_s;
    case HRESULT(0x80284009) : return u"TBS_E_TOO_MANY_TBS_CONTEXTS"_s;
    case HRESULT(0x8028400B) : return u"TBS_E_SERVICE_START_PENDING"_s;
    case HRESULT(0x8028400E) : return u"TBS_E_BUFFER_TOO_LARGE"_s;
    case HRESULT(0x8028400F) : return u"TBS_E_TPM_NOT_FOUND"_s;
    case HRESULT(0x80284010) : return u"TBS_E_SERVICE_DISABLED"_s;
    case HRESULT(0x80284016) : return u"TBS_E_DEACTIVATED"_s;
    case HRESULT(0x80320001) : return u"FWP_E_CALLOUT_NOT_FOUND"_s;
    case HRESULT(0x80320002) : return u"FWP_E_CONDITION_NOT_FOUND"_s;
    case HRESULT(0x80320003) : return u"FWP_E_FILTER_NOT_FOUND"_s;
    case HRESULT(0x80320004) : return u"FWP_E_LAYER_NOT_FOUND"_s;
    case HRESULT(0x80320005) : return u"FWP_E_PROVIDER_NOT_FOUND"_s;
    case HRESULT(0x80320006) : return u"FWP_E_PROVIDER_CONTEXT_NOT_FOUND"_s;
    case HRESULT(0x80320007) : return u"FWP_E_SUBLAYER_NOT_FOUND"_s;
    case HRESULT(0x80320008) : return u"FWP_E_NOT_FOUND"_s;
    case HRESULT(0x80320009) : return u"FWP_E_ALREADY_EXISTS"_s;
    case HRESULT(0x8032000A) : return u"FWP_E_IN_USE"_s;
    case HRESULT(0x8032000B) : return u"FWP_E_DYNAMIC_SESSION_IN_PROGRESS"_s;
    case HRESULT(0x8032000C) : return u"FWP_E_WRONG_SESSION"_s;
    case HRESULT(0x8032000D) : return u"FWP_E_NO_TXN_IN_PROGRESS"_s;
    case HRESULT(0x8032000E) : return u"FWP_E_TXN_IN_PROGRESS"_s;
    case HRESULT(0x8032000F) : return u"FWP_E_TXN_ABORTED"_s;
    case HRESULT(0x80320010) : return u"FWP_E_SESSION_ABORTED"_s;
    case HRESULT(0x80320011) : return u"FWP_E_INCOMPATIBLE_TXN"_s;
    case HRESULT(0x80320012) : return u"FWP_E_TIMEOUT"_s;
    case HRESULT(0x80320013) : return u"FWP_E_NET_EVENTS_DISABLED"_s;
    case HRESULT(0x80320014) : return u"FWP_E_INCOMPATIBLE_LAYER"_s;
    case HRESULT(0x80320015) : return u"FWP_E_KM_CLIENTS_ONLY"_s;
    case HRESULT(0x80320016) : return u"FWP_E_LIFETIME_MISMATCH"_s;
    case HRESULT(0x80320017) : return u"FWP_E_BUILTIN_OBJECT"_s;
    case HRESULT(0x80320018) : return u"FWP_E_TOO_MANY_CALLOUTS"_s;
    case HRESULT(0x80320019) : return u"FWP_E_NOTIFICATION_DROPPED"_s;
    case HRESULT(0x8032001A) : return u"FWP_E_TRAFFIC_MISMATCH"_s;
    case HRESULT(0x8032001B) : return u"FWP_E_INCOMPATIBLE_SA_STATE"_s;
    case HRESULT(0x8032001C) : return u"FWP_E_NULL_POINTER"_s;
    case HRESULT(0x8032001D) : return u"FWP_E_INVALID_ENUMERATOR"_s;
    case HRESULT(0x8032001E) : return u"FWP_E_INVALID_FLAGS"_s;
    case HRESULT(0x8032001F) : return u"FWP_E_INVALID_NET_MASK"_s;
    case HRESULT(0x80320020) : return u"FWP_E_INVALID_RANGE"_s;
    case HRESULT(0x80320021) : return u"FWP_E_INVALID_INTERVAL"_s;
    case HRESULT(0x80320022) : return u"FWP_E_ZERO_LENGTH_ARRAY"_s;
    case HRESULT(0x80320023) : return u"FWP_E_NULL_DISPLAY_NAME"_s;
    case HRESULT(0x80320024) : return u"FWP_E_INVALID_ACTION_TYPE"_s;
    case HRESULT(0x80320025) : return u"FWP_E_INVALID_WEIGHT"_s;
    case HRESULT(0x80320026) : return u"FWP_E_MATCH_TYPE_MISMATCH"_s;
    case HRESULT(0x80320027) : return u"FWP_E_TYPE_MISMATCH"_s;
    case HRESULT(0x80320028) : return u"FWP_E_OUT_OF_BOUNDS"_s;
    case HRESULT(0x80320029) : return u"FWP_E_RESERVED"_s;
    case HRESULT(0x8032002A) : return u"FWP_E_DUPLICATE_CONDITION"_s;
    case HRESULT(0x8032002B) : return u"FWP_E_DUPLICATE_KEYMOD"_s;
    case HRESULT(0x8032002C) : return u"FWP_E_ACTION_INCOMPATIBLE_WITH_LAYER"_s;
    case HRESULT(0x8032002D) : return u"FWP_E_ACTION_INCOMPATIBLE_WITH_SUBLAYER"_s;
    case HRESULT(0x8032002E) : return u"FWP_E_CONTEXT_INCOMPATIBLE_WITH_LAYER"_s;
    case HRESULT(0x8032002F) : return u"FWP_E_CONTEXT_INCOMPATIBLE_WITH_CALLOUT"_s;
    case HRESULT(0x80320030) : return u"FWP_E_INCOMPATIBLE_AUTH_METHOD"_s;
    case HRESULT(0x80320031) : return u"FWP_E_INCOMPATIBLE_DH_GROUP"_s;
    case HRESULT(0x80320032) : return u"FWP_E_EM_NOT_SUPPORTED"_s;
    case HRESULT(0x80320033) : return u"FWP_E_NEVER_MATCH"_s;
    case HRESULT(0x80320034) : return u"FWP_E_PROVIDER_CONTEXT_MISMATCH"_s;
    case HRESULT(0x80320035) : return u"FWP_E_INVALID_PARAMETER"_s;
    case HRESULT(0x80320036) : return u"FWP_E_TOO_MANY_SUBLAYERS"_s;
    case HRESULT(0x80320037) : return u"FWP_E_CALLOUT_NOTIFICATION_FAILED"_s;
    case HRESULT(0x80320038) : return u"FWP_E_INVALID_AUTH_TRANSFORM"_s;
    case HRESULT(0x80320039) : return u"FWP_E_INVALID_CIPHER_TRANSFORM"_s;
    default : return QString();
    }
}

/*!
    \since 5.2

    Returns the DWM colorization color. After the function returns, the optional
    \a opaqueBlend will contain true if the color is an opaque blend and false
    otherwise.

    \obsolete
 */
QColor QtWin::colorizationColor(bool *opaqueBlend)
{
    QWinEventFilter::setup();

    DWORD colorization = 0;
    BOOL dummy = false;
    DwmGetColorizationColor(&colorization, &dummy);
    if (opaqueBlend)
        *opaqueBlend = dummy;
    return QColor::fromRgba(colorization);
}

/*!
    \since 5.2

    Returns the real colorization color, set by the user, using an undocumented
    registry key. The API-based function \c getColorizationColor() returns an
    alpha-blended color which often turns out a semitransparent gray rather
    than something similar to what is chosen by the user.

    \obsolete

    \sa colorizationColor()
 */
QColor QtWin::realColorizationColor()
{
    QWinEventFilter::setup();

    bool ok = false;
    const QLatin1String path("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM");
    QSettings registry(path, QSettings::NativeFormat);
    QRgb color = registry.value(QLatin1String("ColorizationColor")).toUInt(&ok);
    if (!ok)
        qDebug("Failed to read colorization color.");
    return ok ? QColor::fromRgba(color) : QColor();
}

/*!
    \fn QtWin::setWindowExcludedFromPeek(QWidget *window, bool exclude)
    \since 5.2
    \overload QtWin::setWindowExcludedFromPeek()

    \obsolete
*/

/*!
    \since 5.2

    Excludes the specified \a window from Aero Peek if \a exclude is true.

    \obsolete Use DwmSetWindowAttribute with DWMWA_EXCLUDED_FROM_PEEK instead.
 */
void QtWin::setWindowExcludedFromPeek(QWindow *window, bool exclude)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    QtDwmApiDll::setBooleanWindowAttribute(reinterpret_cast<HWND>(window->winId()), qt_DWMWA_EXCLUDED_FROM_PEEK, exclude);
}

/*!
    \fn bool QtWin::isWindowExcludedFromPeek(QWidget *window)
    \since 5.2
    \overload QtWin::isWindowExcludedFromPeek()

    \obsolete
*/

/*!
    \since 5.2

    Returns true if the specified \a window is excluded from Aero Peek.

    \obsolete Check DwmGetWindowAttribute with DWMWA_EXCLUDED_FROM_PEEK instead.
 */
bool QtWin::isWindowExcludedFromPeek(QWindow *window)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    return QtDwmApiDll::booleanWindowAttribute(reinterpret_cast<HWND>(window->winId()), qt_DWMWA_EXCLUDED_FROM_PEEK);
}

/*!
    \fn void QtWin::setWindowDisallowPeek(QWidget *window, bool disallow)
    \since 5.2
    \overload QtWin::setWindowDisallowPeek()

    \obsolete
*/

/*!
    \since 5.2

    Disables Aero Peek for the specified \a window when hovering over the
    taskbar thumbnail of the window with the mouse pointer if \a disallow is
    true; otherwise allows it.

    The default is false.

    \obsolete Use DwmSetWindowAttribute with DWMWA_DISALLOW_PEEK instead.
 */
void QtWin::setWindowDisallowPeek(QWindow *window, bool disallow)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    QtDwmApiDll::setBooleanWindowAttribute(reinterpret_cast<HWND>(window->winId()), qt_DWMWA_DISALLOW_PEEK, disallow);
}

/*!
    \fn bool QtWin::isWindowPeekDisallowed(QWidget *window)
    \since 5.2
    \overload QtWin::isWindowPeekDisallowed()

    \obsolete
*/

/*!
    \since 5.2

    Returns true if Aero Peek is disallowed on the thumbnail of the specified
    \a window.

    \obsolete Check DwmGetWindowAttribute with DWMWA_DISALLOW_PEEK instead.
 */
bool QtWin::isWindowPeekDisallowed(QWindow *window)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    return QtDwmApiDll::booleanWindowAttribute(reinterpret_cast<HWND>(window->winId()), qt_DWMWA_DISALLOW_PEEK);
}

/*!
    \fn void QtWin::setWindowFlip3DPolicy(QWidget *window, QtWin::WindowFlip3DPolicy policy)
    \since 5.2
    \overload QtWin::setWindowFlip3DPolicy()
*/

/*!
    \since 5.2

    Sets the Flip3D policy \a policy for the specified \a window.

    \obsolete
 */
void QtWin::setWindowFlip3DPolicy(QWindow *window, QtWin::WindowFlip3DPolicy policy)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");

    HWND handle = reinterpret_cast<HWND>(window->winId());

    // Policy should be defaulted first, bug or smth.
    DWORD value = DWMFLIP3D_DEFAULT;
    QtDwmApiDll::setWindowAttribute(handle, DWMWA_FLIP3D_POLICY, value);

    switch (policy) {
    default :
    case FlipDefault :
        value = DWMFLIP3D_DEFAULT;
        break;
    case FlipExcludeBelow :
        value = DWMFLIP3D_EXCLUDEBELOW;
        break;
    case FlipExcludeAbove :
        value = DWMFLIP3D_EXCLUDEABOVE;
        break;
    }

    if (DWMFLIP3D_DEFAULT != value)
        QtDwmApiDll::setWindowAttribute(handle, DWMWA_FLIP3D_POLICY, value);
}

/*!
    \fn QtWin::WindowFlip3DPolicy QtWin::windowFlip3DPolicy(QWidget *window)
    \since 5.2
    \overload QtWin::windowFlip3DPolicy()

    \obsolete
 */

/*!
    \since 5.2

    Returns the current Flip3D policy for the specified \a window.

    \obsolete
 */
QtWin::WindowFlip3DPolicy QtWin::windowFlip3DPolicy(QWindow *window)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");

    const auto value =
        QtDwmApiDll::windowAttribute<DWORD>(reinterpret_cast<HWND>(window->winId()),
                                            DWMWA_FLIP3D_POLICY, DWORD(DWMFLIP3D_DEFAULT));
    QtWin::WindowFlip3DPolicy policy = QtWin::FlipDefault;
    switch (value) {
    case DWMFLIP3D_EXCLUDEABOVE :
        policy = QtWin::FlipExcludeAbove;
        break;
    case DWMFLIP3D_EXCLUDEBELOW :
        policy = QtWin::FlipExcludeBelow;
        break;
    default :
        break;
    }
    return policy;
}

void qt_ExtendFrameIntoClientArea(QWindow *window, int left, int top, int right, int bottom)
{
    QWinEventFilter::setup();
    MARGINS margins = {left, right, top, bottom};
    DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(window->winId()), &margins);
}

/*! \fn void QtWin::extendFrameIntoClientArea(QWidget *window, int left, int top, int right, int bottom)
    \since 5.2
    \overload QtWin::extendFrameIntoClientArea()
    \obsolete
 */

/*!
    \since 5.2

    Extends the glass frame into the client area of the specified \a window
    using the \a left, \a top, \a right, and \a bottom margin values.

    Pass -1 as values for any of the four margins to fully extend the frame,
    creating a \e {sheet of glass} effect.

    If you want the extended frame to act like a standard window border, you
    should handle that yourself.

    \note Qt::WA_NoSystemBackground must not be set on widgets for
    extendFrameIntoClientArea() to work.

    \obsolete

    \sa resetExtendedFrame()
 */
void QtWin::extendFrameIntoClientArea(QWindow *window, int left, int top, int right, int bottom)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    qt_ExtendFrameIntoClientArea(window, left, top, right, bottom);
}

/*! \fn void QtWin::extendFrameIntoClientArea(QWidget *window, const QMargins &margins)
    \since 5.2
    \overload QtWin::extendFrameIntoClientArea()

    Convenience overload that allows passing frame sizes in a \a margins
    structure.

    \obsolete
 */

/*!
    \since 5.2
    \overload QtWin::extendFrameIntoClientArea()

    Extends the glass frame into the client area of the specified \a window
    using the specified \a margins.

    \obsolete
 */
void QtWin::extendFrameIntoClientArea(QWindow *window, const QMargins &margins)
{
    QtWin::extendFrameIntoClientArea(window, margins.left(), margins.top(), margins.right(), margins.bottom());
}

/*!
    \fn void QtWin::resetExtendedFrame(QWidget *window)
    \since 5.2
    \overload QtWin::resetExtendedFrame()

    \obsolete
 */

/*!
    \since 5.2

    Resets the glass frame and restores the \a window attributes.

    This convenience function calls extendFrameIntoClientArea() with margins set
    to 0.

    \note Qt::WA_NoSystemBackground must not be set on widgets for
    extendFrameIntoClientArea() to work.

    \obsolete

    \sa extendFrameIntoClientArea()
 */
void QtWin::resetExtendedFrame(QWindow *window)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    qt_ExtendFrameIntoClientArea(window, 0, 0, 0, 0);
}

/*!
    \fn void QtWin::enableBlurBehindWindow(QWidget *window, const QRegion &region)
    \since 5.2
    \overload QtWin::enableBlurBehindWindow()

    \obsolete
 */

/*!
    \since 5.2

    Enables the blur effect for the specified \a region of the specified
    \a window.

    \obsolete

    \sa disableBlurBehindWindow()
 */
void QtWin::enableBlurBehindWindow(QWindow *window, const QRegion &region)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");

    DWM_BLURBEHIND dwmbb = {0, 0, nullptr, 0};
    dwmbb.dwFlags = DWM_BB_ENABLE;
    dwmbb.fEnable = TRUE;
    HRGN rgn = nullptr;
    if (!region.isNull()) {
        rgn = toHRGN(region);
        if (rgn) {
            dwmbb.hRgnBlur = rgn;
            dwmbb.dwFlags |= DWM_BB_BLURREGION;
        }
    }
    DwmEnableBlurBehindWindow(reinterpret_cast<HWND>(window->winId()), &dwmbb);
    if (rgn)
        DeleteObject(rgn);
}

/*!
    \fn void QtWin::enableBlurBehindWindow(QWidget *window)
    \since 5.2
    \overload QtWin::enableBlurBehindWindow()

    \obsolete
 */

/*!
    \since 5.2

    Enables the blur effect for the specified \a window.

    \obsolete

    \sa disableBlurBehindWindow()
 */
void QtWin::enableBlurBehindWindow(QWindow *window)
{
    QtWin::enableBlurBehindWindow(window, QRegion());
}

/*!
    \fn void QtWin::disableBlurBehindWindow(QWidget *window)
    \since 5.2
    \overload QtWin::disableBlurBehindWindow()

    \obsolete
 */

/*!
    \since 5.2

    Disables the previously enabled blur effect for the specified \a window.

    \obsolete

    \sa enableBlurBehindWindow()
 */
void QtWin::disableBlurBehindWindow(QWindow *window)
{
    Q_ASSERT_X(window, Q_FUNC_INFO, "window is null");
    DWM_BLURBEHIND dwmbb = {0, 0, nullptr, 0};
    dwmbb.dwFlags = DWM_BB_ENABLE;
    DwmEnableBlurBehindWindow(reinterpret_cast<HWND>(window->winId()), &dwmbb);
}

/*!
    \since 5.2

    \obsolete

    Returns the DWM composition state.
 */
bool QtWin::isCompositionEnabled()
{
    QWinEventFilter::setup();

    BOOL enabled = FALSE;
    DwmIsCompositionEnabled(&enabled);
    return enabled;
}

/*!
    \since 5.2

    Sets whether the Windows Desktop composition is \a enabled.

    \note The underlying function was declared deprecated as of Windows 8 and
    takes no effect.

    \obsolete
 */

QT_WARNING_PUSH
QT_WARNING_DISABLE_MSVC(4995)
void QtWin::setCompositionEnabled(bool enabled)
{
    QWinEventFilter::setup();

    UINT compositionEnabled = enabled;
    DwmEnableComposition(compositionEnabled);
}
QT_WARNING_POP

/*!
    \since 5.2

    Returns whether the colorization color is an opaque blend.

    \obsolete
 */
bool QtWin::isCompositionOpaque()
{
    bool opaque;
    colorizationColor(&opaque);
    return opaque;
}

/*!
    \since 5.2

    Sets the Application User Model ID \a id.

    For more information, see
    \l{http://msdn.microsoft.com/en-us/library/windows/desktop/dd378459.aspx}
    {Application User Model IDs}.

    \obsolete Use SetCurrentProcessExplicitAppUserModelID(id.toStdWString().c_str()) instead.
 */
void QtWin::setCurrentProcessExplicitAppUserModelID(const QString &id)
{
    QScopedArrayPointer<wchar_t> wid(qt_qstringToNullTerminated(id));
    SetCurrentProcessExplicitAppUserModelID(wid.data());
}

/*!
    \internal
 */
ITaskbarList3 *qt_createITaskbarList3()
{
    ITaskbarList3 *pTbList = nullptr;
    HRESULT result = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, qIID_ITaskbarList3, reinterpret_cast<void **>(&pTbList));
    if (SUCCEEDED(result)) {
        if (FAILED(pTbList->HrInit())) {
            pTbList->Release();
            pTbList = nullptr;
        }
    }
    return pTbList;
}

/*!
    \internal
 */
ITaskbarList2 *qt_createITaskbarList2()
{
    ITaskbarList3 *pTbList = nullptr;
    HRESULT result = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, qIID_ITaskbarList2, reinterpret_cast<void **>(&pTbList));
    if (SUCCEEDED(result)) {
        if (FAILED(pTbList->HrInit())) {
            pTbList->Release();
            pTbList = nullptr;
        }
    }
    return pTbList;
}

/*!
    \fn void QtWin::markFullscreenWindow(QWidget *window, bool fullscreen)
    \since 5.2
    \overload QtWin::markFullscreenWindow()

    \obsolete Use QWidget::showFullScreen() instead.
 */

/*!
    \since 5.2

    Marks the specified \a window as running in the full-screen mode if
    \a fullscreen is true, so that the shell handles it correctly. Otherwise,
    removes the mark.

    \note You do not usually need to call this function, because the Windows
    taskbar always tries to determine whether a window is running in the
    full-screen mode.

    \obsolete Use QWidget::showFullScreen() instead.
 */
void QtWin::markFullscreenWindow(QWindow *window, bool fullscreen)
{
    ITaskbarList2 *pTbList = qt_createITaskbarList2();
    if (pTbList) {
        pTbList->MarkFullscreenWindow(reinterpret_cast<HWND>(window->winId()), fullscreen);
        pTbList->Release();
    }
}

/*!
    \fn void QtWin::taskbarActivateTab(QWidget *window)
    \since 5.2
    \overload QtWin::taskbarActivateTab()

    \obsolete
 */

/*!
    \since 5.2

    Activates an item on the taskbar without activating the \a window itself.

    \obsolete
 */
void QtWin::taskbarActivateTab(QWindow *window)
{
    ITaskbarList *pTbList = qt_createITaskbarList2();
    if (pTbList) {
        pTbList->ActivateTab(reinterpret_cast<HWND>(window->winId()));
        pTbList->Release();
    }
}

/*!
    \fn void QtWin::taskbarActivateTabAlt(QWidget *window)
    \since 5.2
    \overload QtWin::taskbarActivateTabAlt()

    \obsolete
 */

/*!
    \since 5.2

    Marks the item that represents the specified \a window on the taskbar
    as active, but does not activate it visually.

    \obsolete
 */
void QtWin::taskbarActivateTabAlt(QWindow *window)
{
    ITaskbarList *pTbList = qt_createITaskbarList2();
    if (pTbList) {
        pTbList->SetActiveAlt(reinterpret_cast<HWND>(window->winId()));
        pTbList->Release();
    }
}

/*!
    \fn void QtWin::taskbarAddTab(QWidget *window)
    \since 5.2
    \overload QtWin::taskbarAddTab()

    \obsolete
 */

/*!
    \since 5.2

    Adds an item for the specified \a window to the taskbar.

    \obsolete
 */
void QtWin::taskbarAddTab(QWindow *window)
{
    ITaskbarList *pTbList = qt_createITaskbarList2();
    if (pTbList) {
        pTbList->AddTab(reinterpret_cast<HWND>(window->winId()));
        pTbList->Release();
    }
}

/*!
    \fn void QtWin::taskbarDeleteTab(QWidget *window)
    \since 5.2
    \overload QtWin::taskbarDeleteTab()

    \obsolete
 */

/*!
    \since 5.2

    Removes the specified \a window from the taskbar.

    \obsolete
 */
void QtWin::taskbarDeleteTab(QWindow *window)
{
    ITaskbarList *pTbList = qt_createITaskbarList3();
    if (pTbList) {
        pTbList->DeleteTab(reinterpret_cast<HWND>(window->winId()));
        pTbList->Release();
    }
}

/*!
    \enum QtWin::HBitmapFormat

    \since 5.2

    This enum defines how the conversion between \c
    HBITMAP and QPixmap is performed.

    \value  HBitmapNoAlpha
    The alpha channel is ignored and always treated as
    being set to fully opaque. This is preferred if the \c HBITMAP is
    used with standard GDI calls, such as \c BitBlt().

    \value HBitmapPremultipliedAlpha
    The \c HBITMAP is treated as having an
    alpha channel and premultiplied colors. This is preferred if the
    \c HBITMAP is accessed through the \c AlphaBlend() GDI function.

    \value HBitmapAlpha
    The \c HBITMAP is treated as having a plain alpha
    channel. This is the preferred format if the \c HBITMAP is going
    to be used as an application icon or a systray icon.

    \sa fromHBITMAP(), toHBITMAP()
*/

/*!
    \enum QtWin::WindowFlip3DPolicy

    \since 5.2

    This enum type specifies the Flip3D window policy.

    \value  FlipDefault
            Let the OS decide whether to include the window in the Flip3D
            rendering.

    \value  FlipExcludeAbove
            Exclude the window from Flip3D and display it above the Flip3D
            rendering.

    \value  FlipExcludeBelow
            Exclude the window from Flip3D and display it below the Flip3D
            rendering.

    \sa setWindowFlip3DPolicy()
 */

QT_END_NAMESPACE
