#include "include/action.hpp"
#include "caf/all.hpp"

using namespace caf;

int main(int argc, char const *argv[]) {

  // setup CAF environment
  actor_system_config cfg;
  actor_system system{cfg};

  // create an actor that calls factorize
  auto factorize_actor = system.spawn(factorize);

  // create another actor that calls findFactors
  system.spawn(find_Factors, factorize_actor, 15);

  return 0;
}
