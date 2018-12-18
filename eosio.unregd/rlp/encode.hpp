size_t rlp_encode(const uint8_t* addy, uint32_t nonce, uint8_t* buffer, size_t len) {
   
   eosio_assert(len >= 27, "buffer too small");

   //List: 0xc0 (empty)
   buffer[0] = 0xc0;

   //List item: eth address (0x80 | len = 20)
   buffer[1] = 0x80 + 20;
   memcpy(buffer+2, addy, 20);

   const int nonce_index = 22;
   uint32_t nonce_len = 0;

   //List item: nonce
   if( nonce < 128 ) {
      buffer[nonce_index] = (uint8_t)nonce;
      nonce_len = 1;
      if( nonce == 0 ) buffer[nonce_index] |= 0x80;
   } else {
      nonce = __builtin_bswap32(nonce);
      auto* p = (uint8_t*)&nonce;
      for(int i=0; i<4; i++) {
         if(*p || nonce_len > 0) {
            buffer[nonce_index + ++nonce_len] = *p;
         }
         p++;
      }
      buffer[nonce_index] = 0x80 | nonce_len;
      ++nonce_len;
   }

   //Add total length
   uint8_t total_len = 1 + 20 + nonce_len;
   buffer[0] += total_len;
   return total_len+1;
}
