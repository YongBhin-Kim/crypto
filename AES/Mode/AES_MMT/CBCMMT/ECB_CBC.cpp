/*============================================================
 *              Mode Of Operation and Property
 *              ECB vs CBC
============================================================*/
#include "ECB_CBC.h"



/* ====================================================
 * Function : fileSize
 * argument : 
 *              - const char *file_name : file name 
 * return : size of file
 * ==================================================== */
int fileSize(const char *file_name) {
    ifstream fin;
    fin.open(file_name, ios_base::binary);
    if (fin.fail()) {
        cout << "Input File Open Error" << endl;
        return -1;
    } 
    
    int _file_size;
    fin.seekg(0, fin.end);      // get-pointer 위치시키기
    _file_size = fin.tellg();   // tell "get-pointer"

    fin.close();
    return _file_size;
}

/* ====================================================
 * Function : padding
 * argument : 
 *              - byte input[]  : 0~15바이트 데이터
 *              - int in_length : 데이터 길이
 *              - byte out[16]  : 패딩된 16바이트(한 블록)
 * 
 * 예        : 01020304, 4 --> 01020304 80000000 00000000 00000000
 * 주의      : 입력이 0바이트도 가능
 * ==================================================== */
void padding(byte input[], int in_length, byte out[16]) {
    byte padding_start = 0x80;
    for (int i=0; i<in_length; i++) {
        out[i] = input[i];
    }

    if (in_length==16) 
        return;

    out[in_length] = padding_start;

    for (int i=in_length+1; i<16; i++) {
        out[i] = 0x00;
    }

}

/* ====================================================
 * Function : filePadding
 * argument : 
 *              - const char *src : file before padding
 *              - const char *dst : file after padding
 * 
 * 입력파일 크기 : N 바이트
 * 16의 배수 --> 출력크기 = N+16
 * else    --> 출력크기 = N보다 큰 가장 작은 16의 배수
 * ==================================================== */
void filePadding(const char *src, const char *dst) {
    ifstream fin;
    ofstream fout;

    fin.open(src, ios_base::binary);
    if (fin.fail()) {
        cout << "Input File Open Error : ";
        return;
    }

    fout.open(dst, ios_base::binary);
    if (fout.fail()) {
        cout << "Output File Open Error : ";
        return;
    }

    int file_size = fileSize(src);   // 164
    int num_blocks = file_size / 16; // 10
    int remainder  = file_size % 16; // 4

    byte buffer[16];
    // 패딩이 적용되지 않는 블록
    for (int i=0; i<num_blocks; i++) {
        fin.read((char*)buffer, 16);
        // 한블록 암호화
        fout.write((char*)buffer, 16);
    }

    byte last_input_block[16];
    byte last_out_block[16];
    byte ch;
    for (int i=0; i<remainder; i++) { // 마지막 블록 남은 데이터 + 패딩 으로 채우기
        fin.read((char*)&ch, 1);
        last_input_block[i] = ch;
    }
    
    padding(last_input_block, remainder, last_out_block); // 마지막 블록 남은 데이터 + 패딩 으로 채우기
    fout.write((char*)last_out_block, 16);

    fin.close();
    fout.close();

}

/* ====================================================
 * Function : gen_binfile
 * Description : Generate binary file for pt
 * Function    : gen_binfile
 * argument    : 
 *              - const char *file_name  : pt값을 저장(삽입)할 파일
 * ==================================================== */
void gen_binfile(const char *file_name) {
    ofstream fout;

    fout.open(file_name, ios_base::binary);
    if ( fout.fail() ) {
        cout << "File Open Error" << endl;
        return;
    }

    // pt == Yongbhin * 40
    // YgonbhinYgonbhinYgonbhin....Ygonbhin
    // 이 경우 사이즈가 320 => 16의 배수 이므로 마지막 한 블록이 패딩될 것임
    const char *buffer = "Yongbhin";
    for (int i=0; i<40; i++) { 
        fout.write((char*)buffer, sizeof(buffer));
    }

    fout.close();
}

/* ====================================================
 * Function : AESECB_ENC
 * argument : 
 *              - const char *input_file  : pt값을 불러올 파일
 *              - const char *output_file : ct값을 삽입할 파일
 * ==================================================== */
void AESECB_ENC(const char *input_file, const char *output_file) {
    ifstream fin;
    ofstream fout;

    // Let KEY = 00000000000000000000000000000000
    byte key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

    // open plaintext file
    fin.open(input_file, ios_base::binary);
    if ( fin.fail() ) {
        cout << "Input(Plaintext) File Open Error!" << endl;
        return;
    }

    // open ciphertext file
    fout.open(output_file, ios_base::binary);
    if ( fout.fail() ) {
        cout << "Output File Open Error!" << endl;
        return;
    }

    int file_size = fileSize(input_file); // 평문 크기 (바이트)
    int num_block = file_size / 16; // 마지막 블록을 제외한 블록 개수
    int remainder = file_size % 16; // 마지막 블록 사이즈

    // 평문을 읽어서 ECB 모드로 암호화 : 마지막 블록만 패딩
    byte pt[16], ct[16];
    for (int i=0; i<num_block; i++) {
        fin.read((char*)pt, 16);
        AES32_Enc(pt, key, ct); // 한 블록 AES 암호화
        fout.write((char*)ct, 16);
    }

    byte last_block[16], last_padding_block[16];
    fin.read((char*)last_block, 16);
    padding(last_block, remainder, last_padding_block);
    AES32_Enc(last_padding_block, key, ct); // 마지막(패딩된) 블록 AES 암호화
    fout.write((char*)ct, 16);

    fin.close();
    fout.close();

}

/* ====================================================
 * Function : AESECB_DEC
 * argument : 
 *              - const char *input_file  : ct값을 불러올 파일
 *              - const char *output_file : dec값을 삽입할 파일
 * ==================================================== */
void AESECB_DEC(const char *input_file, const char *output_file) {
    ifstream fin;
    ofstream fout;

    // Let KEY = 00000000000000000000000000000000 (ECB ENC와 동일하게)
    byte key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

    // open ciphertext file
    fin.open(input_file, ios_base::binary);
    if ( fin.fail() ) {
        cout << "Input(Ciphertext) File Open Error!" << endl;
        return;
    }

    // open file to save decrypted text
    fout.open(output_file, ios_base::binary);
    if ( fout.fail() ) {
        cout << "Output File Open Error!" << endl;
        return;
    }

    int file_size = fileSize(input_file); // 암호문 크기 (바이트)
    int num_block = file_size / 16; // 암호문 블록 개수
    int remainder = file_size % 16; // 이 값이 0이어야 암호문의 크기(바이트)가 16의 배수
    if ( remainder != 0 ) {
        cout << "Ciphertext Size Error!" << endl;
        return;
    }

    // 암호문을 읽어서 ECB 모드로 복호화
    byte ct[16], dec[16];
    for (int i=0; i<=num_block-2; i++) {
        fin.read((char*)ct, 16);
        AES8_Dec(ct, key, dec); // 한 블록 AES 복호화
        fout.write((char*)dec, 16);
    }

    // 마지막 블록 복호화
    fin.read((char*)ct, 16);
    AES8_Dec(ct, key, dec); // 마지막 블록 AES 복호화

    // 마지막 블록 패딩 제거
    int paddingIndex;
    for (paddingIndex=15; paddingIndex>=0; paddingIndex--) {
        if (dec[paddingIndex] == 0x80) {
            break;
        }
    }
    // 패딩을 제거한 마지막 평문(decrypted) write
    if (paddingIndex > 0) {
        byte last_dec_block[paddingIndex];
        for (int i=0; i<paddingIndex; i++)
            last_dec_block[i] = dec[i];
        fout.write((char*)last_dec_block, paddingIndex);
    }

    fin.close();
    fout.close();

}

/* ====================================================
 * Function : AESCBC_ENC
 * argument : 
 *              - const char *input_file  : pt값을 불러올 파일
 *              - const char *output_file : ct값을 삽입할 파일
 * ==================================================== */
void AESCBC_ENC(const char *input_file, const char *output_file) {
    ifstream fin;
    ofstream fout;

    // Let KEY = 00000000000000000000000000000000, iv = 00000000000000000000000000000000
    byte key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
    byte iv[16]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // open plaintext file
    fin.open(input_file, ios_base::binary);
    if ( fin.fail() ) {
        cout << "Input(Plaintext) File Open Error!" << endl;
        return;
    }
    // open ciphertext file
    fout.open(output_file, ios_base::binary);
    if ( fout.fail() ) {
        cout << "Output File Open Error!" << endl;
        return;
    }

    int file_size = fileSize(input_file); // 평문 크기 (바이트)
    int num_block = file_size / 16; // 마지막 블록을 제외한 블록 개수
    int remainder = file_size % 16; // 마지막 블록 사이즈

    // 평문을 읽어서 CBC 모드로 암호화
    byte pt[16], ct[16], state[16];

    // 첫 블록 : ct = Enc(pt^iv)
    fin.read((char*)pt, 16);
    for (int i=0; i<16; i++)
        state[i] = pt[i] ^ iv[i];
    AES32_Enc(state, key, ct);
    fout.write((char*)ct, 16);

    // 두 번 째 블록부터 패딩할 블록 제외한 마지막 블록까지, ct(n) = Enc(pt ^ ct(n-1))
    for (int i=1; i<num_block; i++) {
        fin.read((char*)pt, 16);
        for (int i=0; i<16; i++)
            state[i] = pt[i] ^ ct[i]; // 이 때 ct는 전 단계의 ct임

        AES32_Enc(state, key, ct); // 한 블록 AES 암호화
        fout.write((char*)ct, 16);
    }

    // Read last block : last_ct = Enc(padded_pt ^ previout_ct)
    byte last_block[16], last_padding_block[16];
    fin.read((char*)last_block, 16);
    padding(last_block, remainder, last_padding_block); // padding last block
    for (int i=0; i<16; i++)
        state[i] = last_padding_block[i] ^ ct[i]; // this ct is previous one

    AES32_Enc(state, key, ct); // 마지막(패딩된) 블록 AES 암호화
    fout.write((char*)ct, 16);

    fin.close();
    fout.close();

}

/* ====================================================
 * Function : AESCBC_DEC
 * argument : 
 *              - const char *input_file  : ct값을 불러올 파일
 *              - const char *output_file : dec값을 삽입할 파일
 * ==================================================== */
void AESCBC_DEC(const char *input_file, const char *output_file) {
    ifstream fin;
    ofstream fout;

    // Let KEY = 00000000000000000000000000000000, iv = 00000000000000000000000000000000 (CBC ENC와 동일하게)
    byte key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
    byte iv[16]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // open ciphertext file
    fin.open(input_file, ios_base::binary);

    if ( fin.fail() ) {
        cout << "Input(Ciphertext) File Open Error!" << endl;
        return;
    }

    // open file to save decrypted text
    fout.open(output_file, ios_base::binary);
    if ( fout.fail() ) {
        cout << "Output File Open Error!" << endl;
        return;
    }

    int file_size = fileSize(input_file); // 암호문 크기 (바이트)
    int num_block = file_size / 16; // 암호문 블록 개수
    int remainder = file_size % 16; // 이 값이 0이어야 암호문의 크기(바이트)가 16의 배수
    if ( remainder != 0 ) {
        cout << "Ciphertext Size Error!" << endl;
        return;
    }

    // 암호문을 읽어서 CBC 모드로 복호화
    byte prev_ct[16], ct[16], state[16], dec[16];

    // CBC모드 첫 번 째 복호화 : PT1 = Dec(CT1) ^ IV
    fin.read((char*)ct, 16);
    AES8_Dec(ct, key, state);
    for (int i=0; i<16; i++)
        dec[i] = state[i] ^ iv[i];
    fout.write((char*)dec, 16);


    // 두 번 째 ~ 마지막-1 번 째 복호화
    for (int i=1; i<=num_block-2; i++) {
        for (int j=0; j<16; j++)
            prev_ct[j] = ct[j];

        fin.read((char*)ct, 16);
        // CBC모드 복호화 : PT = Dec(CT) ^ prvious_ct
        AES8_Dec(ct, key, state); // 한 블록 AES 복호화

        for (int j=0; j<16; j++)
            dec[j] = state[j] ^ prev_ct[j];
        
        fout.write((char*)dec, 16);
    }

    // previous ciphertext update
    for (int i=0; i<16; i++)
        prev_ct[i] = ct[i];

    // 마지막 블록 복호화
    fin.read((char*)ct, 16);
    AES8_Dec(ct, key, state); // 마지막 블록 AES 복호화

    for (int i=0; i<16; i++)
        dec[i] = state[i] ^ prev_ct[i];
        
    // 마지막 블록 패딩 제거
    int paddingIndex;
    for (paddingIndex=15; paddingIndex>=0; paddingIndex--) {
        if (dec[paddingIndex] == 0x80) {
            break;
        }
    }

    // 패딩을 제거한 마지막 평문(decrypted) write
    if (paddingIndex > 0) {
        byte last_dec_block[paddingIndex];
        for (int i=0; i<paddingIndex; i++)
            last_dec_block[i] = dec[i];
        fout.write((char*)last_dec_block, paddingIndex);
    }



    fin.close();
    fout.close();

}

/* ====================================================
 * Function : gen_NIST_RSP_File
 *             - NIST CBCMMT 중에 사용할 것만 가져와서 새 파일에 저장
 * argument : 
 *              - const char file_name : 생성할 파일 이름
 * ==================================================== */
void gen_NIST_RSP_File(const char *file_name) {
    // NIST AES CBCMMT (COUNT=2) Test Vector
    byte KEY[16]         = { 0x07, 0x00, 0xd6, 0x03, 0xa1, 0xc5, 0x14, 0xe4, 0x6b, 0x61, 0x91, 0xba, 0x43, 0x0a, 0x3a, 0x0c };
    byte IV[16]          = { 0xaa, 0xd1, 0x58, 0x3c, 0xd9, 0x13, 0x65, 0xe3, 0xbb, 0x2f, 0x0c, 0x34, 0x30, 0xd0, 0x65, 0xbb };
    byte PLAINTEXT1[16]  = { 0x06, 0x8b, 0x25, 0xc7, 0xbf, 0xb1, 0xf8, 0xbd, 0xd4, 0xcf, 0xc9, 0x08, 0xf6, 0x9d, 0xff, 0xc5 };
    byte PLAINTEXT2[16]  = { 0xdd, 0xc7, 0x26, 0xa1, 0x97, 0xf0, 0xe5, 0xf7, 0x20, 0xf7, 0x30, 0x39, 0x32, 0x79, 0xbe, 0x91 };
    byte CIPHERTEXT1[16] = { 0xc4, 0xdc, 0x61, 0xd9, 0x72, 0x59, 0x67, 0xa3, 0x02, 0x01, 0x04, 0xa9, 0x73, 0x8f, 0x23, 0x86 };
    byte CIPHERTEXT2[16] = { 0x85, 0x27, 0xce, 0x83, 0x9a, 0xab, 0x17, 0x52, 0xfd, 0x8b, 0xdb, 0x95, 0xa8, 0x2c, 0x4d, 0x00 };

    ofstream fout;

    fout.open(file_name, ios_base::binary);
    if ( fout.fail() ) {
        cout << "MMT REQ File Open Error!" << endl;
        return;
    }

    fout.write("KEY = ", 6);
    fout.write((char*)KEY, 16);

    fout.write("PLAINTEXT = ", 12);
    fout.write((char*)PLAINTEXT1, 16);
    fout.write((char*)PLAINTEXT2, 16);

    fout.write("IV = ", 5);
    fout.write((char*)IV, 16);

    fout.write("CIPHERTEXT = ", 13);
    fout.write((char*)CIPHERTEXT1, 16);
    fout.write((char*)CIPHERTEXT2, 16);

    fout.close();

}

/* ====================================================
 * Function : AESCBC_MMT
 * argument : 
 *              - const char *req_file : NIST CBCMMT 값을 불러올 파일
 *              - const char *rsp_file : CBC 모드로 암호화한 결과를 저장할 파일
 * ==================================================== */
void AESCBC_MMT(const char *req_file, const char *rsp_file) {
    ifstream fin;
    fin.open(req_file, ios_base::binary);
    if ( fin.fail() ) {
        cout << "Request File Open Error!" << endl;
        return;
    }

    ofstream fout;
    fout.open(rsp_file, ios_base::binary);
    if ( fout.fail() ) {
        cout << "Response File Open Error!" << endl;
        return;
    }

    // Read request file
    // Read/Write Key, forms : KEY = ~~~
    byte keystr[6];
    byte key[16];
    fin.read((char*)keystr, 6);
    fin.read((char*)key, 16);
    fout.write("KEY = ", 6);
    fout.write((char*)key, 16);

    // Read/Write PlainText, forms : PLAINTEXT = ~~~
    byte plaintextStr[12];
    byte pt1[16], pt2[16];
    fin.read((char*)plaintextStr, 12);
    fin.read((char*)pt1, 16);
    fin.read((char*)pt2, 16);
    fout.write("PLAINTEXT = ", 12);
    fout.write((char*)pt1, 16);
    fout.write((char*)pt2, 16);

    byte ivstr[5];
    byte iv[16];
    fin.read((char*)ivstr, 5);
    fin.read((char*)iv, 16);
    fout.write("IV = ", 5);
    fout.write((char*)iv, 16);
    

    // AES CBC MMT ENC using given KEY, IV, PT1, PT2
    byte ct1[16], ct2[16];
    
    // CT1 = ENC(IV ^ PT)
    byte state1[16] = { 0x00, };
    for (int i=0; i<16; i++) {
        state1[i] = pt1[i] ^ iv[i];
    }
    AES32_Enc(state1, key, ct1);


    // CT2 = ENC(PT2 ^ CT1)
    byte state2[16] = { 0x00, };
    for (int i=0; i<16; i++) {
        state2[i] = pt2[i] ^ ct1[i];
    }
    AES32_Enc(state2, key, ct2);

    // write ct to my_CBCMMT_rsp
    fout.write("CIPHERTEXT = ", 13);
    fout.write((char*)ct1, 16);
    fout.write((char*)ct2, 16);

    fin.close();
    fout.close();
}

/* ====================================================
 * Function : insert_dummy
 * argument : 
 *              - const char *src : 복사할 파일 (ct)
 *              - const char *dst : 복사한 파일을 1바이트 변경하여 씌워서 넣을 파일 (ct with dummy)
 * ==================================================== */
void insert_dummy(const char *src, const char *dst) { 
    ifstream fin;
    ofstream fout;
    
    fin.open(src, ios_base::binary);
    if (fin.fail()) {
        cout << "Input File Open Error" << endl;
        return;
    }

    fout.open(dst, ios_base::binary);
    if (fout.fail()) {
        cout << "Output File Open Error" << endl;
        return;
    }

    char ch;
    
    // 첫 번 째 바이트 변경
    fin.read(&ch, 1);
    if (ch != '1') {
        fout.write("1", 1);
    }
    else {
        fout.write("0", 1);
    }

    // 두 번 째 바이트부터 카피
    while ( fin.read(&ch, 1) ) { // 버퍼 주소, 데이터 길이
        fout.write(&ch, 1);
    }    
    fin.close();
    fout.close();
}

int main() {

    // Create input(plaintext) file
    const char *pt_file = "pt.bin"; // pt file name
    gen_binfile(pt_file);

    const char *ecb_enc_file = "ecb_enc.bin"; // ecb enc file name
    const char *ecb_dec_file = "ecb_dec.bin"; // ecb dec file name

    //  AES ECB encryption
    AESECB_ENC(pt_file, ecb_enc_file);
    //  AES ECB decryption
    AESECB_DEC(ecb_enc_file, ecb_dec_file);


    const char *cbc_enc_file = "cbc_enc.bin"; // cbc enc file name
    const char *cbc_dec_file = "cbc_dec.bin"; // cbc dec file name

    // AES CBC encryption
    AESCBC_ENC(pt_file, cbc_enc_file);
    // AES CBC decryption
    AESCBC_DEC(cbc_enc_file, cbc_dec_file);


    // NIST CBCMMT128.rsp file
    const char *CBCMMT128    = "CBCMMT128.rsp";
    // gen NIST CBCMMT128.rsp file
    const char *cbc_rsp_file = "AESCBC_MMT.rsp";
    gen_NIST_RSP_File(CBCMMT128);
    // AES CBC MMT Test
    AESCBC_MMT(CBCMMT128, cbc_rsp_file);


    // ecb output file을 한 바이트를 변경하면서 ecb_output_dummyfile로 복사한다.
    const char *ecb_enc_dummyfile = "ecb_enc_dummy.bin";
    const char *ecb_dummy_dec_file = "ecb_dummy_dec_.bin";

    // file copy and insert 1 byte dummy
    insert_dummy(ecb_enc_file, ecb_enc_dummyfile);
    // 더미 파일을 복호화한 후 결과 확인
    AESECB_DEC(ecb_enc_dummyfile, ecb_dummy_dec_file);


    // cbc output file을 한 바이트를 변경하면서 cbc_output_dummyfile로 복사한다.
    const char *cbc_enc_dummyfile = "cbc_enc_dummy.bin";
    const char *cbc_dummy_dec_file = "cbc_dummy_dec_.bin";
    // file copy and insert 1 byte dummy
    insert_dummy(cbc_enc_file, cbc_enc_dummyfile);
    // 더미 파일을 복호화한 후 결과 확인
    AESCBC_DEC(cbc_enc_dummyfile, cbc_dummy_dec_file);


    // ECB 모드로 암호화된 파일의 한 바이트가 변경되면 해당 블록에만 영향을 끼친다.
    // CBC 모드로 암호화된 파일의 한 바이트가 변경되면 해당 블록과 다음 한바이트에 영향을 끼친다.


}
