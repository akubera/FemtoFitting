
#include "bandit/bandit.h"
using namespace bandit;

#include "test_arg_parse.h"

go_bandit([](){

  describe("our first test", test_arg_parse);

});

int
main(int argc, char **argv)
{
  return bandit::run(argc, argv);
}
