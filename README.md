# DEX Pool Analysis Tools

Analyzes DEX pool connectivity to identify arbitrage opportunities by finding pool pairs that lack direct triangular connections.

## Overview

Two-stage pipeline for analyzing token relationships in decentralized exchange pools:

1. **index.js** - Parses JSON pool/token data and generates indexed binary representations
2. **2_pools.c** - Identifies connected pool pairs missing triangular arbitrage paths

## Usage

```bash
# Stage 1: Index pools and tokens
node index.js

# Stage 2: Find pool pairs without triangular connections  
gcc -o 2_pools 2_pools.c
./2_pools
```

## Output

- **Indexed data** - Binary file containing pool/token relationships
- **2_pools.bin** - Pool pairs lacking third-pool connections for complete triangular arbitrage

## Requirements

- Node.js (no dependencies)
- GCC compiler
- JSON input file with DEX pool/token relationships

## Use Case

Identifies potential arbitrage opportunities by finding pool pairs where tokens can be exchanged directly but lack a third pool to complete triangular trades between all three tokens involved.
