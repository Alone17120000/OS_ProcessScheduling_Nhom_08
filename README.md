# 🚀 CPU & Resource Scheduling Simulator (Nhóm 08)

## 📖 Giới thiệu
Đây là chương trình mô phỏng các thuật toán định thời CPU và Tài nguyên (Resource) trong Hệ điều hành. Chương trình được thiết kế với cấu trúc chuẩn C/C++, hỗ trợ tính toán chính xác và xuất ra biểu đồ Gantt trực quan.

**Các thuật toán được hỗ trợ:**
1. **FCFS** (First Come First Serve) - *Non-preemptive*
2. **RR** (Round Robin) - *Preemptive theo Quantum*
3. **SJF** (Shortest Job First) - *Non-preemptive*
4. **SRTN** (Shortest Remaining Time Next) - *Preemptive*

---

## 📁 Cấu trúc thư mục
Dự án được phân chia module rõ ràng để dễ bảo trì và nâng cấp:
* `src/` : Chứa toàn bộ mã nguồn (`.c`) của các thuật toán và logic điều phối.
* `include/` : Chứa các file khai báo header (`.h`) và cấu trúc dữ liệu (`types.h`).
* `obj/` : Thư mục tạm chứa các file object (`.o`) trong quá trình biên dịch.
* `bin/` : Chứa file thực thi cuối cùng sau khi build.
* `build.sh` : Script Bash tự động quét và biên dịch toàn bộ dự án.

---

## ⚙️ Yêu cầu hệ thống
Để biên dịch và chạy chương trình, máy tính của bạn cần có:
* Trình biên dịch **GCC** (GNU Compiler Collection).
* Môi trường Terminal chạy được bash shell (Linux, macOS, hoặc WSL / Git Bash trên Windows).

---

## 🛠 Hướng dẫn Cài đặt & Biên dịch

**Bước 1:** Mở Terminal và di chuyển vào thư mục gốc của dự án (`projectOS`).

**Bước 2:** Cấp quyền thực thi cho file build script (Chỉ cần làm lần đầu tiên):
```bash
chmod +x build.sh
Bước 3: Chạy script biên dịch hệ thống:


./build.sh
Script sẽ tự động dọn dẹp, biên dịch từng module và liên kết chúng lại. Nếu thấy thông báo ✅ Build completed successfully!, bạn đã sẵn sàng.

▶️ Hướng dẫn Sử dụng
Khởi động chương trình bằng lệnh sau (đứng từ thư mục projectOS):


./bin/scheduler
Chương trình cung cấp giao diện Menu tương tác (CLI Interactive). Bạn chỉ cần làm theo các bước hướng dẫn trên màn hình:

Nhập phím số [1-4] để chọn Thuật toán định thời.

Nhập 1 để sử dụng file dữ liệu mẫu (input_test.txt) hoặc 2 để nhập tên file của riêng bạn.

Nếu chọn Round Robin (RR), hệ thống sẽ yêu cầu bạn nhập thêm giá trị Quantum.

Xem kết quả Biểu đồ Gantt được in trực tiếp ra Terminal, đồng thời một file báo cáo chi tiết (VD: output_srtn.txt) sẽ được tạo ra trong thư mục hiện tại.

Lưu ý quan trọng: Đảm bảo file dữ liệu đầu vào (ví dụ input_test.txt) phải nằm cùng cấp với file build.sh trước khi chạy chương trình.
