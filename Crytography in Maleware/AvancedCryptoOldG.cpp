#include <Windows.h>
#include <Wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#include <iostream>
using namespace std;

// khởi tạo và kết nối tời nhà cung cấp khóa 

// Bước 1: Khởi tạo và kết nối với Nhà cung cấp dịch vụ mã hóa (CSP)
// Nhà cung cấp dịch vụ mã hóa (CSP) là một thư viện trong Windows cung cấp các hàm mã hóa. Malware sử dụng API CryptAcquireContext để kết nối với CSP.
// Ví dụ:
// CryptAcquireContext(&hProv, NULL, MS_STRONG_PROV, PROV_RSA_FULL, 0);
// Ở đây, malware kết nối với "Microsoft Strong Cryptographic Provider" hỗ trợ các thuật toán RSA.
// Phân tích: Trong quá trình phân tích, bạn có thể kiểm tra tham số của CryptAcquireContext để biết CSP nào đang được sử dụng, từ đó suy ra thuật toán có thể liên quan.

// Danh sách các CSP hỗ trợ có thể được tìm thấy trong registry tại:

// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography\Defaults\Provider

// Bước 2: Chuẩn bị khóa
// Có hai cách phổ biến để malware chuẩn bị khóa mã hóa:

// Cách 1: Băm khóa văn bản thuần và tạo khóa phiên
// Malware lấy một khóa văn bản thuần (plaintext key), băm nó bằng thuật toán như MD5 hoặc SHA, rồi tạo ra khóa phiên (session key) để mã hóa.
// Ví dụ:
// CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
// CryptHashData(hHash, secretkey, secretkeylen, 0);
// CryptDeriveKey(hProv, CALG_3DES, hHash, 0, &hKey);
// Giải thích:
// CryptCreateHash: Tạo một đối tượng băm với thuật toán MD5.
// CryptHashData: Băm khóa bí mật (secretkey).
// CryptDeriveKey: Tạo khóa phiên dùng thuật toán 3DES từ giá trị băm.
// Phân tích: Thuật toán mã hóa được chỉ định trong tham số thứ hai của CryptDeriveKey. Một số hằng số phổ biến:
// CALG_DES = 0x6601: Thuật toán DES.
// CALG_3DES = 0x6603: Thuật toán Triple DES.
// CALG_AES = 0x6611: Thuật toán AES.
// CALG_RC4 = 0x6801: Thuật toán RC4.
// CALG_RSA_KEYX = 0xa400: Thuật toán trao đổi khóa RSA.


// Bước 3: Mã hóa hoặc giải mã dữ liệu
// Khi khóa đã sẵn sàng, malware sử dụng:
// CryptEncrypt: Để mã hóa dữ liệu.
// CryptDecrypt: Để giải mã dữ liệu.
// Ví dụ:
// CryptEncrypt(hKey, NULL, 1, 0, ciphertext, &ctlen, sz);
// CryptDecrypt(hKey, NULL, 1, 0, plaintext, &ctlen);
// Phân tích: Từ các lệnh gọi này, bạn có thể xác định vị trí bắt đầu của dữ liệu được mã hóa hoặc giải mã.


#include <Windows.h>
#include <Wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#include <stdio.h>
#include <string.h>

int main(VOID) {
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;

    // Dữ liệu gốc
    BYTE dataToEncrypt[] = "Xin chao tao ten ti";
    DWORD data_length = strlen((char*)dataToEncrypt); // Độ dài thực tế

    // Buffer cho mã hóa và giải mã
    BYTE encrypted_data[256] = { 0 };
    BYTE decrypted_data[256] = { 0 };
    memcpy(encrypted_data, dataToEncrypt, data_length); // Sao chép dữ liệu gốc

    // Bước 1: Kết nối CSP
    if (!CryptAcquireContext(&hProv, NULL, MS_STRONG_PROV, PROV_RSA_FULL, 0)) {
        printf("CryptAcquireContext failed\n");
        return -1;
    }
    printf("CryptAcquireContext success\n");

    // Bước 2: Chuẩn bị khóa
    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        printf("CryptCreateHash failed\n");
        return -1;
    }
    BYTE secretKey[] = "taotenti";
    DWORD secret_key_length = strlen((char*)secretKey);
    if (!CryptHashData(hHash, secretKey, secret_key_length, 0)) {
        printf("CryptHashData failed\n");
        return -1;
    }
    if (!CryptDeriveKey(hProv, CALG_3DES, hHash, 0, &hKey)) {
        printf("CryptDeriveKey failed\n");
        return -1;
    }

    // Bước 3: Mã hóa
    DWORD encrypted_data_length = data_length;
    if (!CryptEncrypt(hKey, 0, TRUE, 0, encrypted_data, &encrypted_data_length, 256)) {
        printf("CryptEncrypt failed\n");
        return -1;
    }
    printf("CryptEncrypt success\n");
    printf("Encrypted data (hex): ");
    for (DWORD i = 0; i < encrypted_data_length; i++) {
        printf("%02x", encrypted_data[i]);
    }
    printf("\n");

    // Bước 4: Giải mã
    memcpy(decrypted_data, encrypted_data, encrypted_data_length);
    DWORD decrypted_data_length = encrypted_data_length;
    if (!CryptDecrypt(hKey, 0, TRUE, 0, decrypted_data, &decrypted_data_length)) {
        printf("CryptDecrypt failed\n");
        return -1;
    }
    printf("CryptDecrypt success\n");
    printf("Decrypted data: %s\n", decrypted_data);

    // Giải phóng tài nguyên
    if (hKey) CryptDestroyKey(hKey);
    if (hHash) CryptDestroyHash(hHash);
    if (hProv) CryptReleaseContext(hProv, 0);

    return 0;
}


