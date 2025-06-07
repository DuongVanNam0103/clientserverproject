
# Kien Truc He Thong

## 1. Gioi thieu
Tai lieu nay em mo ta kien truc cua he thong chat. em giai thich cach cac thanh phan client, server va co so du lieu tuong tac voi nhau de thuc hien cac yeu cau dang ky nguoi dung, dang nhap va gui tin nhan.

## 2. So do kien truc

Duoi day la so do kien truc co ban cua he thong chat:


+-------------------+           +------------------+           +---------------+
|                   |           |                  |           |                | |                   |           |                  |           |                | 
|       Client      | <-------> |      Server      |<--------->|   Database     |D                   |           |                  |           |                |
|                   |           |                  |           |                | |                   |           |                  |           |                |
+-------------------+           +------------------+           +-----------------+
  (Ung dung dong lenh)             (Xu ly yeu cau)         (luu user va tin nhan) 
## 3. Tong quan kien truc
Kien truc cua he thong chat dua tren mo hinh client-server, voi ba thanh phan chinh: **Client**, **Server** va **Database**.

### Client:
- **Client** la mot ung dung dong lenh noi nguoi dung co the tuong tac voi he thong. Client gui yeu cau (dang ky, dang nhap, gui tin nhan) toi server. Client cung nhan phan hoi tu server, chan hanh nhu thong bao dang nhap thanh cong hoac loi.

### Server:
- **Server** xu ly cac yeu cau cua client va quan ly viec xac thuc nguoi dung. Server tuong tac voi co so du lieu de luu tru va lay du lieu nguoi dung va tin nhan.

### Database:
- **Database** luu tru thong tin nguoi dung (ten nguoi dung va mat khau) va tin nhan trao doi giua cac nguoi dung.

## 4. Luong giao tiep
1. **Dang ky nguoi dung**:
    - Client gui yeu cau dang ky toi server, bao gom ten nguoi dung va mat khau.
    - Server luu tru thong tin nguoi dung vao bang **Users** neu ten nguoi dung chua ton tai.

2. **Dang nhap nguoi dung**:
    - Client gui yeu cau dang nhap voi ten nguoi dung va mat khau.
    - Server kiem tra thong tin dang nhap voi co so du lieu va phan hoi (thanh cong hoac that bai).

3. **Gui tin nhan**:
    - Client gui tin nhan toi nguoi dung khac.
    - Server luu tin nhan vao bang **Messages** va gui no den nguoi nhan.

## 5. Cong nghe su dung:
- **Ngon ngu lap trinh C**: Dung de trien khai logic cho client va server.
- **SQLite**: Co so du lieu nhe duoc su dung de luu tru du lieu nguoi dung va tin nhan.
- **Giao thuc TCP/IP**: De giao tiep giua client va server.
