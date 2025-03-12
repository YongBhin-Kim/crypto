# KISA_SEED_CMAC.py
# -----------------------------------------------------------------------------
# Filename: KISA_SEED_CMAC.py
# Description: Python implementation of SEED CMAC generation and verification
# Author: Yongbhin Kim
# Company: KISA Next Generation Cryptography Technology Team
# Year: 2024
# -----------------------------------------------------------------------------

import struct
from KISA_SEED import SEED

class SEED_CMAC:
    BLOCK_SIZE = 16

    def __init__(self, mKey, tag_len=16):
        if len(mKey) != 16:
            raise ValueError(f"Invalid key length: {len(mKey)}. Must be 16 bytes.")
        if tag_len > self.BLOCK_SIZE:
            raise ValueError(f"TAG length must be <= {self.BLOCK_SIZE} bytes.")
        self.tag_len = tag_len
        self.rKey = SEED.key_schedule(mKey)

    def _subkey_scheduled(self, key):
        """Generate subkey K1 or K2 from the given key."""
        carry = key[0] >> 7
        for i in range(len(key) - 1):
            key[i] = ((key[i] << 1) & 0xFF) | (key[i + 1] >> 7)
        key[-1] = (key[-1] << 1) & 0xFF
        if carry:
            key[-1] ^= 0x87  # XOR with Rb
        return key

    @staticmethod
    def _xor128(A, B):
        return [A[i] ^ B[i] for i in range(len(A))]

    @staticmethod
    def _ZERO128():
        return [0x00] * 16

    @staticmethod
    def _byte2word(src):
        return [int.from_bytes(src[i:i + 4], 'big') for i in range(0, len(src), 4)]

    @staticmethod
    def _word2byte(src):
        return b''.join(x.to_bytes(4, 'big') for x in src)

    def _encrypt(self, block):
        block_bytes = bytes(block)
        encrypted_bytes = SEED.encrypt(block_bytes, self.rKey)
        return list(encrypted_bytes)

    def generate(self, pIn):
        inLen = len(pIn)
        ZERO = [0] * self.BLOCK_SIZE
        state = ZERO[:]

        # Generate L and subkeys K1, K2
        L = self._encrypt(ZERO)  
        K1 = self._subkey_scheduled(L[:])  
        K2 = self._subkey_scheduled(K1[:])  

        if inLen == 0:
            # Handle empty input case
            last_block = [K2[i] ^ 0x80 if i == 0 else K2[i] for i in range(self.BLOCK_SIZE)]
        else:
            # Process input blocks
            blockLen = (inLen + self.BLOCK_SIZE) // self.BLOCK_SIZE
            
            for i in range(blockLen - 1):
                block = pIn[i * self.BLOCK_SIZE:(i + 1) * self.BLOCK_SIZE]
                block = list(block) + [0x00] * (self.BLOCK_SIZE - len(block)) 
                state = self._encrypt(self._xor128(state, block))
        
            # Process last block
            i = blockLen - 1
            for j in range(0, inLen - (i * self.BLOCK_SIZE)):
                state[j] ^= pIn[self.BLOCK_SIZE * i + j] ^ K2[j]

            j = inLen - (i * self.BLOCK_SIZE)
            state[j] ^= 0x80 ^ K2[j]
            state[j + 1:self.BLOCK_SIZE] = [state[k] ^ K2[k] for k in range(j + 1, self.BLOCK_SIZE)]

            last_block = state

        # Final encryption
        tag = self._encrypt(last_block)
        return bytes(tag[:self.tag_len])

    def verify(self, tag, pIn):
        calculated_mac = self.generate(pIn)
        return calculated_mac == tag


# Example usage
if __name__ == "__main__":
    # SEED CMAC Test 1
    key = b"\xCC" * 16
    plaintext = b""

    cmac = SEED_CMAC(key, tag_len=8)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 1)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)

    # SEED CMAC Test 2
    key = b"\x8F\x97\x16\x85\x67\x5E\x0F\xEF\x6A\xD5\x70\xB4\x9E\xF4\x25\xA4"
    plaintext = b"\xE1\x31\x97\x12\x8B\x98\x73\x87\x4F\x1E\xAD\xB8\x62\x33\xC4\xB6\xDF\x96"

    cmac = SEED_CMAC(key, tag_len=10)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 2)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)

    # SEED CMAC Test 3
    key = b"\x06\xCC\xB9\xA7\x6C\x60\xEB\x86\xB1\x27\x12\xDE\x82\x6C\x5B\xF8"
    plaintext = b""

    cmac = SEED_CMAC(key, tag_len=11)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 3)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)

    # SEED CMAC Test 4
    key = b"\x35\x49\xE0\x15\x3A\x6D\x23\x83\x8B\xEA\x96\xA9\xD7\x0A\x6D\xBF"
    plaintext = b"\x0B\x50\x2F\xFB\x0E\x5D\x0C\xA0\xAF\x10\x70\x37\xDC\xF1\xD8\x1C" \
                b"\x14\xD0\xE5\x3D\x1E\xA9\xAE\x1F\xFC\xCB\x5B\x22\x1B\x39\xF0\x05" \
                b"\xCC\x51\x66\xAC\xF8\xAB\x00\x21\x0E\x44\xDC\xDA\x41\xDC\xC9\x3A" \
                b"\xF1\x9C\x58\x59"

    cmac = SEED_CMAC(key, tag_len=12)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 4)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)
    
    # SEED CMAC Test 5
    key = b"\xF5\xE7\x3B\xF2\x24\x3C\xD9\x83\x74\x84\xA0\x96\x49\x1E\xC0\xA7"
    plaintext = b"\xFA\x9D\xD5\xFB\x8D\x78\xC4\xEC\xC2\x02\xE5\x81\x54\x9D\x68\x41" \
                b"\x7B\x2B\xF1\x49\xB5\xC6\x86\x9D\xC6\xB1\xAB\xBC\xFD\x8E\xCB\x77" \
                b"\xBA\xD1\xDA\x02\x2D\x74\x39\x4C\x60\xED\xDA\x8C\x78\x5D\xA4\x1C" \
                b"\x38\x0A\x19\x8B\xC6\x0F\x36\xEE\xB2\x52\x9A\x76\x34\xB7\xEB\x48" \
                b"\xB5\x93\x76\x88\x91\x64\x15\xB7\x1C\xBF\x56\x40\xE3\x89\xD9\x4D" \
                b"\x34\x6A\xFA\xDF\xE0\x7F\xB0\x1E\x3F\x4F\xB5\xEE\x75\x01\xE8\xC2" \
                b"\xF4\xCC\xEF\xB5\x42\xAE\x20\xD7\xFD\x61\xA2\xC4\x1C\x8B\xCF\x7C" \
                b"\x77\x35\xDD\x6E\x8A\x7E\xBE\xD6\x75\x90\x44\x49\x48\xD4\x89\x8E" \
                b"\x7E\x62\x8E\xB0\xC7\xBC\x22\x51\x0B\xBB\x06\x41\xAB\xC9\x4E\x50" \
                b"\x0A\x51\x0A\x60\x4C\x74\x26\xBE\x5D\xFE\x8F\xB2\x35\x98\x97\x54" \
                b"\x5B\x3F\x9B\xA2\xA8\xF4\xE3\xD0\x4E\xB5\xC9\xDF\x19\xAD\x1E\x71" \
                b"\xF4\xA8\xC9\xDC\xAE\xC9\xB1\x7D\xFE\x7F\xDE\x4F\xC5\xB5"

    cmac = SEED_CMAC(key, tag_len=13)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 5)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)
    
    # SEED CMAC Test 6
    key = b"\xAB\x06\x00\xA3\xF9\x67\x3E\x08\xAE\xE0\x4A\xB7\x64\xB3\x35\x0E"
    plaintext = b"\x53\x46\x98\xD5\x75\xBD\xD5\x70\xE9\xCE\x9F\x59\x96\xD1\xBD\xCE" \
                b"\x10\x17\x0A\xC7\xBF\x7D\xC1\x2B\x3E\x41\xF7\x43\x01\x14\x69\x6F" \
                b"\x3B\x70\x78\x18\xFE\x2B\x72\xE5\xA4\x4D\x13\x32\x6F\x1F"

    cmac = SEED_CMAC(key, tag_len=14)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 6)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)
    
    # SEED CMAC Test 7
    key = b"\xC1\x09\x85\x44\xDE\x3C\x90\x7F\x5F\x68\x3F\x1A\xD6\x68\x50\xEB"
    plaintext = b"\x97\xCF\x60\x2C\xBE\xD8\x0C\x17\x73\x9C\x57\xB3\x6C\x88\x4B\xED" \
                b"\xB4\x0D\xE4\xEA\xAB\x99\x29\x9C\x4F\xC7\x9C\x93\xB9\xD3\xD4\x16" \
                b"\xEA\x50\x69\x73\xC8\x1D\x10\x93\x64\x95\x07\xD1\x7E\x06\xB4\x0C" \
                b"\x4B\x64\x89\xFB\x76\x3F\x2A\xC1\x64\xF3\xD2\xC2\xBC\x1F\xF3\xB4" \
                b"\x27\x58\x1C\xF9\x54\x1E\x20\x2C\x40\x0E\x75\xFA\xB4\x5A\xDA\x33" \
                b"\x0F\x77\x3C\x20\x45\x15\xDB\x18\x28\x54\xA9\x4E\xE6\x35\xF2\xED" \
                b"\xD3\x4E\x42\x67\x69\xC3\x84\x09\x8D\x71\x67\xD4\x14\x6C\x06\x88" \
                b"\x86\xAC\xC7\x01\x22\x03\x83\xC6\x22\x52\xE8\xE0\x40\xFD\x1C\xE8" \
                b"\x78\x9C\xA3\x64\x10\xF4\x83\x54\xD6\x25\xA6\x07\xA9\x24\x7F\x33" \
                b"\x3A\x6C\xF1"

    cmac = SEED_CMAC(key, tag_len=15)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 7)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)
    
    # SEED CMAC Test 8
    key = b"\xB9\x28\xC9\x8B\x08\x37\xE8\x87\x45\x2C\x42\x0E\x36\x07\xE7\xB9"
    plaintext = b"\xB9\x28\xC9\x8B\x08\x37\xE8\x87\x45\x2C\x42\x0E\x36\x07\xE7\xB9" \
                b"\xB9\x28\xC9\x8B\x08\x37\xE8\x87\x45\x2C\x42\x0E\x36\x07\xE7\xB9"

    cmac = SEED_CMAC(key, tag_len=16)
    tag = cmac.generate(plaintext)
    verify = cmac.verify(tag, plaintext)

    print(" [SEED CMAC(Test 8)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Tag:              ", tag.hex().upper())
    print("Verify:           ", verify)