# Build (eosio.cdt >= v1.5.0)

```shell
eosio-cpp -o eosio.unregd.wasm eosio.unregd.cpp
eosio-abigen -contract=unregd -output=eosio.unregd.abi eosio.unregd.cpp -R=./ricardian
```

# Setup

```./setup.sh```

The setup script will install one contract (besides the defaults ones):
  
  `eosio.unregd` (empty)

You need to have nodeos running.

# Add test data

```./add_test_data.sh [nonce]```

# Claim

```shell
python claim.py eostest11125 EOS7jUtjvK61eWM38RyHS3WFM7q41pSYMP7cpjQWWjVaaxH5J9Cb7 thisisatesta@active [nonce]
```

# Dependecies

 ```shell
 pip3 install bitcoin --user
 pip3 install requests --user
 pip3 install eth-utils --user
 pip3 install pycryptodome --user
 pip3 install pyrlp --user
 ```
