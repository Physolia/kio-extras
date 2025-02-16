/*  This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2000 Carsten Pfeiffer <pfeiffer@kde.org>
    SPDX-FileCopyrightText: 2000 Malte Starostik <malte@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imagecreator.h"

#include <QImageReader>

#include <KMemoryInfo>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(ImageCreator, "imagethumbnail.json")

ImageCreator::ImageCreator(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args)
{
}

#define MiB(bytes) ((bytes)*1024ll * 1024ll)
#define GiB(bytes) (MiB(bytes) * 1024ll)

// When the ram check is disabled or not available, this is the expected default value of free RAM
#define DEFAULT_FREE_RAM GiB(2)

// The maximum usable RAM is the free RAM is divided by this number:
// if the calculated image size is greater than this value, the preview is skipped.
#define RAM_DIVISOR 3

// An image smaller than 64 MiB will be loaded even if the usable RAM check fails.
#define MINIMUM_GUARANTEED_SIZE MiB(64)

/**
 * @brief maximumThumbnailRam
 * Calculates the maximum RAM that can be used to generate the thumbnail.
 *
 * The value returned is a third of the available free RAM.
 */
qint64 maximumThumbnailRam()
{
    // read available RAM (physical free ram only)
    auto freeRam = DEFAULT_FREE_RAM;

    KMemoryInfo m;
    if (!m.isNull()) {
        freeRam = qint64(m.availablePhysical());
    }

    /*
     * NOTE 1: a minimal 64MiB image is always guaranteed (this small size should never cause OS thrashing).
     * NOTE 2: the freeRam is divided by 3 for the following reasons:
     *         - the image could be converted (e.g. when depth() != 32)
     *         - we don't want to use all free ram for a thumbnail :)
     */
    return std::max(MINIMUM_GUARANTEED_SIZE, freeRam / RAM_DIVISOR);
}

KIO::ThumbnailResult ImageCreator::create(const KIO::ThumbnailRequest &request)
{
    // create image preview
    QImageReader ir(request.url().toLocalFile());

    /* The idea is to read the free ram and try to avoid OS trashing when the
     * image is too big:
     * - Qt 6: we can simply limit the maximum size that image reader can handle.
     * - Qt 5: the image plugin that allows big images should help us by implementing
     *         the QImageIOHandler::Size option (TIFF, PSB and XCF already have).
     */
    auto ram = maximumThumbnailRam();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (ir.supportsOption(QImageIOHandler::Size)) {
        auto size = ir.size();
        // euristic way: we always calculate the size supposing a 16-bits RGBA image
        if (size == QSize() || (8ll * size.width() * size.height() > ram)) {
            return KIO::ThumbnailResult::fail();
        }
    }
#else
    QImageReader::setAllocationLimit(ram / 1024 / 1024);
#endif

    ir.setAutoTransform(true);
    ir.setDecideFormatFromContent(true);
    if (ir.format() == QByteArray("raw")) {
        // make preview generation of raw files ~3 times faster (requires setDecideFormatFromContent(true))
        ir.setQuality(1);
    }

    QImage img;
    ir.read(&img);
    if (!img.isNull() && img.depth() != 32) {
        img = img.convertToFormat(img.hasAlphaChannel() ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    }

    if (!img.isNull()) {
        return KIO::ThumbnailResult::pass(img);
    }

    return KIO::ThumbnailResult::fail();
}

#include "imagecreator.moc"
#include "moc_imagecreator.cpp"
