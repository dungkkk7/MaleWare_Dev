#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> createArrS() {
    vector<int> S(256);
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
    return S;
}

vector<int> createArrK(const string& key) {
    vector<int> keyArr(256);
    for (int i = 0; i < 256; i++) {
        keyArr[i] = key[i % key.size()];
    }
    return keyArr;
}

void permunateS(vector<int>& S, const vector<int>& K) {
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + K[i]) % 256; // Sửa K[i % 256] thành K[i]
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

string PRNG(vector<int>& S, const string& data) {
    int i = 0;
    int j = 0;
    string encrypt(data.size(), 0); // Khởi tạo encrypt với kích thước của data
    for (size_t n = 0; n < data.size(); n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        int key = S[(S[i] + S[j]) % 256];
        encrypt[n] = data[n] ^ key; // Mã hóa từng ký tự
    }
    return encrypt;
}

int main() {
    vector<int> S = createArrS();
    string key = "taotenti";
    vector<int> Karray = createArrK(key);
    permunateS(S, Karray);
    string data = "Xin chao toi ten ti";
    string encrypt = PRNG(S, data);
    cout << "Encrypted data: ";
    for (char c : encrypt) {
        printf("%02x", static_cast<unsigned char>(c)); // In dạng hex
    }
    cout << endl;
    // Giải mã
    vector<int> S_decrypt = createArrS();
    permunateS(S_decrypt, Karray); // Sử dụng cùng khóa
    string decrypt = PRNG(S_decrypt, encrypt);
    cout << "Dữ liệu giải mã: " << decrypt << endl;
    return 0;
}