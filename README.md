# Ludo game of Erik Lindby

To run the project enter and empty the build folder, type 'cmake ..', type 'make' and run the generated 'main' executable.

The interesting files are in the Domain-Code folder, q_player.h/.cpp and q_table.h/.cpp.

Different state and action representations are found in Domain-Code/other_representations. Note that this projects implementation was focused on the complete action representation as described in the report, but the aggressive one ended up being better. Therefore the code for the aggressive action representation using player is quite cluttered.
