#include "papaya.hpp"

using namespace basic;
using namespace basic::extension;

int main()
{
  std::cout << SCOPE( // SCOPE(var, body): define a variable and use it in body
    {"prod", 1},      // variable def and init
    {
      WHILESCOPE(             // WHILESCOPE(var, cond, body): a while loop
        {"num", calldata(0)}, // variable def and init
        var("num"),           // loop condition: `var("num")` is not zero
        {
          Set("prod", var("prod") * var("num")),
          StrSUB(1), // `StrSUB(1)` is the stack top (right) operation equivalent to `Set("num", var("num") - 1)`
        }
      ),
      MemorySet(0, var("prod")),
      Ret(0, 0x20),
    }
  );
}
