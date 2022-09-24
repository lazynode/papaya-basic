#include "papaya.hpp"

using namespace basic;
using namespace basic::extension;

int main()
{
  std::cout << SECTION({
    MemorySet(0, calldata(0) + calldata(0x20)),
    Ret(0, 0x20),
  });
}
