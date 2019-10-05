#include "Cocos2dx1pxScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Cocos2dx1pxScene::createScene()
{
    return Cocos2dx1pxScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Cocos2dx1pxScene.cpp\n");
}

void clear_buf(uint8_t * buf, const size_t &buf_size, const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a)
{
	for (size_t i = 0; i < buf_size; i += 4) {
		buf[i + 0] = r;
		buf[i + 1] = g;
		buf[i + 2] = b;
		buf[i + 3] = a;
	}
}

void set_pixel(uint8_t * buf, const Size &size, const int &x, const int &y, const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a)
{
	uint8_t *p = buf + (x * 4) + (y * 4 * (int)size.width);
	*(p + 0) = r;
	*(p + 1) = g;
	*(p + 2) = b;
	*(p + 3) = a;
}

bool Cocos2dx1pxScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Size size = Size(200, 200);
	size_t buf_size = size.width * size.height * 4;
	uint8_t * buf = (uint8_t *)malloc(buf_size);

	// 
	// memo : Sprite <-- SpriteFrame(s) <-- Texture
	//

	// create SpriteFrame(s)
	Vector<SpriteFrame*> sprite_frames;
	for (int i = 0; i < 4; ++i) {
		clear_buf(buf, buf_size, 0, 0, 0, 255);

		switch (i) {
		case 0:
			set_pixel(buf, size, 100, 100, 255, 0, 0, 255);
			break;
		case 1:
			set_pixel(buf, size, 100, 100, 0, 255, 0, 255);
			break;
		case 2:
			set_pixel(buf, size, 100, 100, 0, 0, 255, 255);
			break;
		default:
			break;
		}

		Texture2D* texture = new Texture2D();
		if (!texture->initWithData(buf, buf_size, Texture2D::PixelFormat::RGBA8888, size.width, size.height, size)) {
			CCLOG("Could not create texture");
			delete texture;
			texture = NULL;
		}
		texture->setAliasTexParameters();

		SpriteFrame* sprite_frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, size.width, size.height));
		texture->autorelease();

		sprite_frames.pushBack(sprite_frame);
	}

	free(buf);

	// Create Sprite from SpriteFrame(s)
	auto sprite = Sprite::createWithSpriteFrame(sprite_frames.front());
	sprite->setScale(1.0);
	this->addChild(sprite);
	sprite->setPosition(100, 100);

	// set Animation
	auto animation = Animation::createWithSpriteFrames(sprite_frames, 1.0f / 4);
	sprite->runAction(RepeatForever::create(Animate::create(animation)));

	// press escape key to exit
	auto listeiner = EventListenerKeyboard::create();
	listeiner->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			log("Escape");
			auto director = Director::getInstance();
			director->end();
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listeiner, this);

	return true;
}

void Cocos2dx1pxScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
