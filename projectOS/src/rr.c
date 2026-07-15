#include "rr.h"

int get_next_rr(Process processes[], int n, int current_time, int quantum, int *time_spent) {
    static int last_idx = -1; // Biến static để nhớ vị trí thằng vừa chạy ở lần trước
    
    // Quét một vòng tròn bắt đầu từ thằng kế tiếp của last_idx
    for (int i = 1; i <= n; i++) {
        int idx = (last_idx + i) % n; // Phép chia lấy dư để xoay vòng về 0 nếu vượt quá n
        
        if (processes[idx].state == 1) {
            last_idx = idx;    // Lưu lại vị trí để lần sau quét tiếp
            *time_spent = 0;   // Reset bộ đếm quantum về 0 cho thằng mới
            return idx;
        }
    }
    return -1; // Không có ai đang đợi
}