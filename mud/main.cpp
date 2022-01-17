#include "include/fmt/format.h"
#include <string>
#include <iostream>

using namespace std;

// Create a position
// Create a map representation
// Create a player
//    with inventory
//    with equipped
//    with basic stats (strength, dexterity, intelligence)
// Create class to represent items

int main(int /*argc*/, char** /*argv*/) {
  string user_input;

  cout << "mud> ";

  while (getline(cin, user_input)) {
    if (user_input == "quit") {
      break;
    }

    fmt::print("You entered: {}\n\n", user_input);

    cout << "mud> ";
  }

  fmt::print("\n");
}