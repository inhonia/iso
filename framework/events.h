#pragma once
#include "main.h"

struct event_info
{
	event_info(std::string text, colour_t colour, float time, float alpha)
	{
		this->text = text;
		this->colour = colour;
		this->time = time;
		this->alpha = alpha;
	}

	std::string text;
	colour_t colour;
	float time;
	float alpha;
};


class events : public singleton<events>, public igame_event_listener {
public:
	events()
	{
	}
	static std::deque<event_info>event_deque_;

	void fire_game_event(igame_event* event) override;

	void initialize();
private:
	
protected:

};
