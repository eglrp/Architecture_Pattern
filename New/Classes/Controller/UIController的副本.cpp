#include "UIController.h" // class implemented
#include "deprecated/CCDeprecated.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

/////////////// PUBLIC///////////////////////

//================= 构造函数 ====================

UIController::UIController():layer(nullptr)
{
}//UIController

UIController::~UIController()
{
}//~UIController

 //==============属性存取=====================
void UIController::setLayer(GameLayer* layer)
{
	this->layer = layer;
}

//==============类的操作 =====================

void UIController::updateLifeSprite(int HP)
{
	Sprite* dSp = layer->getLifeSprite();
	Texture2D* texture = CCTextureCache::sharedTextureCache()->addImage("Image/HP" + StringUtils::format("%d", HP) + ".png");
	dSp->setTexture(texture);
}

void UIController::playYX()
{
	//开始播放背景音效，false表示不循环
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Sound/eatprops.wav", false);
	//设置音效音量0.0-1.0
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
}
