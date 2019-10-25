#include <functional>
#include <string>
#include <cmath>

#include <eosiolib/transaction.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/fixed_key.hpp>
#include <eosiolib/public_key.hpp>

#include "ram/exchange_state.cpp"

#define USE_KECCAK
#include "sha3/byte_order.c"
#include "sha3/sha3.c"

#include "abieos_numeric.hpp"
#define uECC_SUPPORTS_secp160r1 0
#define uECC_SUPPORTS_secp192r1 0
#define uECC_SUPPORTS_secp224r1 0
#define uECC_SUPPORTS_secp256r1 0
#define uECC_SUPPORTS_secp256k1 1
#define uECC_SUPPORT_COMPRESSED_POINT 1
#include "ecc/uECC.c"

using namespace eosio;
using namespace std;

#include "utils/inline_calls_helper.hpp"
#include "utils/snapshot.hpp"

// Typedefs
typedef std::string ethereum_address;
typedef vector<char> bytes;
typedef fixed_bytes<32> key_256;

// Namespaces
using eosio::const_mem_fun;
using eosio::indexed_by;
using std::function;
using std::string;

namespace eosio {

CONTRACT unregd : public contract {
 public:
  unregd(name receiver, name code, datastream<const char*> ds ): eosio::contract(receiver, code, ds),
      addresses(_self, _self.value),
      settings(_self, _self.value) {}

  // Actions
  ACTION add(const ethereum_address& ethereum_address, const asset& balance);
  ACTION regaccount(const bytes& signature, const string& account, const string& eos_pubkey);
  ACTION regaccount2(uint32_t nonce, const bytes& signature, const string& account, const string& eos_pubkey);
  ACTION setmaxeos(const asset& maxeos);
  ACTION chngaddress(const ethereum_address& old_address, const ethereum_address& new_address);

 private:
  void regaccount_impl(const bytes& signature, const string& account, const string& eos_pubkey_str, int nonce);

  static uint8_t hex_char_to_uint(char character) {
    const int x = character;

    return (x <= 57) ? x - 48 : (x <= 70) ? (x - 65) + 0x0a : (x - 97) + 0x0a;
  }

  static key_256 compute_ethereum_address_key256(const ethereum_address& ethereum_address) {
    uint8_t ethereum_key[20];
    const char* characters = ethereum_address.c_str();

    // The ethereum address starts with 0x, let's skip those by starting at i = 2
    for (uint64_t i = 2; i < ethereum_address.length(); i += 2) {
      const uint64_t index = (i / 2) - 1;

      ethereum_key[index] = 16 * hex_char_to_uint(characters[i]) + hex_char_to_uint(characters[i + 1]);
    }

    const uint32_t* p32 = reinterpret_cast<const uint32_t*>(&ethereum_key);
    return key_256::make_from_word_sequence<uint32_t>(p32[0], p32[1], p32[2], p32[3], p32[4]);
  }

  static key_256 compute_ethereum_address_key256(uint8_t* ethereum_key) {
    const uint32_t* p32 = reinterpret_cast<const uint32_t*>(ethereum_key);
    return key_256::make_from_word_sequence<uint32_t>(p32[0], p32[1], p32[2], p32[3], p32[4]);
  }

  TABLE address {
    uint64_t id;
    ethereum_address ethereum_address;
    asset balance;

    uint64_t primary_key() const { return id; }
    key_256 by_ethereum_address() const { return unregd::compute_ethereum_address_key256(ethereum_address); }

    EOSLIB_SERIALIZE(address, (id)(ethereum_address)(balance))
  };

  typedef eosio::multi_index<
      "addresses"_n, address,
      indexed_by<"ethaddress"_n, const_mem_fun<address, key_256, &address::by_ethereum_address>>>
      addresses_index;

  
  TABLE settings {
    uint64_t id;
    asset    max_eos_for_8k_of_ram;

    uint64_t primary_key() const { return id; }
    EOSLIB_SERIALIZE(settings, (id)(max_eos_for_8k_of_ram))
  };

  typedef eosio::multi_index<"settings"_n, settings> settings_index;

  void update_address(const ethereum_address& ethereum_address, const function<void(address&)> updater);

  addresses_index addresses;
  settings_index settings;
};

}  // namespace eosio
