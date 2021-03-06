#include "PhysicsComponent.h" // class implemented
#include "cocos2d.h"
#include "Box2D/Box2D.h"

/////////////// PUBLIC///////////////////////

//================= 构造函数 ====================

PhysicsComponent::PhysicsComponent():body(nullptr)
{
}// PhysicsComponent


PhysicsComponent::PhysicsComponent(b2Body* phyBody) : body(phyBody)
{
}// PhysicsComponent

PhysicsComponent::PhysicsComponent(PhysicsComponent& physicsComponent)
{
	this->body = physicsComponent.body;
}// PhysicsComponent

PhysicsComponent::~PhysicsComponent()
{
}// ~PhysicsComponent


PhysicsComponent* PhysicsComponent::Clone()
{
	return new PhysicsComponent(copyBody(this->body));
}


//==============类的操作 =====================

void PhysicsComponent::setSize(float fWidth, float fHeight)
{
	b2Fixture* fixture = body->GetFixtureList();
	b2Filter p_filter = fixture->GetFilterData();
	body->DestroyFixture(fixture);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(fWidth / pixToMeter, fHeight / pixToMeter);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1000.0f;
	fixtureDef.friction = 0;
	fixtureDef.restitution = 1;
	fixtureDef.filter = p_filter;
	body->CreateFixture(&fixtureDef);



}


//==============属性存取 =====================

const b2Vec2 PhysicsComponent::getSpeed() 
{
	b2Vec2 vec2(0, 0);
	vec2 = body->GetLinearVelocity();
	return vec2;
}

void PhysicsComponent::setSpeed(b2Vec2 fSpeed)
{
	body->SetLinearVelocity(fSpeed);
}


void PhysicsComponent::setBody(b2Body* phyBody)
{
	this->body = phyBody;
}

b2Body* PhysicsComponent::getBody()
{
	return this->body;
}


void PhysicsComponent::setPosition(b2Vec2 pos)
{
	body->SetTransform(b2Vec2(pos.x / pixToMeter, pos.y / pixToMeter), 0);
}


b2Vec2 PhysicsComponent::getPosition()
{
	return body->GetPosition();
}

void PhysicsComponent::setFilterData(b2Filter filter)
{
	body->GetFixtureList()->SetFilterData(filter);
}

float32 PhysicsComponent::getMass() const
{
	return body->GetMass();
}

b2Vec2 PhysicsComponent::getWorldCenter() const
{
	return body->GetWorldCenter();
}

//==============辅助函数==============

b2Body* createBody(int type, int shape, std::string* id, b2World* world, float* data, float density, float friction, 
	float restitution, int groupIndex, int categoryBits, int maskBits)
{
	b2BodyDef bodyDef;                                                          //创建刚体描述
	if (type == 1)                                                              //动态刚体
		bodyDef.type = b2_dynamicBody;
	else                                                                        //静态刚体
		bodyDef.type = b2_staticBody;
	bodyDef.position.Set(data[0] / pixToMeter, data[1] / pixToMeter);           //设置刚体位置
	b2Body* body = world->CreateBody(&bodyDef);                                 //创建刚体对象
	body->SetUserData(id);                                                      //记录对应的包装对象指针
	b2FixtureDef fixtureDef;                                                    //创建刚体描述
	b2PolygonShape dynamicBox;                                                  //创建矩形对象
	b2CircleShape dynamicCirecle;                                               //创建圆形对象
	if (shape == 1)													            //刚体是矩形
	{
		dynamicBox.SetAsBox(data[2] / pixToMeter, data[3] / pixToMeter);        //设置半宽半高
		fixtureDef.shape = &dynamicBox;                                         //设置形状
	}
	else                                                                        //刚体是圆形
	{
		dynamicCirecle.m_radius = data[2] / pixToMeter;                         //设置半径
		fixtureDef.shape = &dynamicCirecle;                                     //设置形状
	}
	fixtureDef.density = density;                                               //设置密度
	fixtureDef.friction = friction;                                             //设置摩擦系数
	fixtureDef.restitution = restitution;                                       //设置恢复系数
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	body->CreateFixture(&fixtureDef);                                           //将描述与刚体结合
	body->SetLinearDamping(-0.0f);                                              //无阻尼
	return body;
}

b2Body* copyBody(b2Body* phyBody)
{
	b2BodyDef bodyDef;       //创建刚体描述
	bodyDef.type = phyBody->GetType();
	b2World* world = phyBody->GetWorld();
	bodyDef.position = phyBody->GetPosition(); //设置刚体位置	
	b2Body* body = world->CreateBody(&bodyDef);//创建刚体对象
	body->SetUserData(phyBody->GetUserData());//记录对应的包装对象指针
	b2Fixture* fixture = phyBody->GetFixtureList();
	if(fixture != NULL)
	{
		b2FixtureDef fixtureDef;
		b2Fixture tmp = *fixture;
		fixtureDef.shape = tmp.GetShape();
		fixtureDef.density = tmp.GetDensity();
		fixtureDef.friction = tmp.GetFriction();
		fixtureDef.restitution = tmp.GetRestitution();
		fixtureDef.filter = tmp.GetFilterData();
		body->CreateFixture(&fixtureDef);                              //将描述与刚体结合
		body->SetLinearDamping(-0.0f);                                 //无阻尼
	}
	return body;
}

b2Body* createWallBody(int index,  std::string* id, b2World* world, float* data, float density, float friction, float restitution)
{
	//创建线性刚体
	b2BodyDef bodyDef;	//创建刚体描述
	float positionX = (data[0] + data[2]) / 2;//计算刚体中心位置的X坐标
	float positionY = (data[1] + data[3]) / 2;//计算刚体中心位置的Y坐标
	bodyDef.position.Set(positionX / pixToMeter, positionY / pixToMeter);//设置刚体位置
	b2EdgeShape shape;//创建线形物体类对象
	shape.Set(b2Vec2((data[0] - positionX) / pixToMeter, (data[1] - positionY) / pixToMeter), b2Vec2((data[2] - positionX) / pixToMeter, (data[3] - positionY) / pixToMeter));//设置位置
	b2FixtureDef fixtureDef;//创建刚体物理描述
	fixtureDef.shape = &shape;//设置形状
	fixtureDef.density = density;//设置密度
	fixtureDef.friction = friction;//设置摩擦系数
	fixtureDef.restitution = restitution;//设置恢复系数
	if (index == 1)
	{
		fixtureDef.filter.categoryBits = 0;
	}
	else
	{
		fixtureDef.filter.groupIndex = 1;
		fixtureDef.filter.categoryBits = 2;
		fixtureDef.filter.maskBits = 4;
	}
	b2Body* body = world->CreateBody(&bodyDef);//创建刚体对象
	body->SetUserData(id);//在刚体中记录对应的包装对象指针
	body->CreateFixture(&fixtureDef);//将物理描述与刚体结合
	body->SetLinearDamping(0);
	return body;
}