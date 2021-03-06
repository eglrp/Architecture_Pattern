#include "StickyBrick.h"// class implemented

/////////////// PUBLIC///////////////////////

//================= ���캯�� ====================

StickyBrick::StickyBrick():BrickObj()
{
}// StickyBrick

StickyBrick::StickyBrick(PhysicsComponent* physicsComponent, GraphicsComponent* graphicsComponent, std::string* id, int iHP):
BrickObj(physicsComponent, graphicsComponent, id, iHP)
{
}// StickyBrick


StickyBrick::~StickyBrick()
{
}// ~StickyBrick

EntityObj* StickyBrick::Clone()
{
	return new StickyBrick(physicsComponent->Clone(), graphicsComponent->Clone(), new std::string(id->c_str()), HP);
}
