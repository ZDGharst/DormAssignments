#include "annealer.h"

Annealer::Annealer() {
    temperature = 100;
    acceptedChanges = 0;
    attemptedChanges = 0;
}