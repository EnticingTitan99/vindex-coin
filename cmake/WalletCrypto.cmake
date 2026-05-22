# Copyright (c) 2020-2024, The Vindex Project
# Distributed under the MIT/X11 software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# WalletCrypto.cmake
# Provides helper macros for selecting and wiring the wallet crypto backend.
#
# Macros defined here:
#   vindex_crypto_autodetect(AVAILABLE <var> BEST <var>)
#     Probes available wallet crypto backends and sets AVAILABLE to a list
#     of supported backends and BEST to the recommended one.  If nothing
#     better than the built-in CN implementation is found both output
#     variables are left undefined so callers can fall back to "cn".
#
#   vindex_crypto_generate_header(<backend> <output_header_path>)
#     Generates the ops.h shim for the given backend into output_header_path.
#
#   vindex_crypto_get_target(<backend> <out_var>)
#     Sets <out_var> to the CMake target name that provides the requested
#     backend.

# ---------------------------------------------------------------------------
# Internal helper: list every backend whose prerequisites are satisfied.
# Currently recognised backends (in preference order):
#   cn   - built-in CryptoNote/Monero scalar implementation (always available)
# Future backends (e.g. aesni, sodium) can be appended here.
# ---------------------------------------------------------------------------
function(_vindex_wallet_crypto_probe out_available)
  set(_avail "")

  # "cn" is always available - it is compiled from source in src/crypto/
  list(APPEND _avail "cn")

  set(${out_available} "${_avail}" PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------------------
# vindex_crypto_autodetect
# Usage:
#   vindex_crypto_autodetect(AVAILABLE avail_var BEST best_var)
# ---------------------------------------------------------------------------
macro(vindex_crypto_autodetect)
  # Parse keyword arguments: AVAILABLE <var> BEST <var>
  set(_vca_opts "")
  set(_vca_onevals AVAILABLE BEST)
  set(_vca_multvals "")
  cmake_parse_arguments(_VCA "${_vca_opts}" "${_vca_onevals}" "${_vca_multvals}" ${ARGN})

  _vindex_wallet_crypto_probe(_vca_avail_list)

  if(_VCA_AVAILABLE)
    set(${_VCA_AVAILABLE} "${_vca_avail_list}" PARENT_SCOPE)
  endif()

  # Pick the best backend: anything other than "cn" is preferred.
  # With only "cn" available we leave BEST undefined so callers fall
  # back to the internal implementation without printing a spurious message.
  list(REMOVE_ITEM _vca_avail_list "cn")
  if(_vca_avail_list AND _VCA_BEST)
    list(GET _vca_avail_list 0 _vca_best_choice)
    set(${_VCA_BEST} "${_vca_best_choice}" PARENT_SCOPE)
  endif()
endmacro()

# ---------------------------------------------------------------------------
# vindex_crypto_generate_header(<backend> <output_path>)
# Writes the ops.h shim that exposes the chosen backend's scalar multiply
# functions under a stable API expected by src/crypto/wallet/crypto.h.
# ---------------------------------------------------------------------------
function(vindex_crypto_generate_header backend output_path)
  if(backend STREQUAL "cn")
    # "cn" should never reach this path - callers use the ALIAS route instead.
    configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/src/crypto/wallet/empty.h.in"
      "${output_path}"
      @ONLY
    )
  else()
    message(FATAL_ERROR
      "vindex_crypto_generate_header: unknown wallet crypto backend '${backend}'.\n"
      "Supported values: cn"
    )
  endif()
endfunction()

# ---------------------------------------------------------------------------
# vindex_crypto_get_target(<backend> <out_var>)
# Returns the CMake target name that provides the compiled objects for the
# requested backend.
# ---------------------------------------------------------------------------
function(vindex_crypto_get_target backend out_var)
  if(backend STREQUAL "cn")
    set(${out_var} "cncrypto" PARENT_SCOPE)
  else()
    message(FATAL_ERROR
      "vindex_crypto_get_target: unknown wallet crypto backend '${backend}'.\n"
      "Supported values: cn"
    )
  endif()
endfunction()
