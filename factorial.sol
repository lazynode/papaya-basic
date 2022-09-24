// SPDX-License-Identifier: MIT
pragma solidity >0.8.0;

contract factorial {
    fallback(bytes calldata I) external returns (bytes memory O) {
        uint x = abi.decode(I, (uint));
        uint n = 1;
        for (uint i = 1; i <= x; i++) {
            n *= i;
        }
        O = abi.encode(n);
    }
}
