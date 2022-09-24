// SPDX-License-Identifier: MIT
pragma solidity >0.8.0;

contract addition {
    fallback(bytes calldata I) external returns (bytes memory O) {
        (uint x, uint y) = abi.decode(I, (uint, uint));
        O = abi.encode(x + y);
    }
}
