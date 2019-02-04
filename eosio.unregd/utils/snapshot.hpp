#pragma once

namespace eosio {

   asset buyrambytes( uint32_t bytes ) {
      eosiosystem::rammarket market("eosio"_n, "eosio"_n.value);
      auto itr = market.find(eosiosystem::ramcore_symbol.raw());
      eosio_assert(itr != market.end(), "RAMCORE market not found");
      auto tmp = *itr;
      return tmp.convert( asset(bytes,eosiosystem::ram_symbol), itr->quote.balance.symbol );
   }

   vector<asset> split_snapshot(const asset& balance) {
      
      auto core_symbol = eosiosystem::get_core_symbol();

      if ( balance < asset(5000, core_symbol) ) {
         return {};
      }

      // everyone has minimum 0.25 EOS staked
      // some 10 EOS unstaked
      // the rest split between the two

      auto cpu = asset(2500, core_symbol);
      auto net = asset(2500, core_symbol);

      auto remainder = balance - cpu - net;

      if ( remainder <= asset(100000, core_symbol) ) /* 10.0 EOS */ {
         return {net, cpu, remainder};
      }

      remainder -= asset(100000, core_symbol); // keep them floating, unstaked

      auto first_half = remainder / 2;
      cpu += first_half;
      net += remainder - first_half;

      return {net, cpu, asset(100000, core_symbol)};
   }


   vector<asset> split_snapshot_abp(const asset& balance) {
      
      auto core_symbol = eosiosystem::get_core_symbol();

      eosio_assert( balance >= asset(1000, core_symbol), "insuficient balance" );

      asset floatingAmount;

      if (balance > asset(110000, core_symbol)) { 
         floatingAmount = asset(100000, core_symbol);
      } else if (balance > asset(30000, core_symbol)) { 
         floatingAmount = asset(20000, core_symbol); 
      } else { 
         floatingAmount = asset(1000, core_symbol);
      }

      asset to_split  = balance - floatingAmount;
      
      asset split_cpu = to_split/2; 
      asset split_net = to_split - split_cpu;

      return {split_net, split_cpu, floatingAmount};
   }


}