#include "ProgressBarDelegate.h"

#include <QApplication>

namespace
{
    static const int PROGRESS_BAR_VERTICAL_MARGIN = 2;
    static const int PROGRESS_BAR_HORIZONTAL_MARGIN = 2;
}

////////////////////////////////////////////////////////////////////////////////

ProgressBarDelegate::ProgressBarDelegate(QObject *parent /*= nullptr*/)
    : QStyledItemDelegate(parent)
{ }

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant data = index.model()->data(index, Qt::DisplayRole);
    if (data.type() != QVariant::Int)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    int progress = data.toInt();
    bool isMarquee = (progress == -1);

    QStyleOptionProgressBar progressBarOption;
    QRect r = option.rect;
    r.setHeight(r.height() - 2 * PROGRESS_BAR_VERTICAL_MARGIN);
    r.setWidth(r.width() - 2 * PROGRESS_BAR_HORIZONTAL_MARGIN);
    r.moveCenter(option.rect.center());
    progressBarOption.rect = r;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.minimum = 0;
    progressBarOption.maximum = isMarquee? 0 : 100;
    progressBarOption.progress = isMarquee? 0 : progress;
    if (!isMarquee)
    {
        progressBarOption.text = QString("%1 \%").arg(QString::number(progress));
        progressBarOption.textVisible = true;
    }
    else
    {
        progressBarOption.textVisible = false;
    }

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
