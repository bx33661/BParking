#include "parking.h"

void printMenu() {
    printf("\n===== 停车场管理系统 =====\n");
    printf("1. 车辆进入 (输入: 1 车牌号)\n");
    printf("2. 车辆离开 (输入: 2 车牌号)\n");
    printf("3. 显示停车场状态\n");
    printf("0. 退出系统\n");
    printf("请输入操作命令: ");
}

// 清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 安全输入函数
int safeInputInt() {
    int value;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // 检查是否为数字
        if (sscanf(buffer, "%d", &value) == 1) {
            return value;
        }
    }
    
    return -1; // 返回错误值
}

int main() {
    ParkingStack parkingLot, tempLot;
    WaitingQueue waitingLane;
    int choice, carNumber;
    char buffer[100];
    
    // 初始化停车场和便道
    initStack(&parkingLot);
    initStack(&tempLot);
    initQueue(&waitingLane);
    
    printf("欢迎使用停车场管理系统！\n");
    printf("停车场容量: %d 辆车\n", STACKSIZE);
    
    while (1) {
        printMenu();
        
        // 使用安全输入函数
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("输入错误，请重新输入！\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("输入必须是数字，请重新输入！\n");
            continue;
        }
        
        if (choice <= 0) {
            printf("系统退出，谢谢使用！\n");
            break;
        }
        
        switch (choice) {
            case 1: // 车辆进入
                printf("请输入进入车辆的车牌号: ");
                
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("输入错误，请重新输入！\n");
                    break;
                }
                
                if (sscanf(buffer, "%d", &carNumber) != 1) {
                    printf("车牌号必须是数字，请重新输入！\n");
                    break;
                }
                
                if (carNumber <= 0) {
                    printf("车牌号无效，请重新输入！\n");
                    break;
                }
                
                parkCar(&parkingLot, &waitingLane, carNumber);
                break;
                
            case 2: // 车辆离开
                printf("请输入离开车辆的车牌号: ");
                
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("输入错误，请重新输入！\n");
                    break;
                }
                
                if (sscanf(buffer, "%d", &carNumber) != 1) {
                    printf("车牌号必须是数字，请重新输入！\n");
                    break;
                }
                
                if (carNumber <= 0) {
                    printf("车牌号无效，请重新输入！\n");
                    break;
                }
                
                leaveCar(&parkingLot, &tempLot, &waitingLane, carNumber);
                break;
                
            case 3: // 显示停车场状态
                displayParkingStatus(&parkingLot, &waitingLane);
                break;
                
            default:
                printf("无效的操作命令，请重新输入！\n");
        }
    }
    
    // 释放便道队列中的内存
    clearQueue(&waitingLane);
    return 0;
}
