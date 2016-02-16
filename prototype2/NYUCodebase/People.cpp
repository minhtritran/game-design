#include "People.h"
#include "Poop.h"
#include <iostream>
People::People(float height, float width, float x, float y, float rotation)
	: Entity(height, width, x, y, rotation) {
	infected_threshold = 0.1f;
}

void People::Update(float elapsed) {
	Entity::Update(elapsed);

	if (!infected) {
		if (x < -1.53f) x = 1.53f;
		if (x > 1.53f) x = -1.53f;
		if (y < -1.2f) y = 1.2f;
		if (y > 1.2f) y = -1.2f;
	}
	
	Poop* col_poop = dynamic_cast<Poop*>(collision);
	People* col_peep = dynamic_cast<People*>(collision);
	if (!infected && (col_poop != 0 || col_peep != 0)) {
		if (col_poop != 0) {
			float amountToReduce = (infected_threshold - infected_amount) / 3.0f;
			if (amountToReduce < 0.0f) amountToReduce = 0.0f;
			col_poop->reduceSize(amountToReduce);
		}
			
		
		if (collision->width > 0) {
			if (col_poop)
				infected_amount += collision->width;
			if (col_peep && col_peep->infected_amount > infected_amount)
				infected_amount = col_peep->infected_amount;

		}
		

		float percentInfected = infected_amount / infected_threshold;
		if (percentInfected > 1.0f) percentInfected = 1.0f;

		int rVal = 200.0f - 50.0f * percentInfected;
		int gVal = 200.0f - 80.0f * percentInfected;
		int bVal = 200.0f - 100.0f * percentInfected;
		
		setColor(rVal, gVal, bVal);
	}

	if (!infected && infected_amount >= infected_threshold) {
		setColor(110.0f, 90.0f, 70.0f);

		float dist = pow(pow(x - 0.0f, 2) + pow(y - 0.0f, 2), 0.5);

		float vector_x = (x - 0.0f) / dist;
		float vector_y = (y - 0.0f) / dist;

		velocity_x = vector_x * 1.5f;
		velocity_y = vector_y * 1.5f;
		
		infected = true;
	}
}

void People::FixedUpdate() {
	Entity::FixedUpdate();
}

void People::Render() {
	Entity::Render();
}