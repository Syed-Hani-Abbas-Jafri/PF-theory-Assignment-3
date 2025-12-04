#include <stdio.h>

typedef struct { 
    int fuel; 
    int regen; 
} Event;

int simulateRecursively(Event events[], int consumption, int index, int totalStops, int solarBonus) {
 
    if (index >= totalStops) {
        return events[index - 1].fuel; 
    }

    if (events[index].fuel <= 0) {
        return 0;
    }

    int currentFuel = events[index].fuel;

    currentFuel -= consumption;

    currentFuel += events[index].regen;

    if ((index + 1) % 4 == 0) {
        currentFuel += solarBonus;
        printf("[Solar Bonus] ");
    }

    if (currentFuel < 0) currentFuel = 0;

    events[index].fuel = currentFuel;
    printf("Planet %d: Fuel Remaining = %d\n", index + 1, currentFuel);

    if (index + 1 < totalStops) {
        events[index + 1].fuel = currentFuel;
        events[index + 1].regen = events[index].regen; 
    }

    return simulateRecursively(events, consumption, index + 1, totalStops, solarBonus);
}

int main() {
    
    int totalStops = 10;
    int startFuel = 500;
    int consumption = 70;
    int gravityRegen = 20;
    int solarBonus = 100;

    Event events[10];

    events[0].fuel = startFuel;
    events[0].regen = gravityRegen;

    printf("--- Spacecraft Simulation (Array Version) ---\n");

    int finalFuel = simulateRecursively(events, consumption, 0, totalStops, solarBonus);

    printf("-------------------------------------------\n");
    if (finalFuel > 0) {
        printf("Status: SUCCESS. Final fuel = %d\n", finalFuel);
    } else {
        printf("Status: FAILED. Fuel exhausted.\n");
    }

    return 0;
}
