#pragma once

#include "authority.hpp"

namespace eosio {

   // Helper struct for inline calls.
   // Only the prototype of the functions are used for 
   // the serialization of the action's payload.
   struct call {
      struct token {
         void issue( name to, asset quantity, string memo );
         void transfer( name from,
                        name to,
                        asset        quantity,
                        string       memo );
      };

      struct eosio {
         void newaccount(name creator, name name, 
                           authority owner, authority active);
         void delegatebw( name from, name receiver,
                          asset stake_net_quantity, asset stake_cpu_quantity, bool transfer );
         void buyram( name buyer, name receiver, asset tokens );
      };
   };   
}
