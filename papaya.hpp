#pragma once
#pragma GCC diagnostic error "-Wsign-conversion"

#include <iomanip>
#include <iostream>
#include <jsoncpp/json/json.h>

namespace
{
namespace functiondef
{
static std::string ToHex(uint64_t val)
{
  std::stringstream stream;
  stream << std::hex << val;
  std::string ret = stream.str();
  return ret.size() % 2 ? "0" + ret : ret;
}
static std::string ToFixedSizeHex(uint64_t val)
{
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(16) << std::hex << val;
  return stream.str();
}
}
namespace classdef
{
namespace base
{
class Base : protected Json::Value
{
protected:
  Base() {}

public:
  friend std::ostream &operator<<(std::ostream &out, const Base &value)
  {
    return out << Json::writeString(Json::StreamWriterBuilder(), value);
  }
};
class Expr : public Base
{
public:
  Expr(int val) : Expr(static_cast<int64_t>(val)) {}
  Expr(unsigned val) : Expr(static_cast<uint64_t>(val)) {}
  Expr(int64_t val) : Expr("0x" + (val >= 0 ? functiondef::ToHex(static_cast<uint64_t>(val)) : "ffffffffffffffffffffffffffffffffffffffffffffffff" + functiondef::ToFixedSizeHex(static_cast<uint64_t>(val)))) {}
  Expr(uint64_t val) : Expr("0x" + functiondef::ToHex(val)) {}
  Expr(const char *val) : Expr(static_cast<std::string>(val)) {}
  Expr(const std::string val) : Expr()
  {
    (*this)["$nodename$"] = "BasicExpressionLiteral";
    if (val.size() < 2 || val.size() > 66 || val.size() % 2 > 0 || val[0] != '0' || val[1] != 'x')
    {
      std::cerr << "[literal]: invalid string data: " << val << std::endl;
      (*this)["value"] = "0x00";
      return;
    }
    for (auto c : val.substr(2, val.size() - 2))
    {
      if (c >= '0' && c <= '9')
      {
        continue;
      }
      if (c >= 'a' && c <= 'f')
      {
        continue;
      }
      if (c >= 'A' && c <= 'F')
      {
        continue;
      }
      std::cerr << "[literal]: invalid string data: " << val << std::endl;
      (*this)["value"] = "0x00";
      return;
    }
    (*this)["value"] = val;
  }
  Expr operator&&(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionAND";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator||(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionOR";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator<(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionLT";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator>(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionGT";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator+(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionADD";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator-(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionSUB";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator*(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionMUL";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  Expr operator/(const Expr b) const
  {
    Expr ret;
    Expr a = *this;
    ret["$nodename$"] = "BasicExpressionDIV";
    ret["$children$"] = Json::Value(Json::arrayValue);
    ret["$children$"].append(a);
    ret["$children$"].append(b);
    return ret;
  }
  friend Expr operator&&(int a, const Expr b)
  {
    return Expr(a) && b;
  }
  friend Expr operator&&(unsigned a, const Expr b)
  {
    return Expr(a) && b;
  }
  friend Expr operator&&(int64_t a, const Expr b)
  {
    return Expr(a) && b;
  }
  friend Expr operator&&(uint64_t a, const Expr b)
  {
    return Expr(a) && b;
  }
  friend Expr operator&&(const std::string a, const Expr b)
  {
    return Expr(a) && b;
  }
  friend Expr operator||(int a, const Expr b)
  {
    return Expr(a) || b;
  }
  friend Expr operator||(unsigned a, const Expr b)
  {
    return Expr(a) || b;
  }
  friend Expr operator||(int64_t a, const Expr b)
  {
    return Expr(a) || b;
  }
  friend Expr operator||(uint64_t a, const Expr b)
  {
    return Expr(a) || b;
  }
  friend Expr operator||(const std::string a, const Expr b)
  {
    return Expr(a) || b;
  }
  friend Expr operator<(int a, const Expr b)
  {
    return Expr(a) < b;
  }
  friend Expr operator<(unsigned a, const Expr b)
  {
    return Expr(a) < b;
  }
  friend Expr operator<(int64_t a, const Expr b)
  {
    return Expr(a) < b;
  }
  friend Expr operator<(uint64_t a, const Expr b)
  {
    return Expr(a) < b;
  }
  friend Expr operator<(const std::string a, const Expr b)
  {
    return Expr(a) < b;
  }
  friend Expr operator>(int a, const Expr b)
  {
    return Expr(a) > b;
  }
  friend Expr operator>(unsigned a, const Expr b)
  {
    return Expr(a) > b;
  }
  friend Expr operator>(int64_t a, const Expr b)
  {
    return Expr(a) > b;
  }
  friend Expr operator>(uint64_t a, const Expr b)
  {
    return Expr(a) > b;
  }
  friend Expr operator>(const std::string a, const Expr b)
  {
    return Expr(a) > b;
  }
  friend Expr operator+(int a, const Expr b)
  {
    return Expr(a) + b;
  }
  friend Expr operator+(unsigned a, const Expr b)
  {
    return Expr(a) + b;
  }
  friend Expr operator+(int64_t a, const Expr b)
  {
    return Expr(a) + b;
  }
  friend Expr operator+(uint64_t a, const Expr b)
  {
    return Expr(a) + b;
  }
  friend Expr operator+(const std::string a, const Expr b)
  {
    return Expr(a) + b;
  }
  friend Expr operator-(int a, const Expr b)
  {
    return Expr(a) - b;
  }
  friend Expr operator-(unsigned a, const Expr b)
  {
    return Expr(a) - b;
  }
  friend Expr operator-(int64_t a, const Expr b)
  {
    return Expr(a) - b;
  }
  friend Expr operator-(uint64_t a, const Expr b)
  {
    return Expr(a) - b;
  }
  friend Expr operator-(const std::string a, const Expr b)
  {
    return Expr(a) - b;
  }
  friend Expr operator*(int a, const Expr b)
  {
    return Expr(a) * b;
  }
  friend Expr operator*(unsigned a, const Expr b)
  {
    return Expr(a) * b;
  }
  friend Expr operator*(int64_t a, const Expr b)
  {
    return Expr(a) * b;
  }
  friend Expr operator*(uint64_t a, const Expr b)
  {
    return Expr(a) * b;
  }
  friend Expr operator*(const std::string a, const Expr b)
  {
    return Expr(a) * b;
  }
  friend Expr operator/(int a, const Expr b)
  {
    return Expr(a) / b;
  }
  friend Expr operator/(unsigned a, const Expr b)
  {
    return Expr(a) / b;
  }
  friend Expr operator/(int64_t a, const Expr b)
  {
    return Expr(a) / b;
  }
  friend Expr operator/(uint64_t a, const Expr b)
  {
    return Expr(a) / b;
  }
  friend Expr operator/(const std::string a, const Expr b)
  {
    return Expr(a) / b;
  }

protected:
  Expr() {}
};
class Stmt : public Base
{
protected:
  Stmt() {}
};

}

class STOP : public base::Base
{
public:
  STOP()
  {
    (*this)["$nodename$"] = "AssemblySTOP";
  }
};
class ADD : public base::Base
{
public:
  ADD()
  {
    (*this)["$nodename$"] = "AssemblyADD";
  }
};
class MUL : public base::Base
{
public:
  MUL()
  {
    (*this)["$nodename$"] = "AssemblyMUL";
  }
};
class SUB : public base::Base
{
public:
  SUB()
  {
    (*this)["$nodename$"] = "AssemblySUB";
  }
};
class DIV : public base::Base
{
public:
  DIV()
  {
    (*this)["$nodename$"] = "AssemblyDIV";
  }
};
class SDIV : public base::Base
{
public:
  SDIV()
  {
    (*this)["$nodename$"] = "AssemblySDIV";
  }
};
class MOD : public base::Base
{
public:
  MOD()
  {
    (*this)["$nodename$"] = "AssemblyMOD";
  }
};
class SMOD : public base::Base
{
public:
  SMOD()
  {
    (*this)["$nodename$"] = "AssemblySMOD";
  }
};
class ADDMOD : public base::Base
{
public:
  ADDMOD()
  {
    (*this)["$nodename$"] = "AssemblyADDMOD";
  }
};
class MULMOD : public base::Base
{
public:
  MULMOD()
  {
    (*this)["$nodename$"] = "AssemblyMULMOD";
  }
};
class EXP : public base::Base
{
public:
  EXP()
  {
    (*this)["$nodename$"] = "AssemblyEXP";
  }
};
class SIGNEXTEND : public base::Base
{
public:
  SIGNEXTEND()
  {
    (*this)["$nodename$"] = "AssemblySIGNEXTEND";
  }
};
class LT : public base::Base
{
public:
  LT()
  {
    (*this)["$nodename$"] = "AssemblyLT";
  }
};
class GT : public base::Base
{
public:
  GT()
  {
    (*this)["$nodename$"] = "AssemblyGT";
  }
};
class SLT : public base::Base
{
public:
  SLT()
  {
    (*this)["$nodename$"] = "AssemblySLT";
  }
};
class SGT : public base::Base
{
public:
  SGT()
  {
    (*this)["$nodename$"] = "AssemblySGT";
  }
};
class EQ : public base::Base
{
public:
  EQ()
  {
    (*this)["$nodename$"] = "AssemblyEQ";
  }
};
class ISZERO : public base::Base
{
public:
  ISZERO()
  {
    (*this)["$nodename$"] = "AssemblyISZERO";
  }
};
class AND : public base::Base
{
public:
  AND()
  {
    (*this)["$nodename$"] = "AssemblyAND";
  }
};
class OR : public base::Base
{
public:
  OR()
  {
    (*this)["$nodename$"] = "AssemblyOR";
  }
};
class XOR : public base::Base
{
public:
  XOR()
  {
    (*this)["$nodename$"] = "AssemblyXOR";
  }
};
class NOT : public base::Base
{
public:
  NOT()
  {
    (*this)["$nodename$"] = "AssemblyNOT";
  }
};
class BYTE : public base::Base
{
public:
  BYTE()
  {
    (*this)["$nodename$"] = "AssemblyBYTE";
  }
};
class SHL : public base::Base
{
public:
  SHL()
  {
    (*this)["$nodename$"] = "AssemblySHL";
  }
};
class SHR : public base::Base
{
public:
  SHR()
  {
    (*this)["$nodename$"] = "AssemblySHR";
  }
};
class SAR : public base::Base
{
public:
  SAR()
  {
    (*this)["$nodename$"] = "AssemblySAR";
  }
};
class SHA3 : public base::Base
{
public:
  SHA3()
  {
    (*this)["$nodename$"] = "AssemblySHA3";
  }
};
class ADDRESS : public base::Base
{
public:
  ADDRESS()
  {
    (*this)["$nodename$"] = "AssemblyADDRESS";
  }
};
class BALANCE : public base::Base
{
public:
  BALANCE()
  {
    (*this)["$nodename$"] = "AssemblyBALANCE";
  }
};
class ORIGIN : public base::Base
{
public:
  ORIGIN()
  {
    (*this)["$nodename$"] = "AssemblyORIGIN";
  }
};
class CALLER : public base::Base
{
public:
  CALLER()
  {
    (*this)["$nodename$"] = "AssemblyCALLER";
  }
};
class CALLVALUE : public base::Base
{
public:
  CALLVALUE()
  {
    (*this)["$nodename$"] = "AssemblyCALLVALUE";
  }
};
class CALLDATALOAD : public base::Base
{
public:
  CALLDATALOAD()
  {
    (*this)["$nodename$"] = "AssemblyCALLDATALOAD";
  }
};
class CALLDATASIZE : public base::Base
{
public:
  CALLDATASIZE()
  {
    (*this)["$nodename$"] = "AssemblyCALLDATASIZE";
  }
};
class CALLDATACOPY : public base::Base
{
public:
  CALLDATACOPY()
  {
    (*this)["$nodename$"] = "AssemblyCALLDATACOPY";
  }
};
class CODESIZE : public base::Base
{
public:
  CODESIZE()
  {
    (*this)["$nodename$"] = "AssemblyCODESIZE";
  }
};
class CODECOPY : public base::Base
{
public:
  CODECOPY()
  {
    (*this)["$nodename$"] = "AssemblyCODECOPY";
  }
};
class GASPRICE : public base::Base
{
public:
  GASPRICE()
  {
    (*this)["$nodename$"] = "AssemblyGASPRICE";
  }
};
class EXTCODESIZE : public base::Base
{
public:
  EXTCODESIZE()
  {
    (*this)["$nodename$"] = "AssemblyEXTCODESIZE";
  }
};
class EXTCODECOPY : public base::Base
{
public:
  EXTCODECOPY()
  {
    (*this)["$nodename$"] = "AssemblyEXTCODECOPY";
  }
};
class RETURNDATASIZE : public base::Base
{
public:
  RETURNDATASIZE()
  {
    (*this)["$nodename$"] = "AssemblyRETURNDATASIZE";
  }
};
class RETURNDATACOPY : public base::Base
{
public:
  RETURNDATACOPY()
  {
    (*this)["$nodename$"] = "AssemblyRETURNDATACOPY";
  }
};
class EXTCODEHASH : public base::Base
{
public:
  EXTCODEHASH()
  {
    (*this)["$nodename$"] = "AssemblyEXTCODEHASH";
  }
};
class BLOCKHASH : public base::Base
{
public:
  BLOCKHASH()
  {
    (*this)["$nodename$"] = "AssemblyBLOCKHASH";
  }
};
class COINBASE : public base::Base
{
public:
  COINBASE()
  {
    (*this)["$nodename$"] = "AssemblyCOINBASE";
  }
};
class TIMESTAMP : public base::Base
{
public:
  TIMESTAMP()
  {
    (*this)["$nodename$"] = "AssemblyTIMESTAMP";
  }
};
class NUMBER : public base::Base
{
public:
  NUMBER()
  {
    (*this)["$nodename$"] = "AssemblyNUMBER";
  }
};
class DIFFICULTY : public base::Base
{
public:
  DIFFICULTY()
  {
    (*this)["$nodename$"] = "AssemblyDIFFICULTY";
  }
};
class GASLIMIT : public base::Base
{
public:
  GASLIMIT()
  {
    (*this)["$nodename$"] = "AssemblyGASLIMIT";
  }
};
class CHAINID : public base::Base
{
public:
  CHAINID()
  {
    (*this)["$nodename$"] = "AssemblyCHAINID";
  }
};
class SELFBALANCE : public base::Base
{
public:
  SELFBALANCE()
  {
    (*this)["$nodename$"] = "AssemblySELFBALANCE";
  }
};
class BASEFEE : public base::Base
{
public:
  BASEFEE()
  {
    (*this)["$nodename$"] = "AssemblyBASEFEE";
  }
};
class POP : public base::Base
{
public:
  POP()
  {
    (*this)["$nodename$"] = "AssemblyPOP";
  }
};
class MLOAD : public base::Base
{
public:
  MLOAD()
  {
    (*this)["$nodename$"] = "AssemblyMLOAD";
  }
};
class MSTORE : public base::Base
{
public:
  MSTORE()
  {
    (*this)["$nodename$"] = "AssemblyMSTORE";
  }
};
class MSTORE8 : public base::Base
{
public:
  MSTORE8()
  {
    (*this)["$nodename$"] = "AssemblyMSTORE8";
  }
};
class SLOAD : public base::Base
{
public:
  SLOAD()
  {
    (*this)["$nodename$"] = "AssemblySLOAD";
  }
};
class SSTORE : public base::Base
{
public:
  SSTORE()
  {
    (*this)["$nodename$"] = "AssemblySSTORE";
  }
};
class JUMP : public base::Base
{
public:
  JUMP()
  {
    (*this)["$nodename$"] = "AssemblyJUMP";
  }
};
class JUMPI : public base::Base
{
public:
  JUMPI()
  {
    (*this)["$nodename$"] = "AssemblyJUMPI";
  }
};
class PC : public base::Base
{
public:
  PC()
  {
    (*this)["$nodename$"] = "AssemblyPC";
  }
};
class MSIZE : public base::Base
{
public:
  MSIZE()
  {
    (*this)["$nodename$"] = "AssemblyMSIZE";
  }
};
class GAS : public base::Base
{
public:
  GAS()
  {
    (*this)["$nodename$"] = "AssemblyGAS";
  }
};
class JUMPDEST : public base::Base
{
public:
  JUMPDEST()
  {
    (*this)["$nodename$"] = "AssemblyJUMPDEST";
  }
};
class DUP1 : public base::Base
{
public:
  DUP1()
  {
    (*this)["$nodename$"] = "AssemblyDUP1";
  }
};
class DUP2 : public base::Base
{
public:
  DUP2()
  {
    (*this)["$nodename$"] = "AssemblyDUP2";
  }
};
class DUP3 : public base::Base
{
public:
  DUP3()
  {
    (*this)["$nodename$"] = "AssemblyDUP3";
  }
};
class DUP4 : public base::Base
{
public:
  DUP4()
  {
    (*this)["$nodename$"] = "AssemblyDUP4";
  }
};
class DUP5 : public base::Base
{
public:
  DUP5()
  {
    (*this)["$nodename$"] = "AssemblyDUP5";
  }
};
class DUP6 : public base::Base
{
public:
  DUP6()
  {
    (*this)["$nodename$"] = "AssemblyDUP6";
  }
};
class DUP7 : public base::Base
{
public:
  DUP7()
  {
    (*this)["$nodename$"] = "AssemblyDUP7";
  }
};
class DUP8 : public base::Base
{
public:
  DUP8()
  {
    (*this)["$nodename$"] = "AssemblyDUP8";
  }
};
class DUP9 : public base::Base
{
public:
  DUP9()
  {
    (*this)["$nodename$"] = "AssemblyDUP9";
  }
};
class DUP10 : public base::Base
{
public:
  DUP10()
  {
    (*this)["$nodename$"] = "AssemblyDUP10";
  }
};
class DUP11 : public base::Base
{
public:
  DUP11()
  {
    (*this)["$nodename$"] = "AssemblyDUP11";
  }
};
class DUP12 : public base::Base
{
public:
  DUP12()
  {
    (*this)["$nodename$"] = "AssemblyDUP12";
  }
};
class DUP13 : public base::Base
{
public:
  DUP13()
  {
    (*this)["$nodename$"] = "AssemblyDUP13";
  }
};
class DUP14 : public base::Base
{
public:
  DUP14()
  {
    (*this)["$nodename$"] = "AssemblyDUP14";
  }
};
class DUP15 : public base::Base
{
public:
  DUP15()
  {
    (*this)["$nodename$"] = "AssemblyDUP15";
  }
};
class DUP16 : public base::Base
{
public:
  DUP16()
  {
    (*this)["$nodename$"] = "AssemblyDUP16";
  }
};
class SWAP1 : public base::Base
{
public:
  SWAP1()
  {
    (*this)["$nodename$"] = "AssemblySWAP1";
  }
};
class SWAP2 : public base::Base
{
public:
  SWAP2()
  {
    (*this)["$nodename$"] = "AssemblySWAP2";
  }
};
class SWAP3 : public base::Base
{
public:
  SWAP3()
  {
    (*this)["$nodename$"] = "AssemblySWAP3";
  }
};
class SWAP4 : public base::Base
{
public:
  SWAP4()
  {
    (*this)["$nodename$"] = "AssemblySWAP4";
  }
};
class SWAP5 : public base::Base
{
public:
  SWAP5()
  {
    (*this)["$nodename$"] = "AssemblySWAP5";
  }
};
class SWAP6 : public base::Base
{
public:
  SWAP6()
  {
    (*this)["$nodename$"] = "AssemblySWAP6";
  }
};
class SWAP7 : public base::Base
{
public:
  SWAP7()
  {
    (*this)["$nodename$"] = "AssemblySWAP7";
  }
};
class SWAP8 : public base::Base
{
public:
  SWAP8()
  {
    (*this)["$nodename$"] = "AssemblySWAP8";
  }
};
class SWAP9 : public base::Base
{
public:
  SWAP9()
  {
    (*this)["$nodename$"] = "AssemblySWAP9";
  }
};
class SWAP10 : public base::Base
{
public:
  SWAP10()
  {
    (*this)["$nodename$"] = "AssemblySWAP10";
  }
};
class SWAP11 : public base::Base
{
public:
  SWAP11()
  {
    (*this)["$nodename$"] = "AssemblySWAP11";
  }
};
class SWAP12 : public base::Base
{
public:
  SWAP12()
  {
    (*this)["$nodename$"] = "AssemblySWAP12";
  }
};
class SWAP13 : public base::Base
{
public:
  SWAP13()
  {
    (*this)["$nodename$"] = "AssemblySWAP13";
  }
};
class SWAP14 : public base::Base
{
public:
  SWAP14()
  {
    (*this)["$nodename$"] = "AssemblySWAP14";
  }
};
class SWAP15 : public base::Base
{
public:
  SWAP15()
  {
    (*this)["$nodename$"] = "AssemblySWAP15";
  }
};
class SWAP16 : public base::Base
{
public:
  SWAP16()
  {
    (*this)["$nodename$"] = "AssemblySWAP16";
  }
};
class LOG0 : public base::Base
{
public:
  LOG0()
  {
    (*this)["$nodename$"] = "AssemblyLOG0";
  }
};
class LOG1 : public base::Base
{
public:
  LOG1()
  {
    (*this)["$nodename$"] = "AssemblyLOG1";
  }
};
class LOG2 : public base::Base
{
public:
  LOG2()
  {
    (*this)["$nodename$"] = "AssemblyLOG2";
  }
};
class LOG3 : public base::Base
{
public:
  LOG3()
  {
    (*this)["$nodename$"] = "AssemblyLOG3";
  }
};
class LOG4 : public base::Base
{
public:
  LOG4()
  {
    (*this)["$nodename$"] = "AssemblyLOG4";
  }
};
class CREATE : public base::Base
{
public:
  CREATE()
  {
    (*this)["$nodename$"] = "AssemblyCREATE";
  }
};
class CALL : public base::Base
{
public:
  CALL()
  {
    (*this)["$nodename$"] = "AssemblyCALL";
  }
};
class CALLCODE : public base::Base
{
public:
  CALLCODE()
  {
    (*this)["$nodename$"] = "AssemblyCALLCODE";
  }
};
class RETURN : public base::Base
{
public:
  RETURN()
  {
    (*this)["$nodename$"] = "AssemblyRETURN";
  }
};
class DELEGATECALL : public base::Base
{
public:
  DELEGATECALL()
  {
    (*this)["$nodename$"] = "AssemblyDELEGATECALL";
  }
};
class CREATE2 : public base::Base
{
public:
  CREATE2()
  {
    (*this)["$nodename$"] = "AssemblyCREATE2";
  }
};
class STATICCALL : public base::Base
{
public:
  STATICCALL()
  {
    (*this)["$nodename$"] = "AssemblySTATICCALL";
  }
};
class REVERT : public base::Base
{
public:
  REVERT()
  {
    (*this)["$nodename$"] = "AssemblyREVERT";
  }
};
class SELFDESTRUCT : public base::Base
{
public:
  SELFDESTRUCT()
  {
    (*this)["$nodename$"] = "AssemblySELFDESTRUCT";
  }
};
class PUSH : public base::Base
{
public:
  PUSH(const std::string val)
  {
    (*this)["$nodename$"] = "AssemblyPUSH";
    if (val.size() < 2 || val.size() > 66 || val.size() % 2 > 0 || val[0] != '0' || val[1] != 'x')
    {
      std::cerr << "[PUSH]: invalid string data: " << val << std::endl;
      (*this)["pushdata"] = "0x00";
      return;
    }
    for (auto c : val.substr(2))
    {
      if (c >= '0' && c <= '9')
      {
        continue;
      }
      if (c >= 'a' && c <= 'f')
      {
        continue;
      }
      if (c >= 'A' && c <= 'F')
      {
        continue;
      }
      std::cerr << "[PUSH]: invalid string data: " << val << std::endl;
      (*this)["pushdata"] = "0x00";
      return;
    }
    (*this)["pushdata"] = val;
  }
};
class PadSection : public base::Base
{
public:
  PadSection(uint32_t padsize, std::initializer_list<const base::Base> children)
  {
    (*this)["$nodename$"] = "AssemblyPadSection";
    (*this)["padsize"] = padsize;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class Section : public base::Base
{
public:
  Section(std::initializer_list<const base::Base> children)
  {
    (*this)["$nodename$"] = "AssemblySection";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class Tag : public base::Base
{
public:
  Tag(const std::string tag)
  {
    (*this)["$nodename$"] = "AssemblyPlaceTag";
    (*this)["tag"] = tag;
  }
};
class PushTag : public base::Base
{
public:
  PushTag(const std::string tag, uint8_t tagsize)
  {
    (*this)["$nodename$"] = "AssemblyPushTag";
    (*this)["tag"] = tag;
    if (tagsize == 0 || tagsize > 4)
    {
      std::cerr << "[PushTag]: invalid tagsize: " << tagsize << std::endl;
      tagsize = 2;
    }
    (*this)["tagsize"] = tagsize;
  }
};
class JumpTo : public base::Base
{
public:
  JumpTo(const std::string tag, uint8_t tagsize)
  {
    (*this)["$nodename$"] = "AssemblyJumpTo";
    (*this)["tag"] = tag;
    if (tagsize == 0 || tagsize > 4)
    {
      std::cerr << "[JumpTo]: invalid tagsize: " << tagsize << std::endl;
      tagsize = 2;
    }
    (*this)["tagsize"] = tagsize;
  }
};
class JumpIf : public base::Base
{
public:
  JumpIf(const std::string tag, uint8_t tagsize)
  {
    (*this)["$nodename$"] = "AssemblyJumpIf";
    (*this)["tag"] = tag;
    if (tagsize == 0 || tagsize > 4)
    {
      std::cerr << "[JumpIf]: invalid tagsize: " << tagsize << std::endl;
      tagsize = 2;
    }
    (*this)["tagsize"] = tagsize;
  }
};
class literal : public base::Expr
{
public:
  literal(const std::string val) : base::Expr(val) {}
};
class stack : public base::Expr
{
public:
  stack(uint8_t index)
  {
    (*this)["$nodename$"] = "BasicExpressionStack";
    if (index > 15)
    {
      std::cerr << "[stack]: invalid index: " << index << std::endl;
      index = 0;
    }
    (*this)["index"] = index;
  }
};
class placeholder : public base::Expr
{
public:
  placeholder()
  {
    (*this)["$nodename$"] = "BasicExpressionPlaceholder";
  }
};
class calldata : public base::Expr
{
public:
  calldata(const base::Expr &offset)
  {
    (*this)["$nodename$"] = "BasicExpressionCalldata";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
  }
};
class memory : public base::Expr
{
public:
  memory(const base::Expr &offset)
  {
    (*this)["$nodename$"] = "BasicExpressionMemory";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
  }
};
class storage : public base::Expr
{
public:
  storage(const base::Expr &offset)
  {
    (*this)["$nodename$"] = "BasicExpressionStorage";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
  }
};
class address : public base::Expr
{
public:
  address()
  {
    (*this)["$nodename$"] = "BasicExpressionADDRESS";
  }
};
class origin : public base::Expr
{
public:
  origin()
  {
    (*this)["$nodename$"] = "BasicExpressionORIGIN";
  }
};
class caller : public base::Expr
{
public:
  caller()
  {
    (*this)["$nodename$"] = "BasicExpressionCALLER";
  }
};
class callvalue : public base::Expr
{
public:
  callvalue()
  {
    (*this)["$nodename$"] = "BasicExpressionCALLVALUE";
  }
};
class calldatasize : public base::Expr
{
public:
  calldatasize()
  {
    (*this)["$nodename$"] = "BasicExpressionCALLDATASIZE";
  }
};
class codesize : public base::Expr
{
public:
  codesize()
  {
    (*this)["$nodename$"] = "BasicExpressionCODESIZE";
  }
};
class gasprice : public base::Expr
{
public:
  gasprice()
  {
    (*this)["$nodename$"] = "BasicExpressionGASPRICE";
  }
};
class returndatasize : public base::Expr
{
public:
  returndatasize()
  {
    (*this)["$nodename$"] = "BasicExpressionRETURNDATASIZE";
  }
};
class codebase : public base::Expr
{
public:
  codebase()
  {
    (*this)["$nodename$"] = "BasicExpressionCODEBASE";
  }
};
class timestamp : public base::Expr
{
public:
  timestamp()
  {
    (*this)["$nodename$"] = "BasicExpressionTIMESTAMP";
  }
};
class number : public base::Expr
{
public:
  number()
  {
    (*this)["$nodename$"] = "BasicExpressionNUMBER";
  }
};
class difficulty : public base::Expr
{
public:
  difficulty()
  {
    (*this)["$nodename$"] = "BasicExpressionDIFFICULTY";
  }
};
class gaslimit : public base::Expr
{
public:
  gaslimit()
  {
    (*this)["$nodename$"] = "BasicExpressionGASLIMIT";
  }
};
class chainid : public base::Expr
{
public:
  chainid()
  {
    (*this)["$nodename$"] = "BasicExpressionCHAINID";
  }
};
class selfbalance : public base::Expr
{
public:
  selfbalance()
  {
    (*this)["$nodename$"] = "BasicExpressionSELFBALANCE";
  }
};
class basefee : public base::Expr
{
public:
  basefee()
  {
    (*this)["$nodename$"] = "BasicExpressionBASEFEE";
  }
};
class pc : public base::Expr
{
public:
  pc()
  {
    (*this)["$nodename$"] = "BasicExpressionPC";
  }
};
class msize : public base::Expr
{
public:
  msize()
  {
    (*this)["$nodename$"] = "BasicExpressionMSIZE";
  }
};
class gas : public base::Expr
{
public:
  gas()
  {
    (*this)["$nodename$"] = "BasicExpressionGAS";
  }
};
class iszero : public base::Expr
{
public:
  iszero(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionISZERO";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class not_ : public base::Expr
{
public:
  not_(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionNOT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class balance : public base::Expr
{
public:
  balance(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionBALANCE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class calldataload : public base::Expr
{
public:
  calldataload(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionCALLDATALOAD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class extcodesize : public base::Expr
{
public:
  extcodesize(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionEXTCODESIZE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class extcodehash : public base::Expr
{
public:
  extcodehash(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionEXTCODEHASH";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class blockhash : public base::Expr
{
public:
  blockhash(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionBLOCKHASH";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class mload : public base::Expr
{
public:
  mload(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionMLOAD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class sload : public base::Expr
{
public:
  sload(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionSLOAD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class add : public base::Expr
{
public:
  add(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionADD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class mul : public base::Expr
{
public:
  mul(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionMUL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class sub : public base::Expr
{
public:
  sub(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSUB";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class div : public base::Expr
{
public:
  div(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class sdiv : public base::Expr
{
public:
  sdiv(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class mod : public base::Expr
{
public:
  mod(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class smod : public base::Expr
{
public:
  smod(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class exp : public base::Expr
{
public:
  exp(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionEXP";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class signextend : public base::Expr
{
public:
  signextend(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSIGNEXTEND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class lt : public base::Expr
{
public:
  lt(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class gt : public base::Expr
{
public:
  gt(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class slt : public base::Expr
{
public:
  slt(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class sgt : public base::Expr
{
public:
  sgt(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class eq : public base::Expr
{
public:
  eq(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionEQ";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class and_ : public base::Expr
{
public:
  and_(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionAND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class or_ : public base::Expr
{
public:
  or_(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class xor_ : public base::Expr
{
public:
  xor_(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionXOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class byte : public base::Expr
{
public:
  byte(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionBYTE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class shl : public base::Expr
{
public:
  shl(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSHL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class shr : public base::Expr
{
public:
  shr(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSHR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class sar : public base::Expr
{
public:
  sar(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSAR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class sha3 : public base::Expr
{
public:
  sha3(const base::Expr a, const base::Expr b)
  {
    (*this)["$nodename$"] = "BasicExpressionSHA3";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
  }
};
class addmod : public base::Expr
{
public:
  addmod(const base::Expr a, const base::Expr b, const base::Expr n)
  {
    (*this)["$nodename$"] = "BasicExpressionADDMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
    (*this)["$children$"].append(n);
  }
};
class mulmod : public base::Expr
{
public:
  mulmod(const base::Expr a, const base::Expr b, const base::Expr n)
  {
    (*this)["$nodename$"] = "BasicExpressionMULMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(a);
    (*this)["$children$"].append(b);
    (*this)["$children$"].append(n);
  }
};
class create : public base::Expr
{
public:
  create(const base::Expr value, const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicExpressionCREATE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(value);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class create2 : public base::Expr
{
public:
  create2(const base::Expr value, const base::Expr offset, const base::Expr size, const base::Expr salt)
  {
    (*this)["$nodename$"] = "BasicExpressionCREATE2";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(value);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
    (*this)["$children$"].append(salt);
  }
};
class delegatecall : public base::Expr
{
public:
  delegatecall(const base::Expr gas, const base::Expr address, const base::Expr argsoffset, const base::Expr argssize, const base::Expr retoffset, const base::Expr retsize)
  {
    (*this)["$nodename$"] = "BasicExpressionDELEGATECALL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(gas);
    (*this)["$children$"].append(address);
    (*this)["$children$"].append(argsoffset);
    (*this)["$children$"].append(argssize);
    (*this)["$children$"].append(retoffset);
    (*this)["$children$"].append(retsize);
  }
};
class staticcall : public base::Expr
{
public:
  staticcall(const base::Expr gas, const base::Expr address, const base::Expr argsoffset, const base::Expr argssize, const base::Expr retoffset, const base::Expr retsize)
  {
    (*this)["$nodename$"] = "BasicExpressionSTATICCALL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(gas);
    (*this)["$children$"].append(address);
    (*this)["$children$"].append(argsoffset);
    (*this)["$children$"].append(argssize);
    (*this)["$children$"].append(retoffset);
    (*this)["$children$"].append(retsize);
  }
};
class call : public base::Expr
{
public:
  call(const base::Expr gas, const base::Expr address, const base::Expr value, const base::Expr argsoffset, const base::Expr argssize, const base::Expr retoffset, const base::Expr retsize)
  {
    (*this)["$nodename$"] = "BasicExpressionCALL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(gas);
    (*this)["$children$"].append(address);
    (*this)["$children$"].append(value);
    (*this)["$children$"].append(argsoffset);
    (*this)["$children$"].append(argssize);
    (*this)["$children$"].append(retoffset);
    (*this)["$children$"].append(retsize);
  }
};
class callcode : public base::Expr
{
public:
  callcode(const base::Expr gas, const base::Expr address, const base::Expr value, const base::Expr argsoffset, const base::Expr argssize, const base::Expr retoffset, const base::Expr retsize)
  {
    (*this)["$nodename$"] = "BasicExpressionCALLCODE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(gas);
    (*this)["$children$"].append(address);
    (*this)["$children$"].append(value);
    (*this)["$children$"].append(argsoffset);
    (*this)["$children$"].append(argssize);
    (*this)["$children$"].append(retoffset);
    (*this)["$children$"].append(retsize);
  }
};
class var : public base::Expr
{
public:
  var(const std::string name)
  {
    (*this)["$nodename$"] = "BasicExpressionVar";
    (*this)["varname"] = name;
  }
};
class Ret : public base::Stmt
{
public:
  Ret(const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementReturn";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class Rvt : public base::Stmt
{
public:
  Rvt(const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementRevert";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class Die : public base::Stmt
{
public:
  Die(const base::Expr address)
  {
    (*this)["$nodename$"] = "BasicStatementSelfDestruct";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(address);
  }
};
class End : public base::Stmt
{
public:
  End()
  {
    (*this)["$nodename$"] = "BasicStatementStop";
  }
};
class Log : public base::Stmt
{
public:
  Log(const base::Expr offset, const base::Expr size, std::initializer_list<const base::Expr> topics)
  {
    (*this)["$nodename$"] = "BasicStatementLog";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
    for (auto topic : topics)
    {
      (*this)["$children$"].append(topic);
    }
  }
};
class Drop : public base::Stmt
{
public:
  Drop(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementDrop";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class MemcopyExtcode : public base::Stmt
{
public:
  MemcopyExtcode(const base::Expr address, const base::Expr dest, const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementExtcodeCopy";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(address);
    (*this)["$children$"].append(dest);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class MemcopyCode : public base::Stmt
{
public:
  MemcopyCode(const base::Expr dest, const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementCodeCopy";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(dest);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class MemcopyCalldata : public base::Stmt
{
public:
  MemcopyCalldata(const base::Expr dest, const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementCalldataCopy";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(dest);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class MemcopyReturndata : public base::Stmt
{
public:
  MemcopyReturndata(const base::Expr dest, const base::Expr offset, const base::Expr size)
  {
    (*this)["$nodename$"] = "BasicStatementReturndataCopy";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(dest);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(size);
  }
};
class StorageSet : public base::Stmt
{
public:
  StorageSet(const base::Expr offset, const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStorageSet";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(val);
  }
};
class MemorySet : public base::Stmt
{
public:
  MemorySet(const base::Expr offset, const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementMemorySet";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(val);
  }
};
class MemorySetByte : public base::Stmt
{
public:
  MemorySetByte(const base::Expr offset, const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementMemorySetByte";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(offset);
    (*this)["$children$"].append(val);
  }
};
class StackSet : public base::Stmt
{
public:
  StackSet(uint8_t index, const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStackSet";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
    if (index > 15)
    {
      std::cerr << "[StackSet]: invalid index: " << index << std::endl;
      index = 0;
    }
    (*this)["index"] = index;
  }
};
class StackSwap : public base::Stmt
{
public:
  StackSwap(uint8_t x, uint8_t y)
  {
    (*this)["$nodename$"] = "BasicStatementStackSwap";
    if (x > 15 || y > 15 || x == y)
    {
      std::cerr << "[StackSwap]: invalid swap pair: " << x << ", " << y << std::endl;
      x = 0;
      x = 1;
    }
    (*this)["x"] = x;
    (*this)["y"] = y;
  }
};
class StISZERO : public base::Stmt
{
public:
  StISZERO()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopISZERO";
  }
};
class StNOT : public base::Stmt
{
public:
  StNOT()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopNOT";
  }
};
class StBALANCE : public base::Stmt
{
public:
  StBALANCE()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopBALANCE";
  }
};
class StCALLDATALOAD : public base::Stmt
{
public:
  StCALLDATALOAD()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopCALLDATALOAD";
  }
};
class StEXTCODESIZE : public base::Stmt
{
public:
  StEXTCODESIZE()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopEXTCODESIZE";
  }
};
class StEXTCODEHASH : public base::Stmt
{
public:
  StEXTCODEHASH()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopEXTCODEHASH";
  }
};
class StBLOCKHASH : public base::Stmt
{
public:
  StBLOCKHASH()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopBLOCKHASH";
  }
};
class StMLOAD : public base::Stmt
{
public:
  StMLOAD()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopMLOAD";
  }
};
class StSLOAD : public base::Stmt
{
public:
  StSLOAD()
  {
    (*this)["$nodename$"] = "BasicStatementStacktopSLOAD";
  }
};
class StlADD : public base::Stmt
{
public:
  StlADD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftADD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlMUL : public base::Stmt
{
public:
  StlMUL(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftMUL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSUB : public base::Stmt
{
public:
  StlSUB(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSUB";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlDIV : public base::Stmt
{
public:
  StlDIV(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSDIV : public base::Stmt
{
public:
  StlSDIV(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlMOD : public base::Stmt
{
public:
  StlMOD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSMOD : public base::Stmt
{
public:
  StlSMOD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlEXP : public base::Stmt
{
public:
  StlEXP(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftEXP";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSIGNEXTEND : public base::Stmt
{
public:
  StlSIGNEXTEND(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSIGNEXTEND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlLT : public base::Stmt
{
public:
  StlLT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlGT : public base::Stmt
{
public:
  StlGT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSLT : public base::Stmt
{
public:
  StlSLT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSGT : public base::Stmt
{
public:
  StlSGT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlEQ : public base::Stmt
{
public:
  StlEQ(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftEQ";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlAND : public base::Stmt
{
public:
  StlAND(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftAND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlOR : public base::Stmt
{
public:
  StlOR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlXOR : public base::Stmt
{
public:
  StlXOR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftXOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlBYTE : public base::Stmt
{
public:
  StlBYTE(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftBYTE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSHL : public base::Stmt
{
public:
  StlSHL(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSHL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSHR : public base::Stmt
{
public:
  StlSHR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSHR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSAR : public base::Stmt
{
public:
  StlSAR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSAR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StlSHA3 : public base::Stmt
{
public:
  StlSHA3(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopLeftSHA3";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrADD : public base::Stmt
{
public:
  StrADD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightADD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrMUL : public base::Stmt
{
public:
  StrMUL(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightMUL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSUB : public base::Stmt
{
public:
  StrSUB(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSUB";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrDIV : public base::Stmt
{
public:
  StrDIV(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSDIV : public base::Stmt
{
public:
  StrSDIV(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSDIV";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrMOD : public base::Stmt
{
public:
  StrMOD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSMOD : public base::Stmt
{
public:
  StrSMOD(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSMOD";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrEXP : public base::Stmt
{
public:
  StrEXP(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightEXP";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSIGNEXTEND : public base::Stmt
{
public:
  StrSIGNEXTEND(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSIGNEXTEND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrLT : public base::Stmt
{
public:
  StrLT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrGT : public base::Stmt
{
public:
  StrGT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSLT : public base::Stmt
{
public:
  StrSLT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSLT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSGT : public base::Stmt
{
public:
  StrSGT(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSGT";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrEQ : public base::Stmt
{
public:
  StrEQ(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightEQ";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrAND : public base::Stmt
{
public:
  StrAND(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightAND";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrOR : public base::Stmt
{
public:
  StrOR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrXOR : public base::Stmt
{
public:
  StrXOR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightXOR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrBYTE : public base::Stmt
{
public:
  StrBYTE(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightBYTE";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSHL : public base::Stmt
{
public:
  StrSHL(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSHL";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSHR : public base::Stmt
{
public:
  StrSHR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSHR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSAR : public base::Stmt
{
public:
  StrSAR(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSAR";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class StrSHA3 : public base::Stmt
{
public:
  StrSHA3(const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicStatementStacktopRightSHA3";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
  }
};
class SECTION : public base::Stmt
{
public:
  SECTION(std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementSection";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class SCOPE : public base::Stmt
{
public:
  SCOPE(std::pair<const std::string, const base::Expr> var, std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementScope";
    (*this)["varname"] = var.first;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(var.second);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class DOWHILE : public base::Stmt
{
public:
  DOWHILE(uint8_t tagsize, const base::Expr cond, std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementDowhile";
    (*this)["tagsize"] = tagsize;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(cond);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class UNLESS : public base::Stmt
{
public:
  UNLESS(uint8_t tagsize, const base::Expr cond, std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementUnless";
    (*this)["tagsize"] = tagsize;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(cond);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class IF : public base::Stmt
{
public:
  IF(uint8_t tagsize, const base::Expr cond, std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementIf";
    (*this)["tagsize"] = tagsize;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(cond);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class WHILE : public base::Stmt
{
public:
  WHILE(uint8_t tagsize, const base::Expr cond, std::initializer_list<const base::Stmt> children)
  {
    (*this)["$nodename$"] = "BasicStatementWhile";
    (*this)["tagsize"] = tagsize;
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(cond);
    for (auto child : children)
    {
      (*this)["$children$"].append(child);
    }
  }
};
class Set : public base::Stmt
{
public:
  Set(const std::string name, const base::Expr val)
  {
    (*this)["$nodename$"] = "BasicExpressionVarSet";
    (*this)["$children$"] = Json::Value(Json::arrayValue);
    (*this)["$children$"].append(val);
    (*this)["varname"] = name;
  }
};
class Swap : public base::Stmt
{
public:
  Swap(const std::string x, const std::string y)
  {
    (*this)["$nodename$"] = "BasicExpressionVarSwap";
    (*this)["x"] = x;
    (*this)["y"] = y;
  }
};
}
}

namespace assembly
{
classdef::STOP STOP()
{
  return classdef::STOP();
}
classdef::ADD ADD()
{
  return classdef::ADD();
}
classdef::MUL MUL()
{
  return classdef::MUL();
}
classdef::SUB SUB()
{
  return classdef::SUB();
}
classdef::DIV DIV()
{
  return classdef::DIV();
}
classdef::SDIV SDIV()
{
  return classdef::SDIV();
}
classdef::MOD MOD()
{
  return classdef::MOD();
}
classdef::SMOD SMOD()
{
  return classdef::SMOD();
}
classdef::ADDMOD ADDMOD()
{
  return classdef::ADDMOD();
}
classdef::MULMOD MULMOD()
{
  return classdef::MULMOD();
}
classdef::EXP EXP()
{
  return classdef::EXP();
}
classdef::SIGNEXTEND SIGNEXTEND()
{
  return classdef::SIGNEXTEND();
}
classdef::LT LT()
{
  return classdef::LT();
}
classdef::GT GT()
{
  return classdef::GT();
}
classdef::SLT SLT()
{
  return classdef::SLT();
}
classdef::SGT SGT()
{
  return classdef::SGT();
}
classdef::EQ EQ()
{
  return classdef::EQ();
}
classdef::ISZERO ISZERO()
{
  return classdef::ISZERO();
}
classdef::AND AND()
{
  return classdef::AND();
}
classdef::OR OR()
{
  return classdef::OR();
}
classdef::XOR XOR()
{
  return classdef::XOR();
}
classdef::NOT NOT()
{
  return classdef::NOT();
}
classdef::BYTE BYTE()
{
  return classdef::BYTE();
}
classdef::SHL SHL()
{
  return classdef::SHL();
}
classdef::SHR SHR()
{
  return classdef::SHR();
}
classdef::SAR SAR()
{
  return classdef::SAR();
}
classdef::SHA3 SHA3()
{
  return classdef::SHA3();
}
classdef::ADDRESS ADDRESS()
{
  return classdef::ADDRESS();
}
classdef::BALANCE BALANCE()
{
  return classdef::BALANCE();
}
classdef::ORIGIN ORIGIN()
{
  return classdef::ORIGIN();
}
classdef::CALLER CALLER()
{
  return classdef::CALLER();
}
classdef::CALLVALUE CALLVALUE()
{
  return classdef::CALLVALUE();
}
classdef::CALLDATALOAD CALLDATALOAD()
{
  return classdef::CALLDATALOAD();
}
classdef::CALLDATASIZE CALLDATASIZE()
{
  return classdef::CALLDATASIZE();
}
classdef::CALLDATACOPY CALLDATACOPY()
{
  return classdef::CALLDATACOPY();
}
classdef::CODESIZE CODESIZE()
{
  return classdef::CODESIZE();
}
classdef::CODECOPY CODECOPY()
{
  return classdef::CODECOPY();
}
classdef::GASPRICE GASPRICE()
{
  return classdef::GASPRICE();
}
classdef::EXTCODESIZE EXTCODESIZE()
{
  return classdef::EXTCODESIZE();
}
classdef::EXTCODECOPY EXTCODECOPY()
{
  return classdef::EXTCODECOPY();
}
classdef::RETURNDATASIZE RETURNDATASIZE()
{
  return classdef::RETURNDATASIZE();
}
classdef::RETURNDATACOPY RETURNDATACOPY()
{
  return classdef::RETURNDATACOPY();
}
classdef::EXTCODEHASH EXTCODEHASH()
{
  return classdef::EXTCODEHASH();
}
classdef::BLOCKHASH BLOCKHASH()
{
  return classdef::BLOCKHASH();
}
classdef::COINBASE COINBASE()
{
  return classdef::COINBASE();
}
classdef::TIMESTAMP TIMESTAMP()
{
  return classdef::TIMESTAMP();
}
classdef::NUMBER NUMBER()
{
  return classdef::NUMBER();
}
classdef::DIFFICULTY DIFFICULTY()
{
  return classdef::DIFFICULTY();
}
classdef::GASLIMIT GASLIMIT()
{
  return classdef::GASLIMIT();
}
classdef::CHAINID CHAINID()
{
  return classdef::CHAINID();
}
classdef::SELFBALANCE SELFBALANCE()
{
  return classdef::SELFBALANCE();
}
classdef::BASEFEE BASEFEE()
{
  return classdef::BASEFEE();
}
classdef::POP POP()
{
  return classdef::POP();
}
classdef::MLOAD MLOAD()
{
  return classdef::MLOAD();
}
classdef::MSTORE MSTORE()
{
  return classdef::MSTORE();
}
classdef::MSTORE8 MSTORE8()
{
  return classdef::MSTORE8();
}
classdef::SLOAD SLOAD()
{
  return classdef::SLOAD();
}
classdef::SSTORE SSTORE()
{
  return classdef::SSTORE();
}
classdef::JUMP JUMP()
{
  return classdef::JUMP();
}
classdef::JUMPI JUMPI()
{
  return classdef::JUMPI();
}
classdef::PC PC()
{
  return classdef::PC();
}
classdef::MSIZE MSIZE()
{
  return classdef::MSIZE();
}
classdef::GAS GAS()
{
  return classdef::GAS();
}
classdef::JUMPDEST JUMPDEST()
{
  return classdef::JUMPDEST();
}
classdef::DUP1 DUP1()
{
  return classdef::DUP1();
}
classdef::DUP2 DUP2()
{
  return classdef::DUP2();
}
classdef::DUP3 DUP3()
{
  return classdef::DUP3();
}
classdef::DUP4 DUP4()
{
  return classdef::DUP4();
}
classdef::DUP5 DUP5()
{
  return classdef::DUP5();
}
classdef::DUP6 DUP6()
{
  return classdef::DUP6();
}
classdef::DUP7 DUP7()
{
  return classdef::DUP7();
}
classdef::DUP8 DUP8()
{
  return classdef::DUP8();
}
classdef::DUP9 DUP9()
{
  return classdef::DUP9();
}
classdef::DUP10 DUP10()
{
  return classdef::DUP10();
}
classdef::DUP11 DUP11()
{
  return classdef::DUP11();
}
classdef::DUP12 DUP12()
{
  return classdef::DUP12();
}
classdef::DUP13 DUP13()
{
  return classdef::DUP13();
}
classdef::DUP14 DUP14()
{
  return classdef::DUP14();
}
classdef::DUP15 DUP15()
{
  return classdef::DUP15();
}
classdef::DUP16 DUP16()
{
  return classdef::DUP16();
}
classdef::SWAP1 SWAP1()
{
  return classdef::SWAP1();
}
classdef::SWAP2 SWAP2()
{
  return classdef::SWAP2();
}
classdef::SWAP3 SWAP3()
{
  return classdef::SWAP3();
}
classdef::SWAP4 SWAP4()
{
  return classdef::SWAP4();
}
classdef::SWAP5 SWAP5()
{
  return classdef::SWAP5();
}
classdef::SWAP6 SWAP6()
{
  return classdef::SWAP6();
}
classdef::SWAP7 SWAP7()
{
  return classdef::SWAP7();
}
classdef::SWAP8 SWAP8()
{
  return classdef::SWAP8();
}
classdef::SWAP9 SWAP9()
{
  return classdef::SWAP9();
}
classdef::SWAP10 SWAP10()
{
  return classdef::SWAP10();
}
classdef::SWAP11 SWAP11()
{
  return classdef::SWAP11();
}
classdef::SWAP12 SWAP12()
{
  return classdef::SWAP12();
}
classdef::SWAP13 SWAP13()
{
  return classdef::SWAP13();
}
classdef::SWAP14 SWAP14()
{
  return classdef::SWAP14();
}
classdef::SWAP15 SWAP15()
{
  return classdef::SWAP15();
}
classdef::SWAP16 SWAP16()
{
  return classdef::SWAP16();
}
classdef::LOG0 LOG0()
{
  return classdef::LOG0();
}
classdef::LOG1 LOG1()
{
  return classdef::LOG1();
}
classdef::LOG2 LOG2()
{
  return classdef::LOG2();
}
classdef::LOG3 LOG3()
{
  return classdef::LOG3();
}
classdef::LOG4 LOG4()
{
  return classdef::LOG4();
}
classdef::CREATE CREATE()
{
  return classdef::CREATE();
}
classdef::CALL CALL()
{
  return classdef::CALL();
}
classdef::CALLCODE CALLCODE()
{
  return classdef::CALLCODE();
}
classdef::RETURN RETURN()
{
  return classdef::RETURN();
}
classdef::DELEGATECALL DELEGATECALL()
{
  return classdef::DELEGATECALL();
}
classdef::CREATE2 CREATE2()
{
  return classdef::CREATE2();
}
classdef::STATICCALL STATICCALL()
{
  return classdef::STATICCALL();
}
classdef::REVERT REVERT()
{
  return classdef::REVERT();
}
classdef::SELFDESTRUCT SELFDESTRUCT()
{
  return classdef::SELFDESTRUCT();
}
classdef::PUSH PUSH(const std::string val)
{
  return classdef::PUSH(val);
}
classdef::PUSH PUSH(uint64_t v3, uint64_t v2, uint64_t v1, uint64_t v0)
{
  return classdef::PUSH("0x" + functiondef::ToHex(v3) + functiondef::ToFixedSizeHex(v2) + functiondef::ToFixedSizeHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::PUSH PUSH(uint64_t v2, uint64_t v1, uint64_t v0)
{
  return classdef::PUSH("0x" + functiondef::ToHex(v2) + functiondef::ToFixedSizeHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::PUSH PUSH(uint64_t v1, uint64_t v0)
{
  return classdef::PUSH("0x" + functiondef::ToHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::PUSH PUSH(uint64_t v0)
{
  return classdef::PUSH("0x" + functiondef::ToHex(v0));
}
classdef::PadSection PadSection(uint32_t padsize, std::initializer_list<const classdef::base::Base> children)
{
  return classdef::PadSection(padsize, children);
}
classdef::PadSection PadSection(uint32_t padsize)
{
  return classdef::PadSection(padsize, {});
}
classdef::PadSection PadSection(std::initializer_list<const classdef::base::Base> children)
{
  return classdef::PadSection(0, children);
}
classdef::Section Section(std::initializer_list<const classdef::base::Base> children)
{
  return classdef::Section(children);
}
classdef::Tag Tag(const std::string tag)
{
  return classdef::Tag(tag);
}
classdef::PushTag PushTag(const std::string tag, uint8_t tagsize)
{
  return classdef::PushTag(tag, tagsize);
}
classdef::PushTag PushTag(const std::string tag)
{
  return classdef::PushTag(tag, 2);
}
classdef::JumpTo JumpTo(const std::string tag, uint8_t tagsize)
{
  return classdef::JumpTo(tag, tagsize);
}
classdef::JumpTo JumpTo(const std::string tag)
{
  return classdef::JumpTo(tag, 2);
}
classdef::JumpIf JumpIf(const std::string tag, uint8_t tagsize)
{
  return classdef::JumpIf(tag, tagsize);
}
classdef::JumpIf JumpIf(const std::string tag)
{
  return classdef::JumpIf(tag, 2);
}
}

namespace basic
{
classdef::literal literal(const std::string val)
{
  return classdef::literal(val);
}
classdef::literal literal(uint64_t v3, uint64_t v2, uint64_t v1, uint64_t v0)
{
  return classdef::literal("0x" + functiondef::ToHex(v3) + functiondef::ToFixedSizeHex(v2) + functiondef::ToFixedSizeHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::literal literal(uint64_t v2, uint64_t v1, uint64_t v0)
{
  return classdef::literal("0x" + functiondef::ToHex(v2) + functiondef::ToFixedSizeHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::literal literal(uint64_t v1, uint64_t v0)
{
  return classdef::literal("0x" + functiondef::ToHex(v1) + functiondef::ToFixedSizeHex(v0));
}
classdef::literal literal(uint64_t v0)
{
  return classdef::literal("0x" + functiondef::ToHex(v0));
}
classdef::stack stack(uint8_t index)
{
  return classdef::stack(index);
}
classdef::placeholder placeholder()
{
  return classdef::placeholder();
}
classdef::calldata calldata(const classdef::base::Expr offset)
{
  return classdef::calldata(offset);
}
classdef::memory memory(const classdef::base::Expr offset)
{
  return classdef::memory(offset);
}
classdef::storage storage(const classdef::base::Expr offset)
{
  return classdef::storage(offset);
}
classdef::address address()
{
  return classdef::address();
}
classdef::origin origin()
{
  return classdef::origin();
}
classdef::caller caller()
{
  return classdef::caller();
}
classdef::callvalue callvalue()
{
  return classdef::callvalue();
}
classdef::calldatasize calldatasize()
{
  return classdef::calldatasize();
}
classdef::codesize codesize()
{
  return classdef::codesize();
}
classdef::gasprice gasprice()
{
  return classdef::gasprice();
}
classdef::returndatasize returndatasize()
{
  return classdef::returndatasize();
}
classdef::codebase codebase()
{
  return classdef::codebase();
}
classdef::timestamp timestamp()
{
  return classdef::timestamp();
}
classdef::number number()
{
  return classdef::number();
}
classdef::difficulty difficulty()
{
  return classdef::difficulty();
}
classdef::gaslimit gaslimit()
{
  return classdef::gaslimit();
}
classdef::chainid chainid()
{
  return classdef::chainid();
}
classdef::selfbalance selfbalance()
{
  return classdef::selfbalance();
}
classdef::basefee basefee()
{
  return classdef::basefee();
}
classdef::pc pc()
{
  return classdef::pc();
}
classdef::msize msize()
{
  return classdef::msize();
}
classdef::gas gas()
{
  return classdef::gas();
}
classdef::iszero iszero(const classdef::base::Expr val)
{
  return classdef::iszero(val);
}
classdef::not_ not_(const classdef::base::Expr val)
{
  return classdef::not_(val);
}
classdef::balance balance(const classdef::base::Expr val)
{
  return classdef::balance(val);
}
classdef::calldataload calldataload(const classdef::base::Expr val)
{
  return classdef::calldataload(val);
}
classdef::extcodesize extcodesize(const classdef::base::Expr val)
{
  return classdef::extcodesize(val);
}
classdef::extcodehash extcodehash(const classdef::base::Expr val)
{
  return classdef::extcodehash(val);
}
classdef::blockhash blockhash(const classdef::base::Expr val)
{
  return classdef::blockhash(val);
}
classdef::mload mload(const classdef::base::Expr val)
{
  return classdef::mload(val);
}
classdef::sload sload(const classdef::base::Expr val)
{
  return classdef::sload(val);
}
classdef::add add(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::add(a, b);
}
classdef::mul mul(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::mul(a, b);
}
classdef::sub sub(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::sub(a, b);
}
classdef::div div(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::div(a, b);
}
classdef::sdiv sdiv(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::sdiv(a, b);
}
classdef::mod mod(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::mod(a, b);
}
classdef::smod smod(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::smod(a, b);
}
classdef::exp exp(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::exp(a, b);
}
classdef::signextend signextend(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::signextend(a, b);
}
classdef::lt lt(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::lt(a, b);
}
classdef::gt gt(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::gt(a, b);
}
classdef::slt slt(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::slt(a, b);
}
classdef::sgt sgt(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::sgt(a, b);
}
classdef::eq eq(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::eq(a, b);
}
classdef::and_ and_(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::and_(a, b);
}
classdef::or_ or_(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::or_(a, b);
}
classdef::xor_ xor_(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::xor_(a, b);
}
classdef::byte byte(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::byte(a, b);
}
classdef::shl shl(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::shl(a, b);
}
classdef::shr shr(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::shr(a, b);
}
classdef::sar sar(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::sar(a, b);
}
classdef::sha3 sha3(const classdef::base::Expr a, const classdef::base::Expr b)
{
  return classdef::sha3(a, b);
}
classdef::addmod addmod(const classdef::base::Expr a, const classdef::base::Expr b, const classdef::base::Expr n)
{
  return classdef::addmod(a, b, n);
}
classdef::mulmod mulmod(const classdef::base::Expr a, const classdef::base::Expr b, const classdef::base::Expr n)
{
  return classdef::mulmod(a, b, n);
}
classdef::create create(const classdef::base::Expr value, const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::create(value, offset, size);
}
classdef::create2 create2(const classdef::base::Expr value, const classdef::base::Expr offset, const classdef::base::Expr size, const classdef::base::Expr salt)
{
  return classdef::create2(value, offset, size, salt);
}
classdef::delegatecall delegatecall(const classdef::base::Expr gas, const classdef::base::Expr address, const classdef::base::Expr argsoffset, const classdef::base::Expr argssize, const classdef::base::Expr retoffset, const classdef::base::Expr retsize)
{
  return classdef::delegatecall(gas, address, argsoffset, argssize, retoffset, retsize);
}
classdef::staticcall staticcall(const classdef::base::Expr gas, const classdef::base::Expr address, const classdef::base::Expr argsoffset, const classdef::base::Expr argssize, const classdef::base::Expr retoffset, const classdef::base::Expr retsize)
{
  return classdef::staticcall(gas, address, argsoffset, argssize, retoffset, retsize);
}
classdef::call call(const classdef::base::Expr gas, const classdef::base::Expr address, const classdef::base::Expr value, const classdef::base::Expr argsoffset, const classdef::base::Expr argssize, const classdef::base::Expr retoffset, const classdef::base::Expr retsize)
{
  return classdef::call(gas, address, value, argsoffset, argssize, retoffset, retsize);
}
classdef::callcode callcode(const classdef::base::Expr gas, const classdef::base::Expr address, const classdef::base::Expr value, const classdef::base::Expr argsoffset, const classdef::base::Expr argssize, const classdef::base::Expr retoffset, const classdef::base::Expr retsize)
{
  return classdef::callcode(gas, address, value, argsoffset, argssize, retoffset, retsize);
}
classdef::var var(const std::string name)
{
  return classdef::var(name);
}
classdef::Ret Ret(const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::Ret(offset, size);
}
classdef::Rvt Rvt(const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::Rvt(offset, size);
}
classdef::Die Die(const classdef::base::Expr address)
{
  return classdef::Die(address);
}
classdef::End End()
{
  return classdef::End();
}
classdef::Log Log(const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::Log(offset, size, {});
}
classdef::Log Log(const classdef::base::Expr offset, const classdef::base::Expr size, const classdef::base::Expr topic1)
{
  return classdef::Log(offset, size, {topic1});
}
classdef::Log Log(const classdef::base::Expr offset, const classdef::base::Expr size, const classdef::base::Expr topic1, const classdef::base::Expr topic2)
{
  return classdef::Log(offset, size, {topic1, topic2});
}
classdef::Log Log(const classdef::base::Expr offset, const classdef::base::Expr size, const classdef::base::Expr topic1, const classdef::base::Expr topic2, const classdef::base::Expr topic3)
{
  return classdef::Log(offset, size, {topic1, topic2, topic3});
}
classdef::Log Log(const classdef::base::Expr offset, const classdef::base::Expr size, const classdef::base::Expr topic1, const classdef::base::Expr topic2, const classdef::base::Expr topic3, const classdef::base::Expr topic4)
{
  return classdef::Log(offset, size, {topic1, topic2, topic3, topic4});
}
classdef::Drop Drop(const classdef::base::Expr val)
{
  return classdef::Drop(val);
}
classdef::MemcopyExtcode MemcopyExtcode(const classdef::base::Expr address, const classdef::base::Expr dest, const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::MemcopyExtcode(address, dest, offset, size);
}
classdef::MemcopyCode MemcopyCode(const classdef::base::Expr dest, const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::MemcopyCode(dest, offset, size);
}
classdef::MemcopyCalldata MemcopyCalldata(const classdef::base::Expr dest, const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::MemcopyCalldata(dest, offset, size);
}
classdef::MemcopyReturndata MemcopyReturndata(const classdef::base::Expr dest, const classdef::base::Expr offset, const classdef::base::Expr size)
{
  return classdef::MemcopyReturndata(dest, offset, size);
}
classdef::StorageSet StorageSet(const classdef::base::Expr offset, const classdef::base::Expr val)
{
  return classdef::StorageSet(offset, val);
}
classdef::MemorySet MemorySet(const classdef::base::Expr offset, const classdef::base::Expr val)
{
  return classdef::MemorySet(offset, val);
}
classdef::MemorySetByte MemorySetByte(const classdef::base::Expr offset, const classdef::base::Expr val)
{
  return classdef::MemorySetByte(offset, val);
}
classdef::StackSet StackSet(uint8_t index, const classdef::base::Expr val)
{
  return classdef::StackSet(index, val);
}
classdef::StackSwap StackSwap(uint8_t x, uint8_t y)
{
  return classdef::StackSwap(x, y);
}
classdef::StISZERO StISZERO()
{
  return classdef::StISZERO();
}
classdef::StNOT StNOT()
{
  return classdef::StNOT();
}
classdef::StBALANCE StBALANCE()
{
  return classdef::StBALANCE();
}
classdef::StCALLDATALOAD StCALLDATALOAD()
{
  return classdef::StCALLDATALOAD();
}
classdef::StEXTCODESIZE StEXTCODESIZE()
{
  return classdef::StEXTCODESIZE();
}
classdef::StEXTCODEHASH StEXTCODEHASH()
{
  return classdef::StEXTCODEHASH();
}
classdef::StBLOCKHASH StBLOCKHASH()
{
  return classdef::StBLOCKHASH();
}
classdef::StMLOAD StMLOAD()
{
  return classdef::StMLOAD();
}
classdef::StSLOAD StSLOAD()
{
  return classdef::StSLOAD();
}
classdef::StlADD StlADD(const classdef::base::Expr val)
{
  return classdef::StlADD(val);
}
classdef::StlMUL StlMUL(const classdef::base::Expr val)
{
  return classdef::StlMUL(val);
}
classdef::StlSUB StlSUB(const classdef::base::Expr val)
{
  return classdef::StlSUB(val);
}
classdef::StlDIV StlDIV(const classdef::base::Expr val)
{
  return classdef::StlDIV(val);
}
classdef::StlSDIV StlSDIV(const classdef::base::Expr val)
{
  return classdef::StlSDIV(val);
}
classdef::StlMOD StlMOD(const classdef::base::Expr val)
{
  return classdef::StlMOD(val);
}
classdef::StlSMOD StlSMOD(const classdef::base::Expr val)
{
  return classdef::StlSMOD(val);
}
classdef::StlEXP StlEXP(const classdef::base::Expr val)
{
  return classdef::StlEXP(val);
}
classdef::StlSIGNEXTEND StlSIGNEXTEND(const classdef::base::Expr val)
{
  return classdef::StlSIGNEXTEND(val);
}
classdef::StlLT StlLT(const classdef::base::Expr val)
{
  return classdef::StlLT(val);
}
classdef::StlGT StlGT(const classdef::base::Expr val)
{
  return classdef::StlGT(val);
}
classdef::StlSLT StlSLT(const classdef::base::Expr val)
{
  return classdef::StlSLT(val);
}
classdef::StlSGT StlSGT(const classdef::base::Expr val)
{
  return classdef::StlSGT(val);
}
classdef::StlEQ StlEQ(const classdef::base::Expr val)
{
  return classdef::StlEQ(val);
}
classdef::StlAND StlAND(const classdef::base::Expr val)
{
  return classdef::StlAND(val);
}
classdef::StlOR StlOR(const classdef::base::Expr val)
{
  return classdef::StlOR(val);
}
classdef::StlXOR StlXOR(const classdef::base::Expr val)
{
  return classdef::StlXOR(val);
}
classdef::StlBYTE StlBYTE(const classdef::base::Expr val)
{
  return classdef::StlBYTE(val);
}
classdef::StlSHL StlSHL(const classdef::base::Expr val)
{
  return classdef::StlSHL(val);
}
classdef::StlSHR StlSHR(const classdef::base::Expr val)
{
  return classdef::StlSHR(val);
}
classdef::StlSAR StlSAR(const classdef::base::Expr val)
{
  return classdef::StlSAR(val);
}
classdef::StlSHA3 StlSHA3(const classdef::base::Expr val)
{
  return classdef::StlSHA3(val);
}
classdef::StrADD StrADD(const classdef::base::Expr val)
{
  return classdef::StrADD(val);
}
classdef::StrMUL StrMUL(const classdef::base::Expr val)
{
  return classdef::StrMUL(val);
}
classdef::StrSUB StrSUB(const classdef::base::Expr val)
{
  return classdef::StrSUB(val);
}
classdef::StrDIV StrDIV(const classdef::base::Expr val)
{
  return classdef::StrDIV(val);
}
classdef::StrSDIV StrSDIV(const classdef::base::Expr val)
{
  return classdef::StrSDIV(val);
}
classdef::StrMOD StrMOD(const classdef::base::Expr val)
{
  return classdef::StrMOD(val);
}
classdef::StrSMOD StrSMOD(const classdef::base::Expr val)
{
  return classdef::StrSMOD(val);
}
classdef::StrEXP StrEXP(const classdef::base::Expr val)
{
  return classdef::StrEXP(val);
}
classdef::StrSIGNEXTEND StrSIGNEXTEND(const classdef::base::Expr val)
{
  return classdef::StrSIGNEXTEND(val);
}
classdef::StrLT StrLT(const classdef::base::Expr val)
{
  return classdef::StrLT(val);
}
classdef::StrGT StrGT(const classdef::base::Expr val)
{
  return classdef::StrGT(val);
}
classdef::StrSLT StrSLT(const classdef::base::Expr val)
{
  return classdef::StrSLT(val);
}
classdef::StrSGT StrSGT(const classdef::base::Expr val)
{
  return classdef::StrSGT(val);
}
classdef::StrEQ StrEQ(const classdef::base::Expr val)
{
  return classdef::StrEQ(val);
}
classdef::StrAND StrAND(const classdef::base::Expr val)
{
  return classdef::StrAND(val);
}
classdef::StrOR StrOR(const classdef::base::Expr val)
{
  return classdef::StrOR(val);
}
classdef::StrXOR StrXOR(const classdef::base::Expr val)
{
  return classdef::StrXOR(val);
}
classdef::StrBYTE StrBYTE(const classdef::base::Expr val)
{
  return classdef::StrBYTE(val);
}
classdef::StrSHL StrSHL(const classdef::base::Expr val)
{
  return classdef::StrSHL(val);
}
classdef::StrSHR StrSHR(const classdef::base::Expr val)
{
  return classdef::StrSHR(val);
}
classdef::StrSAR StrSAR(const classdef::base::Expr val)
{
  return classdef::StrSAR(val);
}
classdef::StrSHA3 StrSHA3(const classdef::base::Expr val)
{
  return classdef::StrSHA3(val);
}
classdef::SECTION SECTION(std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::SECTION(children);
}
classdef::SCOPE SCOPE(std::pair<const std::string, const classdef::base::Expr> var, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::SCOPE(var, children);
}
classdef::DOWHILE DOWHILE(uint8_t tagsize, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::DOWHILE(tagsize, cond, children);
}
classdef::DOWHILE DOWHILE(const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::DOWHILE(2, cond, children);
}
classdef::UNLESS UNLESS(uint8_t tagsize, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::UNLESS(tagsize, cond, children);
}
classdef::UNLESS UNLESS(const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::UNLESS(2, cond, children);
}
classdef::IF IF(uint8_t tagsize, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::IF(tagsize, cond, children);
}
classdef::IF IF(const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::IF(2, cond, children);
}
classdef::WHILE WHILE(uint8_t tagsize, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::WHILE(tagsize, cond, children);
}
classdef::WHILE WHILE(const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> children)
{
  return classdef::WHILE(2, cond, children);
}
classdef::Set Set(const std::string name, const classdef::base::Expr val)
{
  return classdef::Set(name, val);
}
classdef::Swap Swap(const std::string x, const std::string y)
{
  return classdef::Swap(x, y);
}

namespace extension
{
classdef::base::Stmt SCOPESECTION(std::initializer_list<std::pair<const std::string, const classdef::base::Expr>> vars, std::initializer_list<const classdef::base::Stmt> stmts)
{
  classdef::base::Stmt ret = classdef::SECTION(stmts);
  for (auto var : vars)
  {
    ret = classdef::SCOPE(var, {ret});
  }
  return ret;
}
classdef::base::Stmt DOWHILESCOPE(uint8_t tagsize, std::pair<const std::string, const classdef::base::Expr> var, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> stmts, std::initializer_list<const classdef::base::Stmt> init = {}, std::initializer_list<const classdef::base::Stmt> final = {})
{
  return classdef::SCOPE(var, {classdef::SECTION(init), classdef::DOWHILE(tagsize, cond, stmts), classdef::SECTION(final)});
}
classdef::base::Stmt DOWHILESCOPE(std::pair<const std::string, const classdef::base::Expr> var, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> stmts, std::initializer_list<const classdef::base::Stmt> init = {}, std::initializer_list<const classdef::base::Stmt> final = {})
{
  return classdef::SCOPE(var, {classdef::SECTION(init), classdef::DOWHILE(2, cond, stmts), classdef::SECTION(final)});
}
classdef::base::Stmt WHILESCOPE(uint8_t tagsize, std::pair<const std::string, const classdef::base::Expr> var, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> stmts, std::initializer_list<const classdef::base::Stmt> init = {}, std::initializer_list<const classdef::base::Stmt> final = {})
{
  return classdef::SCOPE(var, {classdef::SECTION(init), classdef::WHILE(tagsize, cond, stmts), classdef::SECTION(final)});
}
classdef::base::Stmt WHILESCOPE(std::pair<const std::string, const classdef::base::Expr> var, const classdef::base::Expr cond, std::initializer_list<const classdef::base::Stmt> stmts, std::initializer_list<const classdef::base::Stmt> init = {}, std::initializer_list<const classdef::base::Stmt> final = {})
{
  return classdef::SCOPE(var, {classdef::SECTION(init), classdef::WHILE(2, cond, stmts), classdef::SECTION(final)});
}
classdef::base::Stmt MemorySetSlot(const classdef::base::Expr index, const classdef::base::Expr val)
{
  return classdef::MemorySet(index * 0x20, val);
}
classdef::base::Stmt StorageSetSlot(const classdef::base::Expr index, const classdef::base::Expr val)
{
  return classdef::StorageSet(index * 0x20, val);
}
classdef::base::Stmt RetVal(const classdef::base::Expr val)
{
  return classdef::SECTION({classdef::MemorySet(0, val), classdef::Ret(0, 0x20)});
}
classdef::base::Stmt RvtVal(const classdef::base::Expr val)
{
  return classdef::SECTION({classdef::MemorySet(0, val), classdef::Rvt(0, 0x20)});
}
}
}
