// buzzer.cpp
#include "buzzer.h"

#define KLOK_FREQ 72000000

Buzzer::Buzzer(TIM_HandleTypeDef* timer, uint32_t channel, uint32_t freqHz)
    : _timer(timer), _channel(channel), _actief(false), knopStatus(false), noodknopStatus(false) {
    _arr = (KLOK_FREQ / freqHz) - 1;
}

void Buzzer::zetAan() {
	if(knopStatus || noodknopStatus){
    if (!_actief) {
        __HAL_TIM_SET_AUTORELOAD(_timer, _arr);
        __HAL_TIM_SET_COMPARE(_timer, _channel, _arr / 2);  // 50% duty cycle
        HAL_TIM_PWM_Start(_timer, _channel);
        _actief = true;
    }
  }
}

void Buzzer::zetUit() {
	if(!noodknopStatus){
		if(!knopStatus){
			if (_actief) {
				HAL_TIM_PWM_Stop(_timer, _channel);
				_actief = false;
			}
		}
	}
}

void Buzzer::knopAan(){
	knopStatus = true;
	zetAan();
}

void Buzzer::noodknopAan(){
	noodknopStatus = true;
	zetAan();
}

void Buzzer::knopUit(){
	knopStatus = false;
	zetUit();
}
void Buzzer::noodknopUit(){
	noodknopStatus = false;
	zetUit();
}
bool Buzzer::isActief() const {
    return _actief;
}
