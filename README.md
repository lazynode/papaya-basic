# papaya

yet another evm compiler

## dependencies

```sh
sudo apt update
sudo apt install nodejs npm
sudo apt install g++
sudo apt install libjsoncpp-dev
npm install
```

## compile

write your smart contact in `contract.cpp` and run the following command:

> **ALL WARNING MESSAGES SHOULD BE CAREFULLY REVIEWED**

```sh
g++ contract.cpp -ljsoncpp -o contract && ./contract | node papaya.js
```

## language

write code in cpp files since papaya compilation depends on g++

> c++ is a powerful static type language; g++ is the lexical and syntactic analyzer and grammer checker of papaya;

choose any of the supported language packs as you want to implement your smart contract:

> using multiple language packs together is also supported

- [assembly](https://github.com/lazynode/papaya-assembly): assembly language with smart push, tag and jump ... support
- [basic](https://github.com/lazynode/papaya-basic): high level language as efficient as assembly
- cando: TODO