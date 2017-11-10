#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <sys/time.h>
#include <math.h>

// CAF includes
#include "caf/all.hpp"
#include "caf/io/all.hpp"

// Boost includes
CAF_PUSH_WARNINGS
#include <boost/multiprecision/cpp_int.hpp>
CAF_POP_WARNINGS

// Own includes
#include "include/is_probable_prime.hpp"
#include "include/int512_serialization.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;

using boost::multiprecision::int512_t;

using namespace caf;

double wall0;
double cpu0;

typedef struct actor_info {
  int512_t z = (int512_t) 0;
  int512_t n;
  int512_t a; // current a
  bool running;
  response_promise promiseToClient;
  std::vector<int512_t> factors;
  std::vector<actor> worker;

} actor_info_t ;

//Getter's for time calculation
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

//Calcultations
//Minimum comumn divisor
int gcd(int a, int b) {
	int remainder;
	while (b != 0) {
		remainder = a % b;
		a = b;
		b = remainder;
	}
	return a;
}
//Pollard's factorization algorithm
int512_t p_factorize( int512_t n){
  srand((unsigned)time(0));
  int512_t a = rand() % n  + 1;
  int512_t  x=a, y=a, d, p = 1;


  do {
      x = (x*x + a) % n;
      y = (y*y + a) % n;
      y = (y*y + a) % n;
      d = (abs(y - x)) % n;
      p = gcd(d, n);

  } while(p == 1);

  return p;
}

//CAF
behavior factorize(event_based_actor* me) {
  return {
    [=] (int512_t n) {
      return p_factorize(n);
    }
  };
}
void printFacto

behavior coordinator (stateful_actor<actor_info_t> *me){
  return {
    [=](int512_t n) {
      me->info.promiseToClient = me->make_response_promise<std::vector<int512_t>>();
      me->info.factors = {};
      me->info.n = n;
      me->info.z = n;
      me->info.a = 2;

      //Need more workers
      if (/* condition */) {
        /* code */
      } else { //Process finished
        me->info.running = false;
        cout << "[COORDINATOR] : No prime factors found for " << n << endl;
        self->info.promiseToClient.deliver(self->info.factors)
      }
    },
    //How to react
    [=](int512_t result, actor a){
      //Is actor running
      if (me->info.running) {
        /* code */
      } else {
        /* code */
      }
    }
  }
}

namespace {

  struct config : actor_system_config {
    string host = "localhost";
    uint16_t port = 0;
    size_t num_workers = 0;
    string mode;
    config() {
      add_message_type<int512_t>("int512_t");
      add_message_type<vector<int512_t>>("vector<int512_t>");
      opt_group{custom_options_, "global"}
      .add(host, "host,H", "server host (ignored in server mode)")
      .add(port, "port,p", "port")
      .add(num_workers, "num-workers,w", "number of workers (in manager mode)")
      .add(mode, "mode,m", "one of 'server', 'manager' or 'client'");
    }
  };

  void run_server(actor_system& sys, const config& cfg) {

  }

  void run_manager(actor_system& sys, const config& cfg) {

  }

  void run_client(actor_system& sys, const config& cfg) {

  }

  // dispatches to run_* function depending on selected mode
  void caf_main(actor_system &sys, const config &cfg) {
    using map_t = unordered_map<string, void (*)(actor_system &, const config &)>;
    map_t modes{
      {"server", run_server},
      {"manager", run_manager},
      {"client", run_client}
    };
    auto i = modes.find(cfg.mode);
    if (i != modes.end())
      (i->second)(sys, cfg);
    else
      cerr << "*** invalid mode specified" << endl;
    }
}

CAF_MAIN(io::middleman)
