#include "annealer.h"

Annealer::Annealer() {
    temperature = INITIAL_TEMPERATURE;
    acceptedChanges = 0;
    attemptedChanges = 0;
}