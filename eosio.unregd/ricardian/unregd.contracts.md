<h1 class="contract"> chngaddress </h1>
### Intent

The intent of the `{{ chngaddress }}` action is to update the the stored information by updating the old Ethereum address {{old_address}} to a new Ethereum address {{new_address}}.

As an authorized party I {{ signer }} wish to modify the Ethereum address {{old_address}} to a new Ethereum address {{new_address}}. 

<h1 class="contract"> regaccount2 </h1>
### Intent

The intent of the `regaccount` action is to create an EOS account using the stored information {{ Ethereum address }} and token balance from the `eosio.unregd` contract, after verifying the submitted Ethereum {{ signature }}. This is intended to be used only once for each Ethereum address stored in the `eosio.unregd` contract.

As an authorized party, I {{ signer }} wish to create an account {{ account }} on the EOS chain with ID: aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906, accessible with EOS public key {{ eos_pubkey_str }} by submitting cryptographic proof {{ signature }} corresponding to the {{ Ethereum address }}.

As signer, I stipulate that if I am not the beneficial owner of these tokens, I have been authorized to take this action by the party submitting the cryptographic proof {{signature}}.

In case of dispute, all cases should be brought to the EOS Core Arbitration Forum at https://eoscorearbitration.io/.

<h1 class="contract"> regaccount </h1>
### Intent

The intent of the `regaccount` action is to create an EOS account using the stored information {{ Ethereum address }} and token balance from the `eosio.unregd` contract, after verifying the submitted Ethereum {{ signature }}. This is intended to be used only once for each Ethereum address stored in the `eosio.unregd` contract.

As an authorized party, I {{ signer }} wish to create an account {{ account }} on the EOS chain with ID: aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906, accessible with EOS public key {{ eos_pubkey_str }} by submitting cryptographic proof {{ signature }} corresponding to the {{ Ethereum address }}.

As signer, I stipulate that if I am not the beneficial owner of these tokens, I have been authorized to take this action by the party submitting the cryptographic proof {{signature}}.

In case of dispute, all cases should be brought to the EOS Core Arbitration Forum at https://eoscorearbitration.io/.

<h1 class="contract"> setmaxeos </h1>
### Intent

The intent of the `{{ setmaxeos }}` action is to set the maximum amount of EOS this contract is willing to pay when creating a new account.

As an authorized party I {{ signer }} wish to set the new maximum amount as {{ maxeos }}.
