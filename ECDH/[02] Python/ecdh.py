"""This Python module is an implementation of the ECDH algorithm.
From https://github.com/YongBhin-Kim/crypto/tree/master/ECDH"""

import random
from secp256k1 import curve, mul

# ECDH Verification algorithm
def ecdh_verify(ss_a, ss_b):
    
    if ss_a != ss_b:
        return False

    ab = (sk_a * sk_b) % curve.n            # (ab)
    abG = mul(ab, curve.G)                  # (ab)G
    
    if ss_a != abG:
        return False

    return True
    

if __name__ == "__main__":
    
    # Generate sk and pk of Alice and Bob
    sk_a = random.randrange(1, curve.n)     # sk_a \in [1, n]
    pk_a = mul(sk_a, curve.G)               # pk_a := sk_a * G

    sk_b = random.randrange(1, curve.n)     # sk_b \in [1, n]
    pk_b = mul(sk_b, curve.G)               # pk_b := sk_b * G

    # ECDH Secure Sharing
    ss_a = mul(sk_a, pk_b)                  # Alice side (ss_a := a(bG))
    ss_b = mul(sk_b, pk_a)                  # Bob side   (ss_b := b(aG))
    
    # ECDH Verification
    ret = ecdh_verify(ss_a, ss_b)
    
    # Debug
    print("\n(Alice) secret key:\t", sk_a)
    print(  "(Alice) public key:\t", pk_a)
    print("\n(Bob)   secret key:\t", sk_b)
    print(  "(Bob)   public key:\t", pk_b)
    print("\n(Alice) shared secret:\t", ss_a)
    print(  "(Bob)   shared secret:\t", ss_b)
    print("\nECDH Verification\nResult:\t", ret)