#include <iostream>
#include <memory>
#include <exception>
#include <cstdlib>
#include <SDL2/SDL.h>

#include "simulation.hpp"
#include "pconfig.hpp"

static const int default_fps = 100;

void usage(const char *appname)
{
    std::cerr << "Usage: " << appname <<
        " <config>" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        usage(argv[0]);

    try {
        PConfig cfg(argv[1]);
        Simulation simulation(400, 400, default_fps);

        while (true) {
            std::unique_ptr<PConfigEntry> entry = cfg.nextEntry();

            if (entry == nullptr)
                break;

            simulation.addParticle(entry->rx, entry->ry, entry->vx, entry->vy,
                                   entry->radius, entry->mass,
                                   entry->r, entry->g, entry->b);
        }

        while (true) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    SDL_Quit();
                    return 0;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        if (simulation.paused())
                            simulation.resume();
                        else
                            simulation.pause();
                    }
                }
                    
            }

            simulation.tick();
        }
    }
    catch (PConfigError &e) {
        std::cerr << "Configuration file error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (SimulationError &e) {
        std::cerr << "Simulation error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
