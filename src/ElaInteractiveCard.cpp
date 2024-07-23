#include "ElaInteractiveCard.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaInteractiveCardPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, ElaCardPixType::PixMode, CardPixMode);
ElaInteractiveCard::ElaInteractiveCard(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaInteractiveCardPrivate())
{
    Q_D(ElaInteractiveCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pTitlePixelSize = 15;
    d->_pSubTitlePixelSize = 12;
    setMinimumSize(270, 80);
    d->_pCardPixmapSize = QSize(height() * 0.8, height() * 0.8);
    d->_pTitleSpacing = 3;
    d->_pCardPixmapBorderRadius = 6;
    d->_pCardPixMode = ElaCardPixType::PixMode::Ellipse;
    d->_themeMode = eTheme->getThemeMode();
    setMouseTracking(true);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaInteractiveCard::~ElaInteractiveCard()
{
}

void ElaInteractiveCard::setCardPixmapSize(int width, int height)
{
    Q_D(ElaInteractiveCard);
    d->_pCardPixmapSize = QSize(width, height);
}

void ElaInteractiveCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaInteractiveCard);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, InteractiveCardHover) : Qt::transparent);
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图片绘制
    if (!d->_pCardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(d->_pCardPixmapSize.width() / 2 + width() / 28, height() / 2), d->_pCardPixmapSize.width() / 2, d->_pCardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(QRect(width() / 28, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmap); // rect为绘制区域，image为要绘制的图片
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::Default)
        {
            painter.drawPixmap(width() / 28, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(width() / 28, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(width() / 28, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        painter.restore();
    }
    // 文字绘制
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(Qt::black);
    }
    else
    {
        painter.setPen(Qt::white);
    }

    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->_pTitlePixelSize);
    painter.setFont(font);
    painter.drawText(d->_pCardPixmapSize.width() + width() / 11, height() / 2 - d->_pTitleSpacing, d->_pTitle);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->_pSubTitlePixelSize);
    painter.setFont(font);
    painter.drawText(d->_pCardPixmapSize.width() + width() / 11, height() / 2 + d->_pTitleSpacing + d->_pSubTitlePixelSize, d->_pSubTitle);
    painter.restore();
}
