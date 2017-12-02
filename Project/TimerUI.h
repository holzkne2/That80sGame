#pragma once
#include "Component.h"
#include "UITextRenderer.h"

class TimerUI :
	public Component
{
public:
	TimerUI(GameObject*);
	virtual ~TimerUI();

	void SetUI(UITextRenderer* ui) { m_ui = ui; }

	void Update() override;

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	UITextRenderer* m_ui;
	float m_totalTime;
};

