// Copyright (c) 2014-2024, The Vindex Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "hardforks.h"

#undef vindex_DEFAULT_LOG_CATEGORY
#define vindex_DEFAULT_LOG_CATEGORY "blockchain.hardforks"

// ---------------------------------------------------------------------------
// VINDEX HARD FORK SCHEDULE
//
// Vindex launches fresh from genesis — no Monero history is inherited.
// All forks activate from block 0 (genesis). The chain begins already at
// protocol version 16 (full feature parity with Monero v0.18), then adds
// Vindex-specific upgrades via future forks.
//
// IMPORTANT: Heights must be 0 (not 1) so that the genesis block itself
// is validated under hf_version=1. The HardFork object uses the height
// of the *current* block being added, so genesis (height=0) must fall
// under version 1. All subsequent forks also start at 0 so the chain
// immediately operates at v16 from block 1 onward.
//
// Post-Quantum Roadmap (defined here for transparency):
//   v17: Hybrid Ed25519 + CRYSTALS-Dilithium wallet keys (planned)
//   v18: Full CRYSTALS-Dilithium (ML-DSA) + Bulletproofs+ (future)
//   v19: Ring size upgrade to 16 once chain has >100k outputs (planned)
// ---------------------------------------------------------------------------

const hardfork_t mainnet_hard_forks[] = {
  // Vindex launches with all Monero v0.18 features active from genesis.
  // Heights are 0 so the hardfork object reports v1 at genesis (height 0),
  // then immediately steps up to v16 for all subsequent blocks.
  // Timestamp: 2026-05-22 00:00:00 UTC = 1748044800
  { 1,  0, 0, 1748044800 },
  { 2,  0, 0, 1748044801 },
  { 3,  0, 0, 1748044802 },
  { 4,  0, 0, 1748044803 },
  { 5,  0, 0, 1748044804 },
  { 6,  0, 0, 1748044805 },
  { 7,  0, 0, 1748044806 },
  { 8,  0, 0, 1748044807 },
  { 9,  0, 0, 1748044808 },
  { 10, 0, 0, 1748044809 },
  { 11, 0, 0, 1748044810 },
  { 12, 0, 0, 1748044811 },
  { 13, 0, 0, 1748044812 },
  { 14, 0, 0, 1748044813 },
  { 15, 0, 0, 1748044814 },
  { 16, 0, 0, 1748044815 },
  // v17: Ring size 16 + Hybrid PQ signatures (planned ~100k blocks)
  // { 17, 100000, 0, TBD },
};
const size_t num_mainnet_hard_forks = sizeof(mainnet_hard_forks) / sizeof(mainnet_hard_forks[0]);
const uint64_t mainnet_hard_fork_version_1_till = 0; // no v1-only period; all forks active from block 0

const hardfork_t testnet_hard_forks[] = {
  { 1,  0, 0, 1748044800 },
  { 2,  0, 0, 1748044801 },
  { 3,  0, 0, 1748044802 },
  { 4,  0, 0, 1748044803 },
  { 5,  0, 0, 1748044804 },
  { 6,  0, 0, 1748044805 },
  { 7,  0, 0, 1748044806 },
  { 8,  0, 0, 1748044807 },
  { 9,  0, 0, 1748044808 },
  { 10, 0, 0, 1748044809 },
  { 11, 0, 0, 1748044810 },
  { 12, 0, 0, 1748044811 },
  { 13, 0, 0, 1748044812 },
  { 14, 0, 0, 1748044813 },
  { 15, 0, 0, 1748044814 },
  { 16, 0, 0, 1748044815 },
};
const size_t num_testnet_hard_forks = sizeof(testnet_hard_forks) / sizeof(testnet_hard_forks[0]);
const uint64_t testnet_hard_fork_version_1_till = 0;

const hardfork_t stagenet_hard_forks[] = {
  { 1,  0, 0, 1748044800 },
  { 2,  0, 0, 1748044801 },
  { 3,  0, 0, 1748044802 },
  { 4,  0, 0, 1748044803 },
  { 5,  0, 0, 1748044804 },
  { 6,  0, 0, 1748044805 },
  { 7,  0, 0, 1748044806 },
  { 8,  0, 0, 1748044807 },
  { 9,  0, 0, 1748044808 },
  { 10, 0, 0, 1748044809 },
  { 11, 0, 0, 1748044810 },
  { 12, 0, 0, 1748044811 },
  { 13, 0, 0, 1748044812 },
  { 14, 0, 0, 1748044813 },
  { 15, 0, 0, 1748044814 },
  { 16, 0, 0, 1748044815 },
};
const size_t num_stagenet_hard_forks = sizeof(stagenet_hard_forks) / sizeof(stagenet_hard_forks[0]);
