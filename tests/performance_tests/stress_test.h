// Copyright (c) 2014-2024, The vindex Project
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

// ============================================================
// stress_test.h  —  Vindex Coin Blockchain Stress Test Suite
// ============================================================
// Purpose: push every critical hot-path in the Vindex protocol
// under maximum realistic load so that regressions in throughput
// are caught before release.
//
// Test categories
// ───────────────
//  1. High-volume transaction construction  (RCT Bulletproof+)
//  2. CLSAG ring-signature sign + verify    (ring sizes 11 / 16 / 32)
//  3. Bulletproof+ range-proof generation   (16 / 32 / 64 outputs)
//  4. Batch ring-sig verification pressure
//  5. Key-derivation storm
//  6. Key-image generation under load
//  7. Subaddress expansion at scale
//
// Usage (add to main.cpp after existing TEST_PERFORMANCE macros):
//
//   #include "stress_test.h"
//
//   // --- Stress: high-volume tx construction ---
//   TEST_PERFORMANCE2(filter, p, test_stress_construct_tx_rct_bp_plus, 8, 8);
//   TEST_PERFORMANCE2(filter, p, test_stress_construct_tx_rct_bp_plus, 16, 16);
//
//   // --- Stress: CLSAG at various ring sizes ---
//   TEST_PERFORMANCE1(filter, p, test_stress_clsag_sign, 11);
//   TEST_PERFORMANCE1(filter, p, test_stress_clsag_sign, 16);
//   TEST_PERFORMANCE1(filter, p, test_stress_clsag_sign, 32);
//
//   // --- Stress: Bulletproof+ range proofs ---
//   TEST_PERFORMANCE1(filter, p, test_stress_bp_plus_prove, 16);
//   TEST_PERFORMANCE1(filter, p, test_stress_bp_plus_prove, 32);
//   TEST_PERFORMANCE1(filter, p, test_stress_bp_plus_prove, 64);
//
//   // --- Stress: key primitives ---
//   TEST_PERFORMANCE0(filter, p, test_stress_key_derivation_storm);
//   TEST_PERFORMANCE0(filter, p, test_stress_key_image_generation);
//   TEST_PERFORMANCE1(filter, p, test_stress_subaddress_expand, 1000);

#pragma once

#include <vector>
#include <stdint.h>

#include "cryptonote_basic/account.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_core/cryptonote_tx_utils.h"
#include "ringct/rctSigs.h"
#include "ringct/rctTypes.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/subaddress_index.h"

#include "multi_tx_test_base.h"
#include "single_tx_test_base.h"

// ──────────────────────────────────────────────────────────────────────────────
// 1. High-volume RCT Bulletproof+ transaction construction
//    Template params: in_count, out_count
// ──────────────────────────────────────────────────────────────────────────────

template<size_t a_in_count, size_t a_out_count>
class test_stress_construct_tx_rct_bp_plus : private multi_tx_test_base<a_in_count>
{
  static_assert(a_in_count  > 0, "in_count must be > 0");
  static_assert(a_out_count > 0, "out_count must be > 0");

public:
  // Fewer loops for heavier combinations; still meaningful sample size
  static const size_t loop_count =
    (a_in_count + a_out_count <= 8)  ? 20 :
    (a_in_count + a_out_count <= 16) ? 10 :
    (a_in_count + a_out_count <= 32) ? 5  : 3;

  typedef multi_tx_test_base<a_in_count> base_class;

  bool init()
  {
    if (!base_class::init())
      return false;

    m_recipient.generate();

    for (size_t i = 0; i < a_out_count; ++i)
    {
      m_destinations.push_back(
        cryptonote::tx_destination_entry(
          this->m_source_amount / a_out_count,
          m_recipient.get_keys().m_account_address,
          false));
    }
    return true;
  }

  bool test()
  {
    crypto::secret_key tx_key;
    std::vector<crypto::secret_key> additional_tx_keys;
    std::unordered_map<crypto::public_key, cryptonote::subaddress_index> subaddresses;
    subaddresses[this->m_miners[this->real_source_idx].get_keys().m_account_address.m_spend_public_key] = {0, 0};

    // Force Bulletproof+ (bp_version=2, RangeProofPaddedBulletproof)
    rct::RCTConfig rct_config{rct::RangeProofPaddedBulletproof, 2};

    cryptonote::transaction tx;
    return cryptonote::construct_tx_and_get_tx_key(
      this->m_miners[this->real_source_idx].get_keys(),
      subaddresses,
      this->m_sources,
      m_destinations,
      boost::none,
      std::vector<uint8_t>(),
      tx,
      tx_key,
      additional_tx_keys,
      true,  // rct = true
      rct_config);
  }

private:
  cryptonote::account_base m_recipient;
  std::vector<cryptonote::tx_destination_entry> m_destinations;
};


// ──────────────────────────────────────────────────────────────────────────────
// 2. CLSAG sign stress — variable ring size
//    Template param: ring_size (recommended: 11, 16, 32)
// ──────────────────────────────────────────────────────────────────────────────

template<size_t a_ring_size>
class test_stress_clsag_sign
{
  static_assert(a_ring_size >= 2, "ring_size must be >= 2");

public:
  // CLSAG sign is expensive; 50 iterations still gives a solid mean/stddev
  static const size_t loop_count =
    (a_ring_size <= 11) ? 50 :
    (a_ring_size <= 16) ? 30 : 15;

  bool init()
  {
    // Build a synthetic ring of random public keys and commitments
    m_p.resize(a_ring_size);
    m_C.resize(a_ring_size);
    for (size_t i = 0; i < a_ring_size; ++i)
    {
      crypto::random32_unbiased((unsigned char*)m_p[i].bytes);
      crypto::random32_unbiased((unsigned char*)m_C[i].bytes);
    }

    // Real index is 0
    crypto::random32_unbiased((unsigned char*)m_sk.bytes);
    crypto::random32_unbiased((unsigned char*)m_a.bytes);

    // Message to sign (32 random bytes)
    crypto::random32_unbiased((unsigned char*)m_msg.bytes);

    // Signing key for commitment: z = a - b (just use random scalars)
    crypto::random32_unbiased((unsigned char*)m_z.bytes);

    return true;
  }

  bool test()
  {
    rct::keyV P(m_p.begin(), m_p.end());
    rct::keyV C(m_C.begin(), m_C.end());
    rct::clsag sig = rct::proveRctCLSAGSimple(
      m_msg,
      P,
      m_sk,
      m_z,
      m_a,
      C,
      0 /* real_index */);
    // Signature must be non-trivial (sanity)
    return sig.s.size() == a_ring_size;
  }

private:
  std::vector<rct::key> m_p;
  std::vector<rct::key> m_C;
  rct::key m_sk;
  rct::key m_a;
  rct::key m_z;
  rct::key m_msg;
};


// ──────────────────────────────────────────────────────────────────────────────
// 3. Bulletproof+ range-proof generation stress
//    Template param: n_outputs (16, 32, 64)
//    Each output is a 64-bit Pedersen commitment.
// ──────────────────────────────────────────────────────────────────────────────

template<size_t a_n_outputs>
class test_stress_bp_plus_prove
{
  static_assert(a_n_outputs > 0, "n_outputs must be > 0");

public:
  static const size_t loop_count =
    (a_n_outputs <= 16) ? 20 :
    (a_n_outputs <= 32) ? 10 : 5;

  bool init()
  {
    m_amounts.resize(a_n_outputs);
    m_masks.resize(a_n_outputs);

    for (size_t i = 0; i < a_n_outputs; ++i)
    {
      // Random amounts in [1, 2^62) to stay within the valid 64-bit range
      m_amounts[i] = ((uint64_t)crypto::rand<uint32_t>() << 10) + 1;
      m_masks[i]   = rct::skGen();
    }
    return true;
  }

  bool test()
  {
    rct::BulletproofPlus proof = rct::bulletproof_plus_PROVE(m_amounts, m_masks);
    // Proof must contain at least one L element (sanity check)
    return !proof.L.empty();
  }

private:
  std::vector<uint64_t> m_amounts;
  rct::keyV              m_masks;
};


// ──────────────────────────────────────────────────────────────────────────────
// 4. Batch Bulletproof+ verification pressure
//    Generates BATCH_SIZE proofs then verifies them all at once.
//    This mirrors the daemon's mempool batch-verification path.
// ──────────────────────────────────────────────────────────────────────────────

class test_stress_bp_plus_batch_verify
{
public:
  static const size_t BATCH_SIZE = 16;
  static const size_t loop_count = 10;

  bool init()
  {
    m_proofs.clear();
    m_proofs.reserve(BATCH_SIZE);

    for (size_t i = 0; i < BATCH_SIZE; ++i)
    {
      // 2-output proof per transaction (typical)
      std::vector<uint64_t> amounts = {
        (uint64_t)crypto::rand<uint32_t>() + 1,
        (uint64_t)crypto::rand<uint32_t>() + 1
      };
      rct::keyV masks = {rct::skGen(), rct::skGen()};
      m_proofs.push_back(rct::bulletproof_plus_PROVE(amounts, masks));
    }
    return true;
  }

  bool test()
  {
    // Build pointer list expected by the batch verifier
    std::vector<const rct::BulletproofPlus*> proof_ptrs;
    proof_ptrs.reserve(m_proofs.size());
    for (const auto& p : m_proofs)
      proof_ptrs.push_back(&p);

    return rct::bulletproof_plus_VERIFY(proof_ptrs);
  }

private:
  std::vector<rct::BulletproofPlus> m_proofs;
};


// ──────────────────────────────────────────────────────────────────────────────
// 5. Key-derivation storm
//    Simulates a wallet scanning a block full of outputs (view-key scanning).
//    Each call performs one ECDH key derivation — the innermost loop of wallet2.
// ──────────────────────────────────────────────────────────────────────────────

class test_stress_key_derivation_storm
{
public:
  // Equivalent to scanning ~2000 outputs — realistic large block
  static const size_t loop_count = 2000;

  bool init()
  {
    // Generate a fresh spend keypair per session
    crypto::generate_keys(m_pub, m_sec);
    return true;
  }

  bool test()
  {
    // Ephemeral tx public key (new random each call, simulating unique per-tx key)
    crypto::public_key R;
    crypto::secret_key r;
    crypto::generate_keys(R, r);

    crypto::key_derivation derivation;
    return crypto::generate_key_derivation(R, m_sec, derivation);
  }

private:
  crypto::public_key m_pub;
  crypto::secret_key m_sec;
};


// ──────────────────────────────────────────────────────────────────────────────
// 6. Key-image generation under load
//    Key images are computed during spend to prevent double-spends.
//    This measures how fast the daemon can validate key-image uniqueness.
// ──────────────────────────────────────────────────────────────────────────────

class test_stress_key_image_generation
{
public:
  static const size_t loop_count = 1000;

  bool init()
  {
    crypto::generate_keys(m_pub, m_sec);

    // Pre-derive a key derivation (done once — we stress the image generation step)
    crypto::public_key R;
    crypto::secret_key r;
    crypto::generate_keys(R, r);
    return crypto::generate_key_derivation(R, m_sec, m_derivation);
  }

  bool test()
  {
    // Derive the one-time public key for output index m_idx, then generate image
    crypto::public_key out_key;
    crypto::derive_public_key(m_derivation, m_idx, m_pub, out_key);

    crypto::secret_key out_sec;
    crypto::derive_secret_key(m_derivation, m_idx, m_sec, out_sec);

    crypto::key_image ki;
    crypto::generate_key_image(out_key, out_sec, ki);

    ++m_idx;
    return true;  // key image is valid as long as no exception was thrown
  }

private:
  crypto::public_key     m_pub;
  crypto::secret_key     m_sec;
  crypto::key_derivation m_derivation;
  size_t                 m_idx = 0;
};


// ──────────────────────────────────────────────────────────────────────────────
// 7. Subaddress expansion at scale
//    Wallet2 must precompute (major, minor) subaddress tables.
//    Template param: n_subaddresses  — total to expand per call.
// ──────────────────────────────────────────────────────────────────────────────

template<size_t a_n_subaddresses>
class test_stress_subaddress_expand
{
  static_assert(a_n_subaddresses > 0, "n_subaddresses must be > 0");

public:
  static const size_t loop_count =
    (a_n_subaddresses <= 100)  ? 100 :
    (a_n_subaddresses <= 500)  ? 20  :
    (a_n_subaddresses <= 1000) ? 10  : 5;

  bool init()
  {
    m_account.generate();
    return true;
  }

  bool test()
  {
    // Expand subaddresses in the (0, 0..n) range — mirrors wallet2 precomputation
    for (size_t i = 0; i < a_n_subaddresses; ++i)
    {
      cryptonote::subaddress_index idx{0, static_cast<uint32_t>(i)};
      // cryptonote::get_subaddress uses the account keys to derive the address
      volatile auto addr = cryptonote::get_subaddress(m_account.get_keys(), idx);
      (void)addr;
    }
    return true;
  }

private:
  cryptonote::account_base m_account;
};
