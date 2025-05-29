#include "parking.h"

void printMenu() {
    printf("\n===== 停车场管理系统 =====\n");
    printf("1. 车辆进入 (输入: 1 车牌号)\n");
    printf("2. 车辆离开 (输入: 2 车牌号)\n");
    printf("3. 显示停车场状态\n");
    printf("0. 退出系统\n");
    printf("请输入操作命令: ");
}

int main() {
    ParkingStack parkingLot, tempLot;
    WaitingQueue waitingLane;
    int choice, carNumber;
    
    // 初始化停车场和便道
    initStack(&parkingLot);
    initStack(&tempLot);
    initQueue(&waitingLane);
    
    printf("欢迎使用停车场管理系统！\n");
    printf("停车场容量: %d 辆车\n", STACKSIZE);
    
    while (1) {
        printMenu();
        scanf("%d", &choice);
        
        if (choice <= 0) {
            printf("系统退出，谢谢使用！\n");
            break;
        }
        
        switch (choice) {
            case 1: // 车辆进入
                printf("请输入进入车辆的车牌号: ");
                scanf("%d", &carNumber);
                if (carNumber <= 0) {
                    printf("车牌号无效，请重新输入！\n");
                    break;
                }
                parkCar(&parkingLot, &waitingLane, carNumber);
                break;
                
            case 2: // 车辆离开
                printf("请输入离开车辆的车牌号: ");
                scanf("%d", &carNumber);
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
