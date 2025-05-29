// buzzerknop.cpp
#include <buzzerknop.h>
//static bool pressed = false;
Buzzerknop::Buzzerknop() : lastPollTime(0), rawStatus(1), stableStatus(1), previousStableStatus(1), debounceDelay(50), knopStatus(false) {}

Buzzerknop::~Buzzerknop() {}

void Buzzerknop::poll() {
    uint32_t now = HAL_GetTick();
    uint8_t currentRead = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);

    if (currentRead != rawStatus) {
        rawStatus = currentRead;
        lastPollTime = now;
    }

    if ((now - lastPollTime) > debounceDelay) {
        previousStableStatus = stableStatus;
        stableStatus = rawStatus;
    }
}

void Buzzerknop::gedrukt(){
	if(!knopStatus){
		knopStatus = true;
	}else{
		knopStatus = false;
	}

}



void Buzzerknop::onGedrukt(){
	previousStableStatus = GPIO_PIN_RESET;
	stableStatus = GPIO_PIN_SET;
}

bool Buzzerknop::isIngedrukt() {
    return knopStatus;
}
