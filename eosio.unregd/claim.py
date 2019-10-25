import os
import sys
import json
import time
import struct
import requests as req
from eth_utils import decode_hex
from hashlib import sha256
from bitcoin import ecdsa_raw_sign, encode_privkey
from tempfile import mktemp
from subprocess import Popen, PIPE
from getpass import getpass
from Crypto.Hash import keccak

API_URL = os.environ.get("API_URL", "http://127.0.0.1:8888")

def keccak_256(x): return keccak.new(digest_bits=256, data=x)

def url_for(url):
  return '{0}{1}'.format(API_URL, url)

def endian_reverse_u32(x):
 x = x & 0xFFFFFFFF
 return (((x >> 0x18) & 0xFF)        )\
      | (((x >> 0x10) & 0xFF) << 0x08)\
      | (((x >> 0x08) & 0xFF) << 0x10)\
      | (((x        ) & 0xFF) << 0x18)

def is_canonical( sig ):
  return not (sig[1] & 0x80)\
     and not (sig[1] == 0 and not (sig[2] & 0x80))\
     and not (sig[33] & 0x80)\
     and not (sig[33] == 0 and not (sig[34] & 0x80))

def get_tapos_info(block_id):
  block_id_bin = decode_hex(block_id)
  
  hash0 = struct.unpack("<Q", block_id_bin[0:8])[0]
  hash1 = struct.unpack("<Q", block_id_bin[8:16])[0]

  ref_block_num  = endian_reverse_u32(hash0) & 0xFFFF
  ref_block_prefix = hash1 & 0xFFFFFFFF
  
  return ref_block_num, ref_block_prefix

if len(sys.argv) < 3:
  print ("claim.py EOSACCOUNT EOSPUBKEY SIGNTXPRIV [ETHPRIV]")
  print ("    EOSACCOUNT: Desired EOS account name")
  print ("    EOSPUBKEY: Desired EOS pubkey")
  print ("    PUSHER: account@permission used to sign and push the claim transaction")
  print ("    NONCE: Ethereum nonce if claiming tokens controlled by contract (will call regaccount2)")
  sys.exit(1)

eos_account  = sys.argv[1]
eos_pub      = sys.argv[2]
pusher       = sys.argv[3]
nonce        = int(sys.argv[4])
priv         = getpass("Enter ETH private key (Wif or Hex format)")


while True:
  
  block_id = req.get(url_for('/v1/chain/get_info')).json()['last_irreversible_block_id']
  ref_block_num, ref_block_prefix = get_tapos_info(block_id)
  
  msg = '%d,%d,%s,%s' % (ref_block_num, ref_block_prefix, eos_pub, eos_account)
  msg = '%s%s%d%s' % ("\x19", "Ethereum Signed Message:\n", len(msg), msg)
  msghash = keccak_256(msg.encode('utf-8')).digest()
  
  v, r, s = ecdsa_raw_sign(msghash, decode_hex(encode_privkey(priv,'hex')) )
  signature = '00%02x%064x%064x' % (v,r,s)
  
  if is_canonical(decode_hex(signature)):
    break

  time.sleep(1)

tempf = mktemp()
with open(tempf,'w') as fp:
  params = {
    'signature'  : signature,
    'account'    : eos_account,
    'eos_pubkey' : eos_pub
  }
  action_to_call = "regaccount"
  
  if nonce >= 0:
    params['nonce'] = nonce
    action_to_call = "regaccount2"

  fp.write(json.dumps(params))

with open(os.devnull, 'w') as devnull:
  cmd = ["cleos","-u", API_URL, "push", "action", "-r", block_id, "eosio.unregd", action_to_call, tempf, "-p", pusher]
  p = Popen(cmd)
  output, err = p.communicate("")

if p.returncode:
  print ("Error sending tx")
  sys.exit(1)

print ("tx sent")
sys.exit(0)