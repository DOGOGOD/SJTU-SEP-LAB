#include <iostream>
#include <string>
#include <string_view>

#include "simulator.h"

static constexpr int MAX_STEP = 10000;

static void usage(const char *program) {
    std::cout << "Usage: " << program << " file.bin [max_steps]\n";
}

int main(const int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return 0;
    }

    const int max_steps = argc > 2 ? std::stoi(argv[2]) : MAX_STEP;

    const char *filename = argv[1];
    if (not std::string_view(filename).ends_with(".bin")) {
        usage(argv[0]);
        return 0;
    }

    Simulator sim;
    sim.load_binary(filename);

    const Registers save_registers = sim.get_registers();
    const Memory save_memory = sim.get_memory();

    sim.run(max_steps);

    std::cout << "Changes to registers:\n";
    Registers::diff(save_registers, sim.get_registers());

    std::cout << "\nChanges to memory:\n";
    Memory::diff(save_memory, sim.get_memory());

    return 0;
}
