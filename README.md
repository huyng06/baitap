#include <iostream>

void readArray(int a[], int n) {
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
}

void printArray(int a[], int n) {
    for (int i = 0; i < n; i++) {
        std::cout << a[i];
        if (i < n - 1) std::cout << " "; // In khoảng cách giữa các số
    }
    std::cout << std::endl;
}

int main() {
    int n;
    std::cout << "Enter number of elements: ";
    std::cin >> n;
    
    int a[n]; // Khai báo mảng với n phần tử
    readArray(a, n); // Đọc mảng từ bàn phím
    printArray(a, n); // In mảng ra màn hình
    
    return 0;
}
Bạn đã gửi
