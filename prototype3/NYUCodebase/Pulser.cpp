#include "Pulser.h"

Pulser::Pulser(float x, float y, float radius)
	: Entity(x, y, radius) {
	color1 = std::make_tuple(1.0f, 1.0f, 1.0f);
	color2 = std::make_tuple(1.0f, 0.0f, 0.0f);
	timer = 1.0f;
}

void Pulser::Update(float elapsed) {
	Entity::Update(elapsed);

}

void Pulser::FixedUpdate(float elapsed) {
	Entity::FixedUpdate(elapsed);
	if (timer >= 1.0f && !increasing) {
		increasing = true;
		timer = 0.0f;
	}
	if (timer >= 1.0f && increasing) {
		increasing = false;
		timer = 0.0f;
	}
	timer += 0.01f;
	if (increasing) {
		//r = easeExpo(timer, std::get<0>(color1), std::get<0>(color2), 1.0f);
		g = easeExpo(timer, std::get<1>(color1), std::get<1>(color2), 1.0f);
		b = easeExpo(timer, std::get<2>(color1), std::get<2>(color2), 1.0f);
	}
	else {
		//r = easeExpo(timer, std::get<0>(color2), std::get<0>(color1), 1.0f);
		g = easeExpo(timer, std::get<1>(color2), std::get<1>(color1), 1.0f);
		b = easeExpo(timer, std::get<2>(color2), std::get<2>(color1), 1.0f);
	}
	
	/*r = ease(std::get<0>(color1), std::get<0>(color2), timer);
	g = ease(std::get<1>(color1), std::get<1>(color2), timer);
	b = ease(std::get<2>(color1), std::get<2>(color2), timer);*/
	//setColor(color2);
}

void Pulser::Render() {
	Entity::Render();
}