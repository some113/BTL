## Báo cáo bài tập lớn
1. **Hướng dẫn cài đặt**  
* Tải code từ git hub về hoặc dùng lệnh  `git clone https://github.com/some113/BTL.git`
* Vào thư mục đã tải và nháy đúp chuột vào file `game.exe`.
---
2. **Mô tả chung về trò chơi**    
* [Video minh hoạ](https://youtu.be/dVorCNzX0mc)  
* Trò chơi cho phép người chơi điều khiển tàu chiến hộ tống đoàn tàu tiếp tế, bảo vệ đoàn tàu khỏi tàu ngầm của quân địch.  
* Người chơi sẽ phải sử dụng bom chìm - một loại vũ khí chống tàu ngầm để tấn công tàu ngầm địch, đồng thời phải điều khiển cẩn thận để né ngư lôi và không va phải hay thả bom chìm trúng tàu đồng minh.  
* Ý tưởng chính:  
    * Dùng các vector để quản lí các đối tượng, mỗi đối tượng khác loại sẽ thuộc một class. 
    * Hành động của một lớp đối tượng được xây dựng trong các hàm.  
    * Trò chơi kết thúc khi toàn bộ tàu hộ tống chìm hoặc toàn bộ tàu ngầm chìm
---
3. **Mô tả chức năng**  
* Chức năng điều khiển tàu:  
    * Điều khiển tốc độ, góc quay của tàu chiến bằng các nút mũi tên.
    * Dùng các phím `Z`, `X`, `C` lần lượt để thả bom chìm từ mạn trái, mạn phải và đuôi tàu.
* Một số chức năng khác:  
    * Cho phép tùy chọn số lượng tàu mỗi loại đầu mỗi màn chơi (ngẫu nhiên nếu người chơi không nhập)
    * Phím `Tab`: chuyển sang tàu tiếp theo để điều khiển.
    * Phím `T`: bỏ khóa/ khóa di chuyển màn hình theo tàu. 
    * Con lăn chuột: zoom to/nhỏ góc nhìn.  
---
4. **Các kĩ thuật lập trình đã sử dụng**  
* Con trỏ.
* Cấu trúc dữ liệu vector class, struct.  
* Tham chiếu.
* Đồ họa.  
---
5. **Kết luận, hướng phát triển**  
* Kết luận: việc viết code sạch, đẹp cùng với việc chia nhỏ code thành các hàm trong lập trình là một điều rất quan trọng cả ở dự án cá nhân và dự án nhiều người; những việc dù rất nhỏ như đặt tên biến đúng cách lại rất cần thiết.
* Hướng phát triển:
    * Cho phép người chơi sắp xếp đội hình lúc bắt đầu
    * Thêm âm thanh vào trò chơi
    * Cải thiện chất lượng hình ảnh
    * Thiết kế thuật toán để bot thông minh hơn
    * Chơi nhiều người để hỗ trợ chiến thuật, chơi PvP
    * Thêm điều kiện thời tiết
    * Thêm một số vũ khí như pháo, súng máy...
