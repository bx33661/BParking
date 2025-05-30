#include "parking.h"

// 打印菜单
void printMenu() {
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║              停车场管理系统 - 主菜单                          ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. 车辆进入停车场                                            ║\n");
    printf("║  2. 车辆离开停车场                                            ║\n");
    printf("║  3. 显示停车场当前状态                                        ║\n");
    printf("║  4. 显示系统统计信息                                          ║\n");
    printf("║  5. 保存系统状态                                              ║\n");
    printf("║  0. 退出系统                                                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("请输入操作命令 [0-5]: ");
}

// 清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 获取菜单选择
int getMenuChoice() {
    int choice;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\n⚠️ 输入错误，请重新输入！\n");
        return -1;
    }
    
    // 检查输入是否为空
    if (buffer[0] == '\n') {
        printf("\n⚠️ 输入不能为空，请重新输入！\n");
        return -1;
    }
    
    // 尝试解析数字
    if (sscanf(buffer, "%d", &choice) != 1) {
        printf("\n⚠️ 输入必须是数字，请重新输入！\n");
        return -1;
    }
    
    // 检查范围
    if (choice < 0 || choice > 5) {
        printf("\n⚠️ 无效的选择，请输入 0-5 之间的数字！\n");
        return -1;
    }
    
    return choice;
}

// 获取车牌号
char* getPlateNumber(char *buffer, size_t size) {
    printf("请输入车牌号 (例如: 京A12345): ");
    
    if (fgets(buffer, size, stdin) == NULL) {
        printf("\n⚠️ 输入错误！\n");
        return NULL;
    }
    
    // 移除换行符
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    // 检查是否为空
    if (strlen(buffer) == 0) {
        printf("\n⚠️ 车牌号不能为空！\n");
        return NULL;
    }
    
    // 验证车牌号格式
    if (!isValidPlateNumber(buffer)) {
        printf("\n⚠️ 无效的车牌号格式！请使用正确的车牌号格式（例如: 京A12345）\n");
        return NULL;
    }
    
    return buffer;
}

// 主函数
int main() {
    ParkingStack parkingLot, tempLot;
    WaitingQueue waitingLane;
    SystemConfig config;
    SystemStats stats;
    char plateBuffer[MAX_PLATE_LEN];
    int result;
    
    // 初始化系统
    initSystem(&config, &stats);
    initStack(&parkingLot);
    initStack(&tempLot);
    initQueue(&waitingLane);
    
    // 尝试加载之前的系统状态
    if (loadSystemState(&parkingLot, &waitingLane, &stats)) {
        printf("\n✅ 成功加载之前的系统状态！\n");
    } else {
        printf("\n🆕 初始化新的停车场系统！\n");
    }
    
    printf("\n🚗 欢迎使用BParking管理系统 v2.0！\n");
    printf("📊 停车场容量: %d 辆车\n", STACKSIZE);
    
    // 主循环
    while (1) {
        printMenu();
        
        int choice = getMenuChoice();
        if (choice < 0) continue; // 输入错误，重新显示菜单
        
        if (choice == 0) {
            printf("\n👋 系统退出，谢谢使用！\n");
            break;
        }
        
        switch (choice) {
            case 1: // 车辆进入
                if (getPlateNumber(plateBuffer, sizeof(plateBuffer)) != NULL) {
                    result = parkCar(&parkingLot, &waitingLane, plateBuffer);
                    
                    switch (result) {
                        case SUCCESS:
                            printf("\n✅ 车辆 %s 已成功进入停车场！\n", plateBuffer);
                            break;
                        case ERR_FULL:
                            printf("\n⚠️ 车辆 %s 已进入便道等候！\n", plateBuffer);
                            break;
                        case ERR_EXISTS:
                            printf("\n⚠️ 车牌号 %s 已存在于停车场或便道中！\n", plateBuffer);
                            break;
                        default:
                            printf("\n❌ 未知错误，车辆无法进入！\n");
                    }
                }
                break;
                
            case 2: // 车辆离开
                if (getPlateNumber(plateBuffer, sizeof(plateBuffer)) != NULL) {
                    result = leaveCar(&parkingLot, &tempLot, &waitingLane, plateBuffer, &stats);
                    
                    switch (result) {
                        case SUCCESS:
                            printf("\n✅ 车辆 %s 已成功离开停车场！\n", plateBuffer);
                            break;
                        case ERR_NOT_FOUND:
                            printf("\n⚠️ 车牌号 %s 不在停车场中！\n", plateBuffer);
                            break;
                        default:
                            printf("\n❌ 未知错误，车辆无法离开！\n");
                    }
                }
                break;
                
            case 3: // 显示停车场状态
                displayParkingStatus(&parkingLot, &waitingLane, &stats);
                break;
                
            case 4: // 显示系统统计信息
                displaySystemStats(&stats);
                break;
                
            case 5: // 保存系统状态
                saveSystemState(&parkingLot, &waitingLane, &stats);
                printf("\n✅ 系统状态已保存！\n");
                break;
                
            default:
                printf("\n⚠️ 无效的操作命令，请重新输入！\n");
        }
    }
    
    // 保存系统状态并释放资源
    saveSystemState(&parkingLot, &waitingLane, &stats);
    clearQueue(&waitingLane);
    
    return 0;
}
