# KISA_SEED_CCM.py
# -----------------------------------------------------------------------------
# Filename: KISA_SEED_CCM.py
# Description: Python implementation of SEED CCM mode encryption and decryption
# Author: Yongbhin Kim
# Company: KISA Next Generation Cryptography Technology Team
# Year: 2024
# -----------------------------------------------------------------------------

import struct
from KISA_SEED import SEED

class SEED_CCM:

    BLOCK_SIZE = 16
    def __init__(self, nonce, aad, mKey, tag_len=16):
        self.nonce = nonce
        self.aad = aad
        self.mKey = mKey
        if len(mKey) != 16:
            raise ValueError(f"Invalid key length: {len(mKey)}. must be 16 bytes.")
        # MAC tag length (Tlen)
        self.tag_len = tag_len
        if tag_len not in (4, 6, 8, 10, 12, 14, 16):
            raise ValueError("Parameter 'tag_len' must be even" " and in the range 4..16 (not %d)" % tag_len)
        self.rKey = SEED.key_schedule(mKey)

    @staticmethod
    def _shiftr(x, n):
        if len(x) != 4:
            raise ValueError("Input list x must have exactly 4 elements.")
        for i in range(3, 0, -1):
            x[i] = ((x[i] >> n) | (x[i-1] << (32 - n))) & 0xFFFFFFFF
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
        padded_src = src.ljust(16, b'\x00')  # Pad with zeros if src is less than 16 bytes
        return [int.from_bytes(padded_src[i:i+4], 'big') for i in range(0, 16, 4)]

    @staticmethod
    def _word2byte(src):
        return b''.join([x.to_bytes(4, 'big') for x in src])

    def _encrypt(self, block):
        block_bytes = self._word2byte(block)
        encrypted_bytes = SEED.encrypt(block_bytes, self.rKey)
        return self._byte2word(encrypted_bytes)

    def encrypt(self, pt):
        ptLen = len(pt)
        nonceLen = len(self.nonce)
        aadLen = len(self.aad)

        CTR_in = self._ZERO128()
        MAC = self._ZERO128()

        CTR_in = self._byte2word(self.nonce)
        self._shiftr(CTR_in, 8)

        flag = 14 - nonceLen
        CTR_in[0] ^= (flag << 24)

        MAC = self._encrypt(CTR_in)

        ct = bytearray(ptLen + self.tag_len)
        for i in range(0, ptLen, self.BLOCK_SIZE):
            self._increase(CTR_in)
            tmp = self._ZERO128()
            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(pt[i:])
            else:
                tmp = self._byte2word(pt[i:i+self.BLOCK_SIZE])

            CTR_out = self._encrypt(CTR_in)
            tmp = self._xor128(tmp, CTR_out)

            if (ptLen - i) < self.BLOCK_SIZE:
                ct[i:] = self._word2byte(tmp)
            else:
                ct[i:i+self.BLOCK_SIZE] = self._word2byte(tmp)

        CBC_in = self._byte2word(self.nonce)
        self._shiftr(CBC_in, 8)

        flag = (aadLen > 0) << 6
        flag ^= ((self.tag_len - 2) >> 1) << 3
        flag ^= 14 - nonceLen

        CBC_in[0] ^= (flag << 24)
        CBC_in[3] ^= ptLen

        CBC_out = self._encrypt(CBC_in)

        if aadLen > 0:
            tmpLen = min(aadLen, 14)
            CBC_in = self._ZERO128()
            CBC_in = self._byte2word(self.aad[:tmpLen])
            self._shiftr(CBC_in, 16)
            CBC_in[0] ^= (aadLen << 16)
            CBC_in = self._xor128(CBC_in, CBC_out)
            CBC_out = self._encrypt(CBC_in)

            for i in range(tmpLen, aadLen, self.BLOCK_SIZE):
                CBC_in = self._ZERO128()
                if (aadLen - i) < self.BLOCK_SIZE:
                    CBC_in = self._byte2word(self.aad[i:])
                else:
                    CBC_in = self._byte2word(self.aad[i:i+self.BLOCK_SIZE])
                CBC_in = self._xor128(CBC_in, CBC_out)
                CBC_out = self._encrypt(CBC_in)

        for i in range(0, ptLen, self.BLOCK_SIZE):
            tmp = self._ZERO128()
            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(pt[i:])
            else:
                tmp = self._byte2word(pt[i:i+self.BLOCK_SIZE])
            CBC_in = self._xor128(tmp, CBC_out)
            CBC_out = self._encrypt(CBC_in)

        MAC = self._xor128(MAC, CBC_out)
        ct[ptLen:] = self._word2byte(MAC)[:self.tag_len]
        return bytes(ct)
    
    def decrypt(self, ct):
        ctLen = len(ct)
        ptLen = ctLen - self.tag_len
        nonceLen = len(self.nonce)
        aadLen = len(self.aad)

        # Separate the MAC from the ciphertext
        received_mac = ct[ptLen:]
        ct = ct[:ptLen]

        CTR_in = self._ZERO128()
        CTR_out = self._ZERO128()
        CBC_in = self._ZERO128()
        CBC_out = self._ZERO128()
        MAC = self._ZERO128()

        # Step 1: CTR initialization
        CTR_in = self._byte2word(self.nonce)
        self._shiftr(CTR_in, 8)
        flag = 14 - nonceLen
        CTR_in[0] ^= (flag << 24)
        MAC = self._encrypt(CTR_in)

        # Step 2: Decrypt Ciphertext
        pt = bytearray(ptLen)
        for i in range(0, ptLen, self.BLOCK_SIZE):
            self._increase(CTR_in)
            tmp = self._ZERO128()
            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(ct[i:])
            else:
                tmp = self._byte2word(ct[i:i+self.BLOCK_SIZE])

            CTR_out = self._encrypt(CTR_in)
            tmp = self._xor128(tmp, CTR_out)

            if (ptLen - i) < self.BLOCK_SIZE:
                pt[i:] = self._word2byte(tmp)
            else:
                pt[i:i+self.BLOCK_SIZE] = self._word2byte(tmp)

        # Step 3: CBC-MAC initialization
        CBC_in = self._byte2word(self.nonce)
        self._shiftr(CBC_in, 8)
        flag = (aadLen > 0) << 6
        flag ^= ((self.tag_len - 2) >> 1) << 3
        flag ^= 14 - nonceLen
        CBC_in[0] ^= (flag << 24)
        CBC_in[3] ^= ptLen
        CBC_out = self._encrypt(CBC_in)

        # Step 4: Process AAD for CBC-MAC
        if aadLen > 0:
            tmpLen = min(aadLen, 14)
            CBC_in = self._ZERO128()
            CBC_in = self._byte2word(self.aad[:tmpLen])
            self._shiftr(CBC_in, 16)
            CBC_in[0] ^= (aadLen << 16)
            CBC_in = self._xor128(CBC_in, CBC_out)
            CBC_out = self._encrypt(CBC_in)


            for i in range(tmpLen, aadLen, self.BLOCK_SIZE):
                CBC_in = self._ZERO128()
                if (aadLen - i) < self.BLOCK_SIZE: 
                    CBC_in = self._byte2word(self.aad[i:])
                else:
                    CBC_in = self._byte2word(self.aad[i:i+self.BLOCK_SIZE])
                CBC_in = self._xor128(CBC_in, CBC_out)
                CBC_out = self._encrypt(CBC_in)

        # Step 5: Process Plaintext for CBC-MAC
        for i in range(0, ptLen, self.BLOCK_SIZE):
            tmp = self._ZERO128()
            if (ptLen - i) < self.BLOCK_SIZE:
                tmp = self._byte2word(pt[i:ptLen])  
            else:
                tmp = self._byte2word(pt[i:i+self.BLOCK_SIZE])

            CBC_in = self._xor128(tmp, CBC_out)
            CBC_out = self._encrypt(CBC_in)
            
        # Step 6: Final MAC computation
        MAC = self._xor128(MAC, CBC_out)
        calculated_mac = self._word2byte(MAC)[:self.tag_len]

        # Step 7: MAC verification
        if calculated_mac != received_mac:
            raise ValueError("MAC check failed, the message has been tampered with or is invalid.")

        return bytes(pt[:ptLen])


# Example usage
if __name__ == "__main__":
    # SEED CCM Mode Test 1
    key = b"\xFA\xB5\xE5\xDE\x43\x50\xE5\xA4\xE0\xF1\xDF\x63\xE4\x6A\x2A\xA0"
    plaintext = b"\xE5\x46\xF3\x2B\xB5\xB3\x57\x40\xF3\xC4\x08\xC6\xE1\xBF\x02\x53\x09\x1C\xB2\x32\xDC\x94\xB9\x13\x99\x7A\xED\x01\x70\x4E\xA0\x95\xE8\x90\x26\x69\x7E"
    nonce = b"\x0C\x91\x14\x08\xA5\x95\xDF\x62\xA9\x92\x09\xC2"
    aad = b"\x2C\x62\xD1\xFF\xF6\xB7\xF6\x68\x72\x66\xC2\xB3\xC7\x06\x47\x36\x44\xBA\xE9\x5A\x01\x4B\x1C\x4C\xC3\x7A\x6F\xF5\x21\x94\xCA\x2D"
    
    tag_len = 16
    seed = SEED_CCM(nonce, aad, key, tag_len)

    ciphertext = seed.encrypt(plaintext)
    decryptedtext = seed.decrypt(ciphertext)

    print(" [SEED CCM Mode(Test 1)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Nonce:            ", nonce.hex().upper())
    print("Additional Data:  ", aad.hex().upper())
    print("Ciphertext:       ", ciphertext.hex().upper())
    print("Decrypted Text:   ", decryptedtext.hex().upper())

    # SEED CCM Mode Test 2
    key       = b"\x00\x2B\x30\xE2\x0C\xCC\x65\xE9\x5D\xAE\x1F\xDF\x24\x11\xC0\xD5"
    nonce     = b"\x18\x8C\xC3\x10\xD2\xA4\x28"
    aad       = b"\x5C\xE9\x17\xAF\x1A\xF4\x73\x2C\xC2\x20\xFC\x02\x29\x79\x65\x0E"
    plaintext = b"\x55\x98\x69\xFF\x1A\xDD\xCC\x72\x61\xCD\xC9\xCB\x40\xD6\x76\x26"
    
    tag_len = 16
    seed = SEED_CCM(nonce, aad, key, tag_len)
    
    ciphertext = seed.encrypt(plaintext)
    decryptedtext = seed.decrypt(ciphertext)

    print("\n\n [SEED CCM Mode(Test 2)] ")
    print("Key:              ", key.hex().upper())
    print("Plaintext:        ", plaintext.hex().upper())
    print("Nonce:            ", nonce.hex().upper())
    print("Additional Data:  ", aad.hex().upper())
    print("Ciphertext:       ", ciphertext.hex().upper())
    print("Decrypted Text:   ", decryptedtext.hex().upper())