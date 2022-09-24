#include "papaya.hpp"

using namespace basic;
using namespace basic::extension;

int main()
{
  std::cout << SCOPESECTION(
    {
      {"x", 0},
      {"y", 1},
  },
    {
      WHILESCOPE(
        {"n", calldata(0)},
        var("n"),
        {
          Set("y", var("x") + var("y")),
          Set("x", var("y") - var("x")),
          StrSUB(1),
        }
      ),
      RetVal(var("x")),
    }
  );
}
