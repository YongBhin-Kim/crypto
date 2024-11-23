"""This Python module is an implementation of the secp256k1 operations.
From https://github.com/YongBhin-Kim/crypto/tree/master/ECDH"""

import collections

EllipticCurve = collections.namedtuple('EllipticCurve', 'name p a b G n h')

curve = EllipticCurve(
    # Domain parameters of secp256k1 (y^2 = x^3 + 7 mod m)
    'secp256k1',
    p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F,
    a = 0,
    b = 7,
    G = (0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798, # Base point
         0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8), 
    n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFebaaedce6af48a03bbfd25e8cd0364141, # Subgroup order
    h = 1,  # Subgroup cofactor
)

# Inverse operation over Zp
def _inverse_mod(k, p):
    """ Returns the inverse of k modulo p.
    This function returns the only integer x s.t. x*k == 1 (mod n)
    k must be non-zero and p must be a prime. """
    
    if k == 0:
        raise ZeroDivisionError('k divised by zero')
    
    if k < 0:
        # k ** -1 = p - (-k) ** -1 (mod p)
        return p - _inverse_mod(-k, p)
    
    # Extended Euclidean algotirhm
    s, old_s = 0, 1
    t, old_t = 1, 0
    r, old_r = p, k
    
    while r != 0:
        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s
        old_t, t = t, old_t - quotient * t
        
    gcd, x, y = old_r, old_s, old_t
    
    assert gcd == 1
    assert (k * x) % p == 1
    
    return x % p

# Functions that work on curve points
# Check the validation of point over elleptic curve
def _is_on_curve(point):
    """ Returns True if the given point lies on the elleptic curve. """
    
    if point is None:
        # None represents the point at infinity
        return True

    x, y = point
    
    return (y * y - x * x * x - curve.a * x - curve.b ) % curve.p == 0

# Additive operation over secp256k1
def _add(point1, point2):
    """ Returns the result of point1 + point2 according to the group law. """
    
    assert _is_on_curve(point1)
    assert _is_on_curve(point2)
    
    if point1 is None:
        # 0 + point2 = point2
        return point2
    
    if point2 is None:
        # point1 + 0 = point1
        return point1
    
    x1, y1 = point1
    x2, y2 = point2
    
    if x1 == x2 and y1 != y2:
        # point1 + (-point1) = 0
        return None
    
    if x1 == x2:
        # point1 == point2
        m = (3 * x1 * x1 + curve.a) * _inverse_mod(2 * y1, curve.p)
    else:
        # point1 != point2
        m = (y1 - y2) * _inverse_mod(x1 - x2, curve.p)
        
    x3 = m * m - x1 - x2
    y3 = y1 + m * (x3 - x1)
    result = (x3 % curve.p, -y3 % curve.p)
    
    assert _is_on_curve(result)
    
    return result

# Multiplicative operation over secp256k1
def mul(k, point):
    """ Returns k * point computed using the double and _add algotirhm. """
    
    assert _is_on_curve(point)
    
    if k % curve.n == 0 or point is None:
        # 0 * point or k * 0
        return None
    
    if k < 0:
        # k * point = -k * (-point)
        return mul(-k, point_neg(point)) # TODO : point_neg
    
    result = None
    addend = point
    
    while k:
        if k & 1:
            # Add
            result = _add(result, addend)
        
        # Double
        addend = _add(addend, addend)
        
        k >>= 1
        
    assert _is_on_curve(result)
    
    return result