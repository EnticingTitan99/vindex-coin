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
// v1 must be at height 0 so the genesis block (always major_version=1)
// passes the ideal_hf_version check in blockchain.cpp init. All forks
// from v2 onward activate at height 1, so the chain operates at v16
// from block 1 onward.
//
// Post-Quantum Roadmap (defined here for transparency):
//   v17: Hybrid Ed25519 + CRYSTALS-Dilithium wallet keys (planned)
//   v18: Full CRYSTALS-Dilithium (ML-DSA) + Bulletproofs+ (future)
//   v19: Ring size upgrade to 16 once chain has >100k outputs (planned)
// ---------------------------------------------------------------------------

const hardfork_t mainnet_hard_forks[] = {
  // v1 owns genesis (height 0); all subsequent forks activate at height 1
  // Timestamp: 2026-05-22 00:00:00 UTC = 1748044800
  { 1,  0, 0, 1748044800 },
  { 2,  1, 0, 1748044801 },
  { 3,  1, 0, 1748044802 },
  { 4,  1, 0, 1748044803 },
  { 5,  1, 0, 1748044804 },
  { 6,  1, 0, 1748044805 },
  { 7,  1, 0, 1748044806 },
  { 8,  1, 0, 1748044807 },
  { 9,  1, 0, 1748044808 },
  { 10, 1, 0, 1748044809 },
  { 11, 1, 0, 1748044810 },
  { 12, 1, 0, 1748044811 },
  { 13, 1, 0, 1748044812 },
  { 14, 1, 0, 1748044813 },
  { 15, 1, 0, 1748044814 },
  { 16, 1, 0, 1748044815 },
  // v17: Ring size 16 + Hybrid PQ signatures (planned ~100k blocks)
  // { 17, 100000, 0, TBD },
};
const size_t num_mainnet_hard_forks = sizeof(mainnet_hard_forks) / sizeof(mainnet_hard_forks[0]);
const uint64_t mainnet_hard_fork_version_1_till = 0;

const hardfork_t testnet_hard_forks[] = {
  { 1,  0, 0, 1748044800 },
  { 2,  1, 0, 1748044801 },
  { 3,  1, 0, 1748044802 },
  { 4,  1, 0, 1748044803 },
  { 5,  1, 0, 1748044804 },
  { 6,  1, 0, 1748044805 },
  { 7,  1, 0, 1748044806 },
  { 8,  1, 0, 1748044807 },
  { 9,  1, 0, 1748044808 },
  { 10, 1, 0, 1748044809 },
  { 11, 1, 0, 1748044810 },
  { 12, 1, 0, 1748044811 },
  { 13, 1, 0, 1748044812 },
  { 14, 1, 0, 1748044813 },
  { 15, 1, 0, 1748044814 },
  { 16, 1, 0, 1748044815 },
};
const size_t num_testnet_hard_forks = sizeof(testnet_hard_forks) / sizeof(testnet_hard_forks[0]);
const uint64_t testnet_hard_fork_version_1_till = 0;

const hardfork_t stagenet_hard_forks[] = {
  { 1,  0, 0, 1748044800 },
  { 2,  1, 0, 1748044801 },
  { 3,  1, 0, 1748044802 },
  { 4,  1, 0, 1748044803 },
  { 5,  1, 0, 1748044804 },
  { 6,  1, 0, 1748044805 },
  { 7,  1, 0, 1748044806 },
  { 8,  1, 0, 1748044807 },
  { 9,  1, 0, 1748044808 },
  { 10, 1, 0, 1748044809 },
  { 11, 1, 0, 1748044810 },
  { 12, 1, 0, 1748044811 },
  { 13, 1, 0, 1748044812 },
  { 14, 1, 0, 1748044813 },
  { 15, 1, 0, 1748044814 },
  { 16, 1, 0, 1748044815 },
};
const size_t num_stagenet_hard_forks = sizeof(stagenet_hard_forks) / sizeof(stagenet_hard_forks[0]);
