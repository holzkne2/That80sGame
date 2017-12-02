#include "pch.h"
#include "TimerUI.h"
#include "Game.h"

TimerUI::TimerUI(GameObject* gameObject) : Component(gameObject)
{
	m_totalTime = 0;
}


TimerUI::~TimerUI()
{
}

void TimerUI::Update()
{
	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();
	m_totalTime += deltaTime;

	int min = m_totalTime / 60;
	int sec = (int)m_totalTime % 60;

	std::wstring minuteString = std::to_wstring(min);
	if (minuteString.size() < 2) {
		minuteString = L"0" + minuteString;
	}

	std::wstring secondString = std::to_wstring(sec);
	if (secondString.size() < 2) {
		secondString = L"0" + secondString;
	}

	m_ui->SetText(minuteString + L":" + secondString);
}

void TimerUI::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("UI", std::to_string((int)m_ui)));
}