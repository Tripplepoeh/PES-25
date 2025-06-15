#pragma once
#include <iostream>

// Singleton-klasse voor de noodknop
class EmergencyButton {
public:
    // Retourneert de enige instantie van de knop
    static EmergencyButton& getInstance() {
        static EmergencyButton instance;
        return instance;
    }

    // Geeft aan of de knop is ingedrukt
    bool isPressed() const {
        return pressed;
    }

    // Drukt de knop in (zet status op true)
    void press() {
        pressed = true;
        std::cout << "[NOODKNOP] Ingedrukt\n";
    }

    // Resetten van de knopstatus (handmatig of na afhandeling)
    void reset() {
        pressed = false;
        std::cout << "[NOODKNOP] Gereset\n";
    }

    // Singeleton implementatie
    EmergencyButton(const EmergencyButton&) = delete;
    EmergencyButton& operator=(const EmergencyButton&) = delete;

private:
    // Prive constructor voor Singleton-patroon
    EmergencyButton() : pressed(false) {}

    bool pressed; // Interne status van de knop
};
