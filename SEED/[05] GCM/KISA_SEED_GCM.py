# KISA_SEED_GCM.py
# -----------------------------------------------------------------------------
# Filename: KISA_SEED_GCM.py
# Description: Python implementation of SEED GCM mode encryption and decryption
# Author: Yongbhin Kim
# Company: KISA Next Generation Cryptography Technology Team
# Year: 2024
# -----------------------------------------------------------------------------

import struct
from KISA_SEED import SEED

class SEED_GCM:
    
    BLOCK_SIZE = 16
    def __init__(self, nonce, aad, mKey, tag_len=16):
        self.nonce = nonce
        self.aad = aad
        self.mKey = mKey
        if len(mKey) != 16:
            raise ValueError(f"Invalid key length: {len(mKey)}. Must be 16 bytes.")
        if tag_len > self.BLOCK_SIZE:
            raise ValueError(f"TAG length must be <= {self.BLOCK_SIZE} bytes.")
        self.tag_len = tag_len
        self.rKey = SEED.key_schedule(mKey)
        self.H = self._encrypt([0, 0, 0, 0])  # H = E(Key, 0^128)
        self.M8 = self._generate_m8(self.H)
    
    R8 = [ 0x00000000, 0x01c20000, 0x03840000, 0x02460000, 0x07080000, 0x06ca0000, 0x048c0000, 0x054e0000,
        0x0e100000, 0x0fd20000, 0x0d940000, 0x0c560000, 0x09180000, 0x08da0000, 0x0a9c0000, 0x0b5e0000,
        0x1c200000, 0x1de20000, 0x1fa40000, 0x1e660000, 0x1b280000, 0x1aea0000, 0x18ac0000, 0x196e0000,
        0x12300000, 0x13f20000, 0x11b40000, 0x10760000, 0x15380000, 0x14fa0000, 0x16bc0000, 0x177e0000,
        0x38400000, 0x39820000, 0x3bc40000, 0x3a060000, 0x3f480000, 0x3e8a0000, 0x3ccc0000, 0x3d0e0000,
        0x36500000, 0x37920000, 0x35d40000, 0x34160000, 0x31580000, 0x309a0000, 0x32dc0000, 0x331e0000,
        0x24600000, 0x25a20000, 0x27e40000, 0x26260000, 0x23680000, 0x22aa0000, 0x20ec0000, 0x212e0000,
        0x2a700000, 0x2bb20000, 0x29f40000, 0x28360000, 0x2d780000, 0x2cba0000, 0x2efc0000, 0x2f3e0000,
        0x70800000, 0x71420000, 0x73040000, 0x72c60000, 0x77880000, 0x764a0000, 0x740c0000, 0x75ce0000,
        0x7e900000, 0x7f520000, 0x7d140000, 0x7cd60000, 0x79980000, 0x785a0000, 0x7a1c0000, 0x7bde0000,
        0x6ca00000, 0x6d620000, 0x6f240000, 0x6ee60000, 0x6ba80000, 0x6a6a0000, 0x682c0000, 0x69ee0000,
        0x62b00000, 0x63720000, 0x61340000, 0x60f60000, 0x65b80000, 0x647a0000, 0x663c0000, 0x67fe0000,
        0x48c00000, 0x49020000, 0x4b440000, 0x4a860000, 0x4fc80000, 0x4e0a0000, 0x4c4c0000, 0x4d8e0000,
        0x46d00000, 0x47120000, 0x45540000, 0x44960000, 0x41d80000, 0x401a0000, 0x425c0000, 0x439e0000,
        0x54e00000, 0x55220000, 0x57640000, 0x56a60000, 0x53e80000, 0x522a0000, 0x506c0000, 0x51ae0000,
        0x5af00000, 0x5b320000, 0x59740000, 0x58b60000, 0x5df80000, 0x5c3a0000, 0x5e7c0000, 0x5fbe0000,
        0xe1000000, 0xe0c20000, 0xe2840000, 0xe3460000, 0xe6080000, 0xe7ca0000, 0xe58c0000, 0xe44e0000,
        0xef100000, 0xeed20000, 0xec940000, 0xed560000, 0xe8180000, 0xe9da0000, 0xeb9c0000, 0xea5e0000,
        0xfd200000, 0xfce20000, 0xfea40000, 0xff660000, 0xfa280000, 0xfbea0000, 0xf9ac0000, 0xf86e0000,
        0xf3300000, 0xf2f20000, 0xf0b40000, 0xf1760000, 0xf4380000, 0xf5fa0000, 0xf7bc0000, 0xf67e0000,
        0xd9400000, 0xd8820000, 0xdac40000, 0xdb060000, 0xde480000, 0xdf8a0000, 0xddcc0000, 0xdc0e0000,
        0xd7500000, 0xd6920000, 0xd4d40000, 0xd5160000, 0xd0580000, 0xd19a0000, 0xd3dc0000, 0xd21e0000,
        0xc5600000, 0xc4a20000, 0xc6e40000, 0xc7260000, 0xc2680000, 0xc3aa0000, 0xc1ec0000, 0xc02e0000,
        0xcb700000, 0xcab20000, 0xc8f40000, 0xc9360000, 0xcc780000, 0xcdba0000, 0xcffc0000, 0xce3e0000,
        0x91800000, 0x90420000, 0x92040000, 0x93c60000, 0x96880000, 0x974a0000, 0x950c0000, 0x94ce0000,
        0x9f900000, 0x9e520000, 0x9c140000, 0x9dd60000, 0x98980000, 0x995a0000, 0x9b1c0000, 0x9ade0000,
        0x8da00000, 0x8c620000, 0x8e240000, 0x8fe60000, 0x8aa80000, 0x8b6a0000, 0x892c0000, 0x88ee0000,
        0x83b00000, 0x82720000, 0x80340000, 0x81f60000, 0x84b80000, 0x857a0000, 0x873c0000, 0x86fe0000,
        0xa9c00000, 0xa8020000, 0xaa440000, 0xab860000, 0xaec80000, 0xaf0a0000, 0xad4c0000, 0xac8e0000,
        0xa7d00000, 0xa6120000, 0xa4540000, 0xa5960000, 0xa0d80000, 0xa11a0000, 0xa35c0000, 0xa29e0000,
        0xb5e00000, 0xb4220000, 0xb6640000, 0xb7a60000, 0xb2e80000, 0xb32a0000, 0xb16c0000, 0xb0ae0000,
        0xbbf00000, 0xba320000, 0xb8740000, 0xb9b60000, 0xbcf80000, 0xbd3a0000, 0xbf7c0000, 0xbebe0000 ]
    
    @staticmethod
    def _shiftr(x, n):
        if len(x) != 4:
            raise ValueError("Input list x must have exactly 4 elements.")
        for i in range(3, 0, -1):
            x[i] = ((x[i] >> n) | (x[i - 1] << (32 - n))) & 0xFFFFFFFF
        x[0] = (x[0] >> n) & 0xFFFFFFFF

    @staticmethod
    def _xor128(A, B):
        return [A[i] ^ B[i] for i in range(4)]

    @staticmethod
    def _increase(ctr):
        if ctr[3] == 0xFFFFFFFF:
            ctr[2] += 1
            ctr[3] = 0
        else:
            ctr[3] += 1

    @staticmethod
    def _ZERO128():
        return [0x00000000] * 4

    @staticmethod
    def _byte2word(src):
        padded_src = src.ljust(16, b'\x00')
        return [int.from_bytes(padded_src[i:i + 4], 'big') for i in range(0, 16, 4)]

    @staticmethod
    def _word2byte(src):
        return b''.join([x.to_bytes(4, 'big') for x in src])

    def _encrypt(self, block):
        block_bytes = self._word2byte(block)
        encrypted_bytes = SEED.encrypt(block_bytes, self.rKey)
        return self._byte2word(encrypted_bytes)

    def _generate_m8(self, H):
        # Initialize M8 with 256 entries of [0, 0, 0, 0]
        M8 = [[0, 0, 0, 0] for _ in range(256)]
        M8[128] = H[:]

        # Fill in M8[128], M8[64], ..., down to M8[1]
        for i in range(64, 0, -1):
            temp = M8[i << 1][:]  # Copy M8[i * 2]
            if temp[3] & 0x01:  # Check the least significant bit
                self._shiftr(temp, 1)
                temp[0] ^= 0xE1000000
            else:
                self._shiftr(temp, 1)
            M8[i] = temp[:]

        # Fill in M8[2] to M8[255]
        i = 2
        while i < 256:
            for j in range(1, i):
                if i + j < 256:  # Ensure the index stays within bounds
                    M8[i + j] = self._xor128(M8[i], M8[j])
            i <<= 1

        # Initialize M8[0]
        M8[0] = [0, 0, 0, 0]
        
        return M8

    def _ghash(self, out, inp, M8):
        Z = self._xor128(out, inp)
        W = self._ZERO128()
        
        for i in range(15):
            temp = (Z[3 - (i >> 2)] >> ((i & 3) << 3)) & 0xFF
            W = self._xor128(W, M8[temp])
            temp = W[3] & 0xFF
            self._shiftr(W, 8)
            W[0] ^= self.R8[temp]

        temp = (Z[0] >> 24) & 0xFF
        out[:] = self._xor128(W, M8[temp])

    def encrypt(self, pt):
        ptLen = len(pt)
        nonceLen = len(self.nonce)
        aadLen = len(self.aad)

        # Step 1: Compute GCTR_in and Z
        if nonceLen == 12:
            GCTR_in = self._byte2word(self.nonce + b'\x00\x00\x00\x01')
        else:
            GCTR_in = self._ZERO128()
            for i in range(0, nonceLen, self.BLOCK_SIZE):
                tmp = self._ZERO128()
                if (nonceLen - i) < self.BLOCK_SIZE:
                    tmp = self._byte2word(self.nonce[i:])
                else:
                    tmp = self._byte2word(self.nonce[i:i+self.BLOCK_SIZE])
                self._ghash(GCTR_in, tmp, self.M8)
            tmp = self._ZERO128()
            tmp[3] = nonceLen * 8
            self._ghash(GCTR_in, tmp, self.M8)

        Z = self._encrypt(GCTR_in)
        
        # Step 2: Encrypt plaintext
        ct = bytearray(ptLen + self.tag_len)
        for i in range(0, ptLen, self.BLOCK_SIZE):
            tmp = self._ZERO128()
            self._increase(GCTR_in)

            GCTR_out = self._encrypt(GCTR_in)

            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(pt[i:])
                GCTR_out = self._xor128(GCTR_out, tmp)
                ct[i:] = self._word2byte(GCTR_out)
            else:
                tmp = self._byte2word(pt[i:i+self.BLOCK_SIZE])
                GCTR_out = self._xor128(GCTR_out, tmp)
                ct[i:i+self.BLOCK_SIZE] = self._word2byte(GCTR_out)

        # Step 3: Compute GHASH
        GHASH_out = self._ZERO128()
        for i in range(0, aadLen, self.BLOCK_SIZE):
            GHASH_in = self._ZERO128()
            if (aadLen - i) < self.BLOCK_SIZE:
                GHASH_in = self._byte2word(self.aad[i:])
            else:
                GHASH_in = self._byte2word(self.aad[i:i+self.BLOCK_SIZE])
            self._ghash(GHASH_out, GHASH_in, self.M8)

        for i in range(0, ptLen, self.BLOCK_SIZE):
            GHASH_in = self._ZERO128()
            if (ptLen - i) < self.BLOCK_SIZE:
                GHASH_in = self._byte2word(ct[i:ptLen])
            else:
                GHASH_in = self._byte2word(ct[i:i+self.BLOCK_SIZE])
            self._ghash(GHASH_out, GHASH_in, self.M8)

        GHASH_in = self._ZERO128()
        GHASH_in[1] = aadLen * 8
        GHASH_in[3] = ptLen * 8
        self._ghash(GHASH_out, GHASH_in, self.M8)

        # Step 4: Compute final tag
        GHASH_out = self._xor128(GHASH_out, Z)

        ct[ptLen:] = self._word2byte(GHASH_out)[:self.tag_len]
        return bytes(ct)

    def decrypt(self, ct):
        ctLen = len(ct)
        ptLen = ctLen - self.tag_len
        nonceLen = len(self.nonce)
        aadLen = len(self.aad)

        # Separate ciphertext and tag
        received_tag = ct[ptLen:]
        ct = ct[:ptLen]

        # Step 1: Compute GCTR_in and Z
        if nonceLen == 12:
            GCTR_in = self._byte2word(self.nonce + b'\x00\x00\x00\x01')
        else:
            GCTR_in = self._ZERO128()
            for i in range(0, nonceLen, self.BLOCK_SIZE):
                tmp = self._ZERO128()
                if (nonceLen - i) < self.BLOCK_SIZE:
                    tmp = self._byte2word(self.nonce[i:])
                else:
                    tmp = self._byte2word(self.nonce[i:i+self.BLOCK_SIZE])
                self._ghash(GCTR_in, tmp, self.M8)
            tmp = self._ZERO128()
            tmp[3] = nonceLen * 8
            self._ghash(GCTR_in, tmp, self.M8)

        Z = self._encrypt(GCTR_in)

        # Step 2: Decrypt ciphertext
        pt = bytearray(ptLen)
        for i in range(0, ptLen, self.BLOCK_SIZE):
            tmp = self._ZERO128()
            self._increase(GCTR_in)

            GCTR_out = self._encrypt(GCTR_in)

            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(ct[i:])
                GCTR_out = self._xor128(GCTR_out, tmp)
                pt[i:] = self._word2byte(GCTR_out)
            else:
                tmp = self._byte2word(ct[i:i+self.BLOCK_SIZE])
                GCTR_out = self._xor128(GCTR_out, tmp)
                pt[i:i+self.BLOCK_SIZE] = self._word2byte(GCTR_out)

        # Step 3: Compute GHASH
        GHASH_out = self._ZERO128()
        for i in range(0, aadLen, self.BLOCK_SIZE):
            GHASH_in = self._ZERO128()
            if (aadLen - i) < self.BLOCK_SIZE:
                GHASH_in = self._byte2word(self.aad[i:])
            else:
                GHASH_in = self._byte2word(self.aad[i:i+self.BLOCK_SIZE])
            self._ghash(GHASH_out, GHASH_in, self.M8)

        for i in range(0, ctLen - self.tag_len, self.BLOCK_SIZE):
            GHASH_in = self._ZERO128()
            
            if (ctLen - self.tag_len - i) < self.BLOCK_SIZE:
                GHASH_in = self._byte2word(ct[i:(ctLen - self.tag_len)])
            else:
                GHASH_in = self._byte2word(ct[i:i + self.BLOCK_SIZE])

            self._ghash(GHASH_out, GHASH_in, self.M8)

        GHASH_in = self._ZERO128()
        GHASH_in[1] = aadLen * 8
        GHASH_in[3] = ptLen * 8
        self._ghash(GHASH_out, GHASH_in, self.M8)
        
        # Step 4: Verify tag
        calculated_tag = self._xor128(GHASH_out, Z)
        calculated_tag = self._word2byte(calculated_tag)[:self.tag_len]

        if calculated_tag != received_tag:
            raise ValueError("TAG check failed. The ciphertext has been tampered with or is invalid.")

        return bytes(pt[:ptLen])


# Example usage
if __name__ == "__main__":
    # SEED GCM Mode Test 1
    key       = b"\x10\x32\xF9\x90\xB7\x6B\x06\x86\xC0\xCF\x9B\xBB\x80\xAE\xE0\x8C"
    plaintext = b"\x67\x02\xC7\x2A\xA0\x4D\x49\xBD\xD4\x26\x9D\x67\x2A\x6C\x36\x9A\xD9\xC7\x2C\xDC\xDF\x8D\x92\xCB\xF6\xE2\x04\x5E\xC4\x24\x7F\x6D\x52\x86\x75\x74\xBF\xFA\x21\x94\x36\x55\x19\xDA\x1D\xAD\x22\xC4\x8F\x06\x47\x01\x0D\x2E\x2D\x79\x70\xE6\xA1\x8D\x22\x42\x73\xA0\x8E\x53\x87\xD6\xD5\x03\x29\x1B\xC3\x3F\xA1\x68\x01\x5C\x07\x41\x8C\xB3\x59\x83\x65\x8F\xCB\x5C\x8B\x4A\x5E\x9B\x26\xB2\xB4\x2A\x05\xB1\x23\xD8\x4A\x2E\x08\x5C\x64\x2E\x5E\x97\x3E\x3F\x8F\x1A\xB6\x16\x89\xE8\x51\x77\x15\x7D\x2D\x55\x64\x0F\x37\x3B\xEB\x13"
    nonce     = b"\x75\xE2\x53\x4A\x34\xF6\x5F\x85\xA2\x8E\x31\x8A"
    aad       = b"\x9D\xEA\x72\x03\x87\x44\x67\x5F\x02\x68\x77\xF2\x3C\x1F\x60\x56\xF7\x77\x00\xBA\x38\xAD\xB2\xE3\x3F\x50\xDB\x71\xBC\xA4\xC0\x64\x40\x45\x9B\xDE\xF2\x0C\xED\x2A\x83\x36\x15\xFE\x64\xC3\x22\xFD\x36\x1D\xE6\x80\x82\xFA\x4B\x96\xAA\x83\xEB\x4A\x1F\xB6\xDA\x24\xD5\x09\xC6\xF2\xF4\x50\x43\xC7\xD1\xE0\x60\x45\x1C\xF5\x7E\x18\x5B\x51\x62\xC3\x96\x26\x88\x9F\x54\x36\xBA\x20\xC7\x39\xE2\x5B\x44\x7F\x1D\xC5\xF6\xD6\x10\x3E\xD2\xAE\x7F\x4E\xCD\x7B\x1B\xAE\x4D\x5B\x9C\x0A\xDE\xF9\x10\x05\x27\xB1\x73\x7E\x1C\xF5\x7F\x11"

    seed = SEED_GCM(nonce, aad, key, tag_len=12)
    ciphertext = seed.encrypt(plaintext)
    decryptedtext = seed.decrypt(ciphertext)
    
    print(" [SEED GCM Mode(Test 1)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Nonce:            ", nonce.hex().upper())
    print("Additional Data:  ", aad.hex().upper())
    print("Ciphertext:       ", ciphertext.hex().upper())
    print("Decrypted Text:   ", decryptedtext.hex().upper())

    # SEED GCM Mode Test 2
    key       = b"\x11\xB1\x3A\xD7\x05\x56\x00\x9D\xA9\xD0\xA8\xA8\xC1\x1E\x41\x99"
    plaintext = b""
    nonce     = b"\xFC\x80\x17\x5A\x2A\xDF\x87\xA8\xA9\x69\x11\xCF\x20\x7C\xAF\xB5"
    aad       = b""

    print("\n\n [SEED GCM Mode(Test 2)] ")
    seed = SEED_GCM(nonce, aad, key, tag_len=16)
    ciphertext = seed.encrypt(plaintext)
    decryptedtext = seed.decrypt(ciphertext)
    
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Nonce:            ", nonce.hex().upper())
    print("Additional Data:  ", aad.hex().upper())
    print("Ciphertext:       ", ciphertext.hex().upper())
    print("Decrypted Text:   ", decryptedtext.hex().upper())