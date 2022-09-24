// SPDX-License-Identifier: MIT
pragma solidity >0.8.0;

contract fibonacci {
    fallback(bytes calldata I) external returns (bytes memory O) {
        uint n = abi.decode(I, (uint));
        uint x = 0;
        uint y = 1;
        for (uint i = 0; i <= n; i++) {
            (x, y) = (y, x + y);
        }
        O = abi.encode(x);
    }
}
