import sys
import rlp
from bitcoin import privtopub
from eth_utils import decode_hex, encode_hex, int_to_big_endian, big_endian_to_int

from Crypto.Hash import keccak
def keccak_256(x): return keccak.new(digest_bits=256, data=x)

#https://github.com/ethereum/pyethereum/blob/b704a5c6577863edc539a1ec3d2620a443b950fb/ethereum/utils.py#L119
from rlp.utils import ALL_BYTES

def is_numeric(x): return isinstance(x, int)

def ascii_chr(n):
    return ALL_BYTES[n]

def int_to_addr(x):
    o = [b''] * 20
    for i in range(20):
        o[19 - i] = ascii_chr(x & 0xff)
        x >>= 8
    return b''.join(o)

def normalize_address(x, allow_blank=False):
    if is_numeric(x):
        return int_to_addr(x)
    if allow_blank and x in {'', b''}:
        return b''
    if len(x) in (42, 50) and x[:2] in {'0x', b'0x'}:
        x = x[2:]
    if len(x) in (40, 48):
        x = decode_hex(x)
    if len(x) == 24:
        assert len(x) == 24 and keccak_256(x[:20]).digest()[:4] == x[-4:]
        x = x[:20]
    if len(x) != 20:
        raise Exception("Invalid address format: %r" % x)
    return x

def mk_contract_address(sender, nonce):
    return keccak_256(rlp.encode([normalize_address(sender), nonce])).digest()[12:]

priv = sys.argv[1]
nonce = -1
if len(sys.argv) > 2:
  nonce = int(sys.argv[2])

pub = privtopub(priv)
addy = encode_hex(keccak_256(decode_hex(pub[2:])).digest()[12:])

if nonce >= 0:
  addy = encode_hex(mk_contract_address(addy, nonce))

print (addy)