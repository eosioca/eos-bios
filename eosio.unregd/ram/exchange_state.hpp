#pragma once

#include <eosiolib/asset.hpp>

namespace eosiosystem {
   using eosio::asset;
   using eosio::symbol;
   using eosio::symbol_code;
   using eosio::name;

   typedef double real_type;

   /**
    *  Uses Bancor math to create a 50/50 relay between two asset types. The state of the
    *  bancor exchange is entirely contained within this struct. There are no external
    *  side effects associated with using this API.
    */
   struct exchange_state {
      asset    supply;

      struct connector {
         asset balance;
         double weight = .5;

         EOSLIB_SERIALIZE( connector, (balance)(weight) )
      };

      connector base;
      connector quote;

      uint64_t primary_key()const { return supply.symbol.raw(); }

      asset convert_to_exchange( connector& c, asset in );
      asset convert_from_exchange( connector& c, asset in );
      asset convert( asset from, const symbol& to );

      EOSLIB_SERIALIZE( exchange_state, (supply)(base)(quote) )
   };

   static constexpr symbol ramcore_symbol = symbol(symbol_code("RAMCORE"), 4);
   static constexpr symbol ram_symbol     = symbol(symbol_code("RAM"), 0);

   typedef eosio::multi_index< "rammarket"_n, exchange_state > rammarket;

   static symbol get_core_symbol( const rammarket& rm ) {
      auto itr = rm.find(ramcore_symbol.raw());
      eosio_assert(itr != rm.end(), "system contract must first be initialized");
      return itr->quote.balance.symbol;
   }

   static symbol get_core_symbol( name system_account = "eosio"_n ) {
      rammarket rm(system_account, system_account.value);
      const static auto sym = get_core_symbol( rm );
      return sym;
   }


} /// namespace eosiosystem
