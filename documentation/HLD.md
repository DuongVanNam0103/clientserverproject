
# High-Level Design (HLD)

## 1. Gioi thieu
Tai lieu nay cung cap cai nhin tong quan ve he thong chat. He thong duoc thiet ke de cho phep nguoi dung dang ky, dang nhap va gui tin nhan. He thong bao gom ba thanh phan chinh: **Client**, **Server** va **Database**.

## 2. Tong quan he thong
He thong hoat dong theo mo hinh client-server, noi client gui cac yeu cau (dang ky, dang nhap, gui tin nhan) toi server, va server xu ly cac yeu cau nay va tuong tac voi co so du lieu. Viec giao tiep giua client va server su dung giao thuc TCP, va du lieu duoc luu tru trong co so du lieu SQLite.

### Cac thanh phan:
1. **Client**:
    - Client la mot ung dung dong lenh cho phep nguoi dung tuong tac voi he thong.
    - Nguoi dung co the thuc hien cac hanh dong sau:
      - Dang ky tai khoan (cung cap ten nguoi dung va mat khau).
      - Dang nhap vao he thong.
      - Gui tin nhan cho nguoi dung khac.
  
2. **Server**:
    - Server nhan cac yeu cau tu client va xu ly chung.
    - Server quan ly viec xac thuc nguoi dung (dang ky va dang nhap).
    - Server cung xu ly viec dinh tuyen tin nhan, dam bao rang tin nhan duoc gui den dung nguoi nhan.

3. **Database**:
    - Co so du lieu luu tru thong tin nguoi dung (ten nguoi dung va mat khau) va cac tin nhan trao doi giua cac nguoi dung.
    - No bao gom hai bang chinh:
      1. **Users**: Luu tru ten nguoi dung va mat khau.
      2. **Messages**: Luu tru cac tin nhan gui giua cac nguoi dung.

## 3. Luong du lieu
Luong du lieu trong he thong hoat dong nhu sau:

1. **Dang ky nguoi dung**: 
    - Client gui yeu cau dang ky voi ten nguoi dung va mat khau toi server.
    - Server kiem tra xem ten nguoi dung da ton tai chua. Neu chua, server luu nguoi dung moi vao bang **Users** trong co so du lieu.

2. **Dang nhap nguoi dung**: 
    - Client gui yeu cau dang nhap voi ten nguoi dung va mat khau toi server.
    - Server kiem tra xem thong tin dang nhap co khop voi du lieu trong he thong hay khong. Neu dung, nguoi dung se duoc dang nhap.

3. **Gui tin nhan**:
    - Client gui tin nhan den nguoi dung khac.
    - Server xu ly tin nhan va luu vao bang **Messages**.
    - Tin nhan duoc gui toi nguoi nhan.

## 4. Cong nghe su dung
- **Ngon ngu lap trinh C**: Duoc su dung cho ca ung dung client va server.
- **SQLite**: Dung de luu tru du lieu nguoi dung va tin nhan.
- **Giao thuc TCP**: De giao tiep giua client va server.
