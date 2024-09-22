"""This Python module is an implementation of the ecdsa algorithm.
From https://github.com/YongBhin-Kim/crypto/tree/master/ECDSA"""

import math
import random
from sha256 import sha256

# Domain parameters of secp256k1 (y^2 = x^3 + 7 mod m)
a = 0
b = 7
m = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
G = (0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
     0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8)
n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

# Additive operation over secp256k1
def _add(a, b, p, q, m):
    if q == (math.inf, math.inf):
        return p
    
    x1 = p[0]
    y1 = p[1]
    x2 = q[0]
    y2 = q[1]
    
    if p == q:
        # Doubling (slope (s) = (3 * x1 ^ 2 + a) / (2 * y1) mod m)
        r = 2 * y1
        r_inv = pow(r, m-2, m)   # FLT(Fermat's Little Theorem)
        s = (r_inv * (3 * (x1 ** 2) + a)) % m
    else:
        r = x2 - x1
        r_inv = pow(r, m-2, m)
        s = (r_inv * (y2 - y1)) % m
        
    x3 = (s ** 2 - x1 - x2) % m
    y3 = (s * (x1 - x3) - y1) % m
    return x3, y3

# Generate private key for ECDSA signing
def _generate_sk():
    while(1):
        d = random.getrandbits(256)
        if d > 0 & d < n:
            break
        
    return d

# Compute public key for ECDSA verifing
def _generate_pk(d, g):
    bits = bin(d)
    bits = bits[2:len(bits)]
    
    # Initialize: bits[0] = 1 (always)
    K = g
    
    # From the second bit, Double-and-Add
    bits = bits[1:len(bits)]
    for bit in bits:
        K = _add(a, b, K, K, m) # doubling
        if bit == '1': # multiply
            K = _add(a, b, K, g, m)
            
    return K

# Multiplicative operation over secp256k1 (same to compute public key)
def _mul(d, g):
    return _generate_pk(d, g)

# ECDSA Signature algorithm
def ecdsa_sign(message: str):

    # Compute sign 'r'
    d = _generate_sk() # Generate private key of signer
    Q = _generate_pk(d, G) # Compute public key of signer
    
    k = _generate_sk() # Genertae ephemeral(secret) key
    x, y = _mul(k, G)
    r = x % n 

    # Compute sign 's'
    message = message.encode() # Convert UTF-8 byte array of message string
    e = sha256(message).hex()
    e = int(e, 16)

    k_inv = pow(k, n-2, n) # FLT(Fermat's Little Theorem)
    s = ((e + d * r) * k_inv) % n # Compuyte sign 's'
    
    return Q, r, s

# ECDSA Verification algorithm
def ecdsa_verify(message: str, Q, r, s):
    
    message = message.encode() # Convert UTF-8 byte array of message string
    e = sha256(message).hex()
    e = int(e, 16)
    
    w = pow(s, n-2, n)
    u1 = (w * e) % n
    u2 = (w * r) % n
    v1 = _mul(u1, G)
    v2 = _mul(u2, Q)
    x, y = _add(a, b, v1, v2, m)
    
    if r == x % n:
        return True
    else:  
        return False
        

if __name__ == "__main__":
    
    message = 'Hello World!'
    
    # ECDSA(SHA-256) Signing
    Q, r, s = ecdsa_sign(message)
    
    # ECDSA(SHA-256) Verification
    ret = ecdsa_verify(message, Q, r, s)
    
    # Debug
    print("\nMessage =", message)
    print("\nECDSA(SHA-256) Sign :")
    print("   r =", hex(r))
    print("   s =", hex(s))
    print("\nECDSA(SHA-256) Verify :")
    print("   Result =", ret)
    print()
    