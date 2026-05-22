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
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <boost/uuid/uuid.hpp>

// ---------------------------------------------------------------------------
// VINDEX FORK CONFIGURATION
// Ticker: VDX | Tagline: Libertatis Vindex (Champion of Liberty)
// Forked from Monero v0.18.x (Fluorine Fermi)
// ---------------------------------------------------------------------------

#define CRYPTONOTE_DNS_TIMEOUT_MS                       20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER                     500000000
#define CRYPTONOTE_MAX_TX_SIZE                          1000000
#define CRYPTONOTE_MAX_TX_PER_BLOCK                     0x10000000
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER          0
#define CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW            60
#define CURRENT_TRANSACTION_VERSION                     2
#define CURRENT_BLOCK_MAJOR_VERSION                     1
#define CURRENT_BLOCK_MINOR_VERSION                     0
// FIX: Tightened from 60*60*2 (2 hours) to 60*30 (30 minutes).
// A 2-hour future-time window is excessive for a 60s block-time chain — it
// creates a large DoS surface where an attacker can submit blocks with
// timestamps far in the future to stall difficulty adjustment.
// 30 minutes = 30 blocks of leeway, which is generous for clock skew.
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT              60*30
#define CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE             10

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW               60

// MONEY_SUPPLY - total number coins to be generated
#define MONEY_SUPPLY                                    ((uint64_t)(-1))
// Adaptive tail emission is implemented in cryptonote_basic_impl.cpp
// Base: 0.3 VDX/block, max: 0.6 VDX/block (scales with tx activity)
#define EMISSION_SPEED_FACTOR_PER_MINUTE                (20)
#define FINAL_SUBSIDY_PER_MINUTE                        ((uint64_t)300000000000) // 0.3 VDX base tail emission
#define FINAL_SUBSIDY_PER_MINUTE_MAX                    ((uint64_t)600000000000) // 0.6 VDX max tail emission
// Adaptive emission: if rolling 100-block median tx count > 50 -> max subsidy
//                    if > 10 -> base + (tx_count * 6000000000)
//                    else    -> base subsidy
// See: src/cryptonote_basic/cryptonote_basic_impl.cpp get_block_reward()

#define CRYPTONOTE_REWARD_BLOCKS_WINDOW                 100
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2    60000
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1    20000
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5    300000
#define CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE   100000
#define CRYPTONOTE_SHORT_TERM_BLOCK_WEIGHT_SURGE_FACTOR 50
#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE          600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT                12
// COIN - number of smallest units in one coin (1 VDX = 10^12 atomic units)
#define COIN                                            ((uint64_t)1000000000000)

#define FEE_PER_KB_OLD                                  ((uint64_t)10000000000)
#define FEE_PER_KB                                      ((uint64_t)2000000000)
#define FEE_PER_BYTE                                    ((uint64_t)300000)
#define DYNAMIC_FEE_PER_KB_BASE_FEE                     ((uint64_t)2000000000)
#define DYNAMIC_FEE_PER_KB_BASE_BLOCK_REWARD            ((uint64_t)10000000000000)
#define DYNAMIC_FEE_PER_KB_BASE_FEE_V5                  ((uint64_t)2000000000 * (uint64_t)CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 / CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5)
#define DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT         ((uint64_t)3000)

#define ORPHANED_BLOCKS_MAX_COUNT                       100

// ---------------------------------------------------------------------------
// VINDEX DIFFERENTIATOR 1: Faster block time (60s vs Monero 120s)
// - 2x faster confirmations for users
// - More frequent difficulty adjustments = smoother difficulty curve
// NOTE: DIFFICULTY_WINDOW=720 gives ~12 hours of adjustment history at 60s/block.
//       Monero uses 720 at 120s/block = ~24 hours. Vindex intentionally halves
//       this to react faster to hash rate swings on a young chain.
// ---------------------------------------------------------------------------
#define DIFFICULTY_TARGET_V2                            60   // seconds (Vindex: 60s, Monero: 120s)
#define DIFFICULTY_TARGET_V1                            60   // seconds
#define DIFFICULTY_WINDOW                               720  // blocks (~12 hours at 60s/block)
#define DIFFICULTY_LAG                                  15
#define DIFFICULTY_CUT                                  60
#define DIFFICULTY_BLOCKS_COUNT                         DIFFICULTY_WINDOW + DIFFICULTY_LAG

// ---------------------------------------------------------------------------
// VINDEX DIFFERENTIATOR 2: Tighter ring size default (11 vs Monero 16)
// Plan: hard fork to 16 once chain has >100k outputs (v19 roadmap)
// GUARD: Do NOT lower this below 11 — minimum enforced at HF_VERSION_MIN_MIXIN_10
// ---------------------------------------------------------------------------
#define CRYPTONOTE_DEFAULT_RING_SIZE                    11

#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1   DIFFICULTY_TARGET_V1 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V2   DIFFICULTY_TARGET_V2 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS       1

#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN             DIFFICULTY_TARGET_V1

#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT          10000
#define BLOCKS_IDS_SYNCHRONIZING_MAX_COUNT              25000
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT_PRE_V4       100
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT              20
#define BLOCKS_SYNCHRONIZING_MAX_COUNT                  2048
#define BATCH_MAX_WEIGHT                                10
#define BATCH_MAX_ALLOWED_WEIGHT                        50
#define BLOCKS_MAX_WINDOW                               CRYPTONOTE_REWARD_BLOCKS_WINDOW

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME                    (86400*3)
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME     604800

#define CRYPTONOTE_DANDELIONPP_STEMS              2
#define CRYPTONOTE_DANDELIONPP_FLUFF_PROBABILITY 20
#define CRYPTONOTE_DANDELIONPP_MIN_EPOCH         10
#define CRYPTONOTE_DANDELIONPP_EPOCH_RANGE       30
#define CRYPTONOTE_DANDELIONPP_FLUSH_AVERAGE      5
#define CRYPTONOTE_DANDELIONPP_EMBARGO_AVERAGE   39

#define CRYPTONOTE_NOISE_MIN_EPOCH                      5
#define CRYPTONOTE_NOISE_EPOCH_RANGE                    30
#define CRYPTONOTE_NOISE_MIN_DELAY                      10
#define CRYPTONOTE_NOISE_DELAY_RANGE                    5
#define CRYPTONOTE_NOISE_BYTES                          3*1024
#define CRYPTONOTE_NOISE_CHANNELS                       2

#define CRYPTONOTE_FORWARD_DELAY_BASE (CRYPTONOTE_NOISE_MIN_DELAY + CRYPTONOTE_NOISE_DELAY_RANGE)
#define CRYPTONOTE_FORWARD_DELAY_AVERAGE (CRYPTONOTE_FORWARD_DELAY_BASE + (CRYPTONOTE_FORWARD_DELAY_BASE / 2))

#define CRYPTONOTE_MAX_FRAGMENTS                        20

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_BLOCK_COUNT     1000
#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_TX_COUNT        20000
#define DEFAULT_RPC_MAX_CONNECTIONS_PER_PUBLIC_IP       3
#define DEFAULT_RPC_MAX_CONNECTIONS_PER_PRIVATE_IP      25
#define DEFAULT_RPC_MAX_CONNECTIONS                     100
#define DEFAULT_RPC_SOFT_LIMIT_SIZE                     25 * 1024 * 1024
#define MAX_RPC_CONTENT_LENGTH                          1048576

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT                  1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT                   5000

#define P2P_DEFAULT_CONNECTIONS_COUNT                   12
#define P2P_DEFAULT_HANDSHAKE_INTERVAL                  60
#define P2P_DEFAULT_PACKET_MAX_SIZE                     50000000
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE                  250
#define P2P_MAX_PEERS_IN_HANDSHAKE                      250
#define P2P_DEFAULT_CONNECTION_TIMEOUT                  5000
#define P2P_DEFAULT_SOCKS_CONNECT_TIMEOUT               45
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT             2000
#define P2P_DEFAULT_INVOKE_TIMEOUT                      60*2*1000
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT            5000
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT       70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT            2
#define P2P_DEFAULT_SYNC_SEARCH_CONNECTIONS_COUNT       2
#define P2P_DEFAULT_LIMIT_RATE_UP                       8192
#define P2P_DEFAULT_LIMIT_RATE_DOWN                     32768

#define P2P_FAILED_ADDR_FORGET_SECONDS                  (60*60)
#define P2P_IP_BLOCKTIME                                (60*60*24)
#define P2P_IP_FAILS_BEFORE_BLOCK                       10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL               (5*60)

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS                  0x01
#define P2P_SUPPORT_FLAGS                               P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

#define RPC_IP_FAILS_BEFORE_BLOCK                       3

// ---------------------------------------------------------------------------
// VINDEX IDENTITY
// ---------------------------------------------------------------------------
#define CRYPTONOTE_NAME                         "bitvindex"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME      "data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME "lock.mdb"
#define P2P_NET_DATA_FILENAME                   "p2pstate.bin"
#define RPC_PAYMENTS_DATA_FILENAME              "rpcpayments.bin"
#define MINER_CONFIG_FILE_NAME                  "miner_conf.json"

#define THREAD_STACK_SIZE                       5 * 1024 * 1024

#define HF_VERSION_DYNAMIC_FEE                  4
#define HF_VERSION_MIN_MIXIN_4                  6
#define HF_VERSION_MIN_MIXIN_6                  7
#define HF_VERSION_CRYPTONIGHT_VARIANT_1        7
#define HF_VERSION_MIN_MIXIN_10                 8
#define HF_VERSION_MIN_MIXIN_15                 15
#define HF_VERSION_ENFORCE_RCT                  6
#define HF_VERSION_PER_BYTE_FEE                 8
#define HF_VERSION_SMALLER_BP                   10
#define HF_VERSION_LONG_TERM_BLOCK_WEIGHT       10
#define HF_VERSION_MIN_2_OUTPUTS                12
#define HF_VERSION_MIN_V2_COINBASE_TX           12
#define HF_VERSION_SAME_MIXIN                   12
#define HF_VERSION_REJECT_SIGS_IN_COINBASE      12
#define HF_VERSION_ENFORCE_MIN_AGE              12
#define HF_VERSION_EFFECTIVE_SHORT_TERM_MEDIAN_IN_PENALTY 12
#define HF_VERSION_EXACT_COINBASE               13
#define HF_VERSION_CLSAG                        13
#define HF_VERSION_DETERMINISTIC_UNLOCK_TIME    13
#define HF_VERSION_BULLETPROOF_PLUS             15
#define HF_VERSION_VIEW_TAGS                    15
#define HF_VERSION_2021_SCALING                 15

#define PER_KB_FEE_QUANTIZATION_DECIMALS        8
#define CRYPTONOTE_SCALING_2021_FEE_ROUNDING_PLACES 2

#define HASH_OF_HASHES_STEP                     512

#define DEFAULT_TXPOOL_MAX_WEIGHT               648000000ull

#define BULLETPROOF_MAX_OUTPUTS                 16
#define BULLETPROOF_PLUS_MAX_OUTPUTS            16

#define CRYPTONOTE_PRUNING_STRIPE_SIZE          4096
#define CRYPTONOTE_PRUNING_LOG_STRIPES          3
#define CRYPTONOTE_PRUNING_TIP_BLOCKS           5500

#define RPC_CREDITS_PER_HASH_SCALE ((float)(1<<24))

#define DNS_BLOCKLIST_LIFETIME (86400 * 8)

#define MAX_TX_EXTRA_SIZE                       1060

// New constants are intended to go here
namespace config
{
  uint64_t const DEFAULT_FEE_ATOMIC_VDX_PER_KB = 500;
  uint8_t const FEE_CALCULATION_MAX_RETRIES = 10;
  uint64_t const DEFAULT_DUST_THRESHOLD = ((uint64_t)2000000000);
  uint64_t const BASE_REWARD_CLAMP_THRESHOLD = ((uint64_t)100000000);

  // ---------------------------------------------------------------------------
  // VINDEX MAINNET — Address prefixes chosen so addresses start with 'V'
  // Base58 prefix 20 -> leading character 'V' for standard addresses
  // Use: https://xmr.llcoins.net/addresstests.html to verify
  // ---------------------------------------------------------------------------
  uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 20;            // 'V'
  uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 21; // 'Vi'
  uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 43;         // 'Vs'

  // ---------------------------------------------------------------------------
  // VINDEX PORTS — distinct from Monero (18080) to prevent accidental peering
  // ---------------------------------------------------------------------------
  uint16_t const P2P_DEFAULT_PORT = 28080;
  uint16_t const RPC_DEFAULT_PORT = 28081;
  uint16_t const ZMQ_RPC_DEFAULT_PORT = 28082;

  // ---------------------------------------------------------------------------
  // VINDEX NETWORK ID — unique 16 magic bytes, prevents peering with Monero
  // Generated: 2026-05-22. Do NOT reuse Monero's bytes.
  // ---------------------------------------------------------------------------
  boost::uuids::uuid const NETWORK_ID = { {
      0x56, 0x44, 0x58, 0x4e, 0x45, 0x54, 0x20, 0x4c,
      0x49, 0x42, 0x45, 0x52, 0x54, 0x41, 0x54, 0x49
    } }; // "VDXNET LIBERTATI" in ASCII

  // ---------------------------------------------------------------------------
  // GENESIS BLOCK — PASS 2 COMPLETE
  // Genesis TX locked from --print-genesis-tx output on 2026-05-22.
  // Do NOT change these values — changing them creates a new incompatible chain.
  // ---------------------------------------------------------------------------
  std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302df5d56da0c7d643ddd1ce61901c7bdc5fb1738bfe39fbe69c28a3a7032729c0f2101168d0c4ca86fb55a4cf6a36d31431be1c53a3bd7411bb24e8832410289fa6f3b";
  uint32_t const GENESIS_NONCE = 10000;

  // Hash domain separators
  const char HASH_KEY_BULLETPROOF_EXPONENT[] = "bulletproof";
  const char HASH_KEY_BULLETPROOF_PLUS_EXPONENT[] = "bulletproof_plus";
  const char HASH_KEY_BULLETPROOF_PLUS_TRANSCRIPT[] = "bulletproof_plus_transcript";
  const char HASH_KEY_RINGDB[] = "ringdsb";
  const char HASH_KEY_SUBADDRESS[] = "SubAddr";
  const unsigned char HASH_KEY_ENCRYPTED_PAYMENT_ID = 0x8d;
  const unsigned char HASH_KEY_WALLET = 0x8c;
  const unsigned char HASH_KEY_WALLET_CACHE = 0x8d;
  const unsigned char HASH_KEY_BACKGROUND_CACHE = 0x8e;
  const unsigned char HASH_KEY_BACKGROUND_KEYS_FILE = 0x8f;
  const unsigned char HASH_KEY_RPC_PAYMENT_NONCE = 0x58;
  const unsigned char HASH_KEY_MEMORY = 'k';
  const unsigned char HASH_KEY_MULTISIG[] = {'M', 'u', 'l', 't' , 'i', 's', 'i', 'g', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  const unsigned char HASH_KEY_MULTISIG_KEY_AGGREGATION[] = "Multisig_key_agg";
  const unsigned char HASH_KEY_CLSAG_ROUND_MULTISIG[] = "CLSAG_round_ms_merge_factor";
  const unsigned char HASH_KEY_TXPROOF_V2[] = "TXPROOF_V2";
  const unsigned char HASH_KEY_CLSAG_ROUND[] = "CLSAG_round";
  const unsigned char HASH_KEY_CLSAG_AGG_0[] = "CLSAG_agg_0";
  const unsigned char HASH_KEY_CLSAG_AGG_1[] = "CLSAG_agg_1";
  const char HASH_KEY_MESSAGE_SIGNING[] = "vindexMessageSignature";
  const unsigned char HASH_KEY_MM_SLOT = 'm';
  const constexpr char HASH_KEY_MULTISIG_TX_PRIVKEYS_SEED[] = "multisig_tx_privkeys_seed";
  const constexpr char HASH_KEY_MULTISIG_TX_PRIVKEYS[] = "multisig_tx_privkeys";
  const constexpr char HASH_KEY_TXHASH_AND_MIXRING[] = "txhash_and_mixring";

  const uint32_t MULTISIG_MAX_SIGNERS{16};

  namespace testnet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 53;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 54;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 63;
    uint16_t const P2P_DEFAULT_PORT = 38080;
    uint16_t const RPC_DEFAULT_PORT = 38081;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 38082;
    boost::uuids::uuid const NETWORK_ID = { {
        0x56, 0x44, 0x58, 0x4e, 0x45, 0x54, 0x20, 0x4c,
        0x49, 0x42, 0x45, 0x52, 0x54, 0x41, 0x54, 0x4a
      } }; // "VDXNET LIBERTATJ"
    std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302df5d56da0c7d643ddd1ce61901c7bdc5fb1738bfe39fbe69c28a3a7032729c0f2101168d0c4ca86fb55a4cf6a36d31431be1c53a3bd7411bb24e8832410289fa6f3b";
    uint32_t const GENESIS_NONCE = 10000;
  }

  namespace stagenet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 24;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 25;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 36;
    uint16_t const P2P_DEFAULT_PORT = 48080;
    uint16_t const RPC_DEFAULT_PORT = 48081;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 48082;
    boost::uuids::uuid const NETWORK_ID = { {
        0x56, 0x44, 0x58, 0x4e, 0x45, 0x54, 0x20, 0x4c,
        0x49, 0x42, 0x45, 0x52, 0x54, 0x41, 0x54, 0x4b
      } }; // "VDXNET LIBERTATK"
    std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302df5d56da0c7d643ddd1ce61901c7bdc5fb1738bfe39fbe69c28a3a7032729c0f2101168d0c4ca86fb55a4cf6a36d31431be1c53a3bd7411bb24e8832410289fa6f3b";
    uint32_t const GENESIS_NONCE = 10000;
  }
}

namespace cryptonote
{
  enum network_type : uint8_t
  {
    MAINNET = 0,
    TESTNET,
    STAGENET,
    FAKECHAIN,
    UNDEFINED = 255
  };
  struct config_t
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX;
    uint16_t const P2P_DEFAULT_PORT;
    uint16_t const RPC_DEFAULT_PORT;
    uint16_t const ZMQ_RPC_DEFAULT_PORT;
    boost::uuids::uuid const NETWORK_ID;
    std::string const GENESIS_TX;
    uint32_t const GENESIS_NONCE;
  };
  inline const config_t& get_config(network_type nettype)
  {
    static const config_t mainnet = {
      ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::P2P_DEFAULT_PORT,
      ::config::RPC_DEFAULT_PORT,
      ::config::ZMQ_RPC_DEFAULT_PORT,
      ::config::NETWORK_ID,
      ::config::GENESIS_TX,
      ::config::GENESIS_NONCE
    };
    static const config_t testnet = {
      ::config::testnet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::testnet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::testnet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::testnet::P2P_DEFAULT_PORT,
      ::config::testnet::RPC_DEFAULT_PORT,
      ::config::testnet::ZMQ_RPC_DEFAULT_PORT,
      ::config::testnet::NETWORK_ID,
      ::config::testnet::GENESIS_TX,
      ::config::testnet::GENESIS_NONCE
    };
    static const config_t stagenet = {
      ::config::stagenet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::stagenet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::stagenet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::stagenet::P2P_DEFAULT_PORT,
      ::config::stagenet::RPC_DEFAULT_PORT,
      ::config::stagenet::ZMQ_RPC_DEFAULT_PORT,
      ::config::stagenet::NETWORK_ID,
      ::config::stagenet::GENESIS_TX,
      ::config::stagenet::GENESIS_NONCE
    };
    switch (nettype)
    {
      case MAINNET: return mainnet;
      case TESTNET: return testnet;
      case STAGENET: return stagenet;
      case FAKECHAIN: return mainnet;
      default: throw std::runtime_error("Invalid network type");
    }
  };
}
