
/*
auto test_arg_parse = []() {

};
#include <bandit/bandit.h>
using namespace bandit;
*/

#include <array>
#include "ArgParse.h"

auto test_arg_parse = [](){
  it("should fail", [&](){
      AssertThat(5, Equals(6));
  });

  it("Should pass", [&](){
    std::array<const char*, 1> argv = {"a"};
    ArgParse args(1, argv.data());
    size_t l = 1;
    AssertThat(args.count(), Equals(l));
  });
};

// void
// test_arg_parse()
// {
//   it("should fail", [&]() {
//       AssertThat(5, Equals(6));
//   });
// }
