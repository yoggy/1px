#ifndef __COCOS2DX1PX_SCENE_H__
#define __COCOS2DX1PX_SCENE_H__

#include "cocos2d.h"

class Cocos2dx1pxScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Cocos2dx1pxScene);
};

#endif
