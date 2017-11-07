#include <iostream>
#include <vector>
#include "pollard.hpp"
#include "caf/all.hpp"

using namespace caf;

//Basic factorization
behavior factorize(event_based_actor* me) {
  return {
    [=] (int n) {
      std::vector<int> factors;
      p_factorize(&factors, n);

      return factors;
    }
  };
}

void find_Factors(event_based_actor* me, const actor& other, int n){
  me->request(other, std::chrono::seconds(10),n).then(
    [=](const std::vector<int> fct) {
      for(auto const& f: fct) {
        aout(me) << f << std::endl;
      }
    }
  );
}
