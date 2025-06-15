#include "deurknop.h"
#include "main.h"  // Voor HAL_GPIO_ReadPin en andere STM32 functies

// Constructor
deurknop::deurknop() : lastPollTime(0), rawStatus(0), stableStatus(0), debounceDelay(50) {
    knopstatus = GPIO_PIN_RESET;  // Initieel als 'uit' (0)
}

// Destructor
deurknop::~deurknop() {}

// Poll de status van de deurknop (debounced)
void deurknop::poll() {
    uint32_t now = HAL_GetTick();    // Verkrijg de huidige tijd (in milliseconden)
    uint8_t currentRead = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);  // Lees de knopstatus (0 = gesloten, 1 = open)

    // Controleer of de status van de knop is veranderd
    if (currentRead != rawStatus) {
        rawStatus = currentRead;      // Werk de rauwe status bij
        lastPollTime = now;           // Reset de tijd van de laatste wijziging
    }

    // Als de debounce-vertraging verstreken is, werk dan de stabiele status bij
    if ((now - lastPollTime) > debounceDelay) {
        stableStatus = rawStatus;    // Update de stabiele status
    }

    // Zet de knopstatus naar de stabiele waarde (voor verdere verwerking)
    knopstatus = (stableStatus == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

// Retourneer de stabiele knopstatus (open = 1, gesloten = 0)
uint8_t deurknop::isOpen() const {
    return (knopstatus == GPIO_PIN_SET) ? 0 : 1;  // Return 1 if open, 0 if closed
}
