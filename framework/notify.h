#pragma once

//major credit to deadcell csgo devs. luv u.

struct notification_t {
	notification_t(colour_t c, colour_t c1, float time, std::string buf)
		: colour(c), background_colour(c1), m_time(time), m_buf(buf) { }
	colour_t colour;
	colour_t background_colour;
	float m_time;
	std::string m_buf;
};

class c_notification : public singleton<c_notification> {
private:
	float m_text_duration = 5.f;

	std::deque<notification_t> m_notification;

public:
	void add(bool display, colour_t colour, const std::string message, ...);
	void clear();
	void draw();
};