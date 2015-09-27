#include "animtitle.h"
#include <QPainter>
#include <QString>
#include <QImage>

#include "mediator.h"


animTitle::animTitle(QWidget *parent) :
    QWidget(parent)
{
    std::cout << "animTitle::CONSTRUCTOR" << std::endl;
    myParent = parent;
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateBG()));
    int delay = Mediator::get_instance()->game_data.anim_tiles[Mediator::get_instance()->selectedAnimTileset].delay;
    if (delay < 10) {
        delay = 10;
    }
    _timer->start(delay);
}


void animTitle::updateBG()
{
    //std::cout << "animTitle::updateBG" << std::endl;
    //std::cout << "sprite_preview_area::updateBG - current_npc: " << Mediator::get_instance()->current_npc_n << ", current_sprite_type: " << Mediator::get_instance()->current_sprite_type << std::endl;
    _sprite_n++;
    if (_sprite_n > ANIM_FRAMES_COUNT-1) {
        _sprite_n = 0;
    }

    // check if sprite position is greater than image width
    QString filename = QString(FILEPATH.c_str()) + QString("images/tilesets/anim/") + QString(Mediator::get_instance()->game_data.anim_tiles[Mediator::get_instance()->selectedAnimTileset].filename);
    QImage image(filename);
    if (image.isNull()) {
        _timer->stop();
        _timer->start(Mediator::get_instance()->game_data.anim_tiles[Mediator::get_instance()->selectedAnimTileset].delay);
        return;
    }
    if (_sprite_n > image.width()/TILESIZE) {
        _sprite_n = 0;
    }
    _timer->stop();
    _timer->start(Mediator::get_instance()->game_data.anim_tiles[Mediator::get_instance()->selectedAnimTileset].delay);
    repaint();
}


void animTitle::paintEvent(QPaintEvent *) {
    //std::cout << "animTitle::paintEvent" << std::endl;
    QPainter painter(this);
    QRectF target, source;


    QString filename = QString(FILEPATH.c_str()) + QString("images/tilesets/anim/") + QString(Mediator::get_instance()->game_data.anim_tiles[Mediator::get_instance()->selectedAnimTileset].filename);
    QImage image(filename);
    if (image.isNull() == true || image.width() <= 0) {
        return;
    }
    image = image.scaled(image.width()*2, image.height()*2);

    this->resize(image.size());
    myParent->adjustSize();

    int x_ini = _sprite_n*TILESIZE*2;
    std::cout << "image.w: " << image.width() << ", image.h: " << image.height() << ", _sprite_n: " << _sprite_n << ", x_init: " << x_ini << std::endl;

    source = QRectF(QPoint(_sprite_n*(TILESIZE*2), 0), QSize(TILESIZE*2, TILESIZE*2));
    target = QRectF(QPoint(0, 0), QSize(TILESIZE*2, TILESIZE*2));
    painter.drawImage(target, image, source);


}
