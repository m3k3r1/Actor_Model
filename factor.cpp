#include <string>
#include <iostream>
#include "caf/all.hpp"

using namespace caf;

//Basic factorization
behavior factorize(event_based_actor* me) {
  return {
    [=] (int n) {
      std::vector<int> fct;

      for(int i = 1; i <= n; ++i){
        if(n % i == 0)
            fct.push_back(i);
      }
      return fct;
    }
  };
}

// sends
void find_Factors(event_based_actor* me, const actor& other, int n){
  me->request(other, std::chrono::seconds(10),n).then(
    [=](const std::vector<int> fct) {
      for(auto const& f: fct) {
        aout(me) << f << std::endl;
      }
    }
  );
}

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
