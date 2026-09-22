#include <cassert>
#include <vector>

#include <SFML/Graphics.hpp>

#include "app/HeadlessRunner.h"
#include "app/Program.h"


int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--headless") {
            return headless_main(argc, argv);
        }
    }

    Program program;
    program.run();
}
