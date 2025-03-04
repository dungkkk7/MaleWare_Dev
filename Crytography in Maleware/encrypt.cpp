#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "Advapi.lib")
HCRYPTPROV hProv = 0 ;
HCRYPTKEY hKey  = 0; 

int main(){
    if(CryptAcquireContext(&hProv,NULL,MS_STRONG_PROV,PROV_RSA_FULL,0)){
       printf("CryptAcquireContext success\n");

    }else{
         printf("CryptAcquireContext failed\n");
         return -1; 
    }

    BYTE secretkey[] = "123456789012345678901234";
    DWORD secret_key_length = sizeof(secretkey);
    HCRYPTHASH hHash = 0;

    if(CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)){
    CryptHashData(hHash,secretkey, secret_key_length, 0);

        printf("CryptCreateHash success\n");
    }else{
        printf("CryptCreateHash failed\n");
        return -1;
    }

    if(CryptDeriveKey(hProv,CALG_3DES, hHash, 0, &hKey)){
        printf("CryptDeriveKey success\n");
    }else{
        printf("CryptDeriveKey failed\n");
        return -1;
    }
    BYTE data[] = "Hello World";
    DWORD data_length = sizeof(data);
    BYTE encrypted_data[1024];
    DWORD encrypted_data_length = data_length;
    
    if(CryptEncrypt(hKey,0,TRUE,0,encrypted_data,&encrypted_data_length,sizeof(encrypted_data))){
        printf("CryptEncrypt success\n");   
    }else{
        printf("CryptEncrypt failed\n");
        return -1;
    }

    // free the key and hash handle
    if(hKey){
        CryptDestroyKey(hKey);
        printf("CryptDestroyKey success\n");
    }
    if(hHash){
       CryptDestroyHash(hHash);
       printf("CryptDestroyHash success\n");

    }
    if(hProv){
        CryptReleaseContext(hProv,0);
        printf("CryptReleaseContext success\n");
    }
        
}
