#pragma once
#include <vector>
#include "../scene/scene.h"

class Node2D;
class UISprite;

class Alpha :public Scene
{
private:
    Node2D* _ndAlpha;
	Node2D* _ndLogo;
	UISprite* _spriteLogo;
	float _alphaDuration;
	float _logoAlphaNum;

	int _initDelayScheduleID;
	
	
	void _initRes();
	void _initAlpha();
	void _updateLogoAlpha(float deltaTime);
public:
    Alpha(const std::string name, const std::string uuid="");
	void init();
	void update(float deltaTime) override;
	void destroy() override;
    ~Alpha();
};

