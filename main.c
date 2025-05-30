#include "parking.h"
#include "colors.h"

// 打印菜单
void printMenu() {
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║              %s停车场管理系统 - 主菜单%s%s                          ║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_YELLOW, COLOR_CYAN, STYLE_BOLD, COLOR_RESET);
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s1.%s 车辆进入停车场                                            %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_GREEN, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s2.%s 车辆离开停车场                                            %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_GREEN, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s3.%s 显示停车场当前状态                                        %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_GREEN, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s4.%s 显示系统统计信息                                          %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_GREEN, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s5.%s 保存系统状态                                              %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_GREEN, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s║%s  %s0.%s 退出系统                                                  %s%s║%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET, COLOR_RED, COLOR_BRIGHT_WHITE, STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("%s请输入操作命令 [0-5]:%s ", COLOR_YELLOW, COLOR_RESET);
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
        printf("\n%s%s⚠️ 输入错误，请重新输入！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return -1;
    }
    
    // 检查输入是否为空
    if (buffer[0] == '\n') {
        printf("\n%s%s⚠️ 输入不能为空，请重新输入！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return -1;
    }
    
    // 尝试解析数字
    if (sscanf(buffer, "%d", &choice) != 1) {
        printf("\n%s%s⚠️ 输入必须是数字，请重新输入！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return -1;
    }
    
    // 检查范围
    if (choice < 0 || choice > 5) {
        printf("\n%s%s⚠️ 无效的选择，请输入 0-5 之间的数字！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return -1;
    }
    
    return choice;
}

// 获取车牌号
char* getPlateNumber(char *buffer, size_t size) {
    printf("%s请输入车牌号 (例如: 京A12345):%s ", COLOR_CYAN, COLOR_RESET);
    
    if (fgets(buffer, size, stdin) == NULL) {
        printf("\n%s%s⚠️ 输入错误！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return NULL;
    }
    
    // 移除换行符
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    // 检查是否为空
    if (strlen(buffer) == 0) {
        printf("\n%s%s⚠️ 车牌号不能为空！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        return NULL;
    }
    
    // 验证车牌号格式
    if (!isValidPlateNumber(buffer)) {
        printf("\n%s%s⚠️ 无效的车牌号格式！请使用正确的车牌号格式（例如: 京A12345）%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
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
        printf("\n%s%s✅ 成功加载之前的系统状态！%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    } else {
        printf("\n%s%s🆕 初始化新的停车场系统！%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    }
    
    // 显示欢迎标题
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s║%s     %s%s🚗 欢迎使用BParking停车场管理系统 v2.0！%s     %s%s             ║%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s║%s     %s📊 停车场容量: %s%d%s 辆车%s                        %s%s            ║%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, STACKSIZE, COLOR_CYAN, COLOR_RESET, STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    
    // 主循环
    while (1) {
        printMenu();
        
        int choice = getMenuChoice();
        if (choice < 0) continue; // 输入错误，重新显示菜单
        
        if (choice == 0) {
            printf("\n%s%s👋 系统退出，谢谢使用！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
            break;
        }
        
        switch (choice) {
            case 1: // 车辆进入
                if (getPlateNumber(plateBuffer, sizeof(plateBuffer)) != NULL) {
                    result = parkCar(&parkingLot, &waitingLane, plateBuffer);
                    
                    switch (result) {
                        case SUCCESS:
                            printf("\n%s%s✅ 车辆 %s%s%s %s已成功进入停车场！%s\n", 
                                STYLE_BOLD, COLOR_GREEN, COLOR_BRIGHT_WHITE, plateBuffer, COLOR_GREEN, STYLE_BOLD, COLOR_RESET);
                            break;
                        case ERR_FULL:
                            printf("\n%s%s⚠️ 车辆 %s%s%s %s已进入便道等候！%s\n", 
                                STYLE_BOLD, COLOR_YELLOW, COLOR_BRIGHT_WHITE, plateBuffer, COLOR_YELLOW, STYLE_BOLD, COLOR_RESET);
                            break;
                        case ERR_EXISTS:
                            printf("\n%s%s⚠️ 车牌号 %s%s%s %s已存在于停车场或便道中！%s\n", 
                                STYLE_BOLD, COLOR_YELLOW, COLOR_BRIGHT_WHITE, plateBuffer, COLOR_YELLOW, STYLE_BOLD, COLOR_RESET);
                            break;
                        default:
                            printf("\n%s%s❌ 未知错误，车辆无法进入！%s\n", 
                                STYLE_BOLD, COLOR_RED, COLOR_RESET);
                    }
                }
                break;
                
            case 2: // 车辆离开
                if (getPlateNumber(plateBuffer, sizeof(plateBuffer)) != NULL) {
                    result = leaveCar(&parkingLot, &tempLot, &waitingLane, plateBuffer, &stats);
                    
                    switch (result) {
                        case SUCCESS:
                            printf("\n%s%s✅ 车辆 %s%s%s %s已成功离开停车场！%s\n", 
                                STYLE_BOLD, COLOR_GREEN, COLOR_BRIGHT_WHITE, plateBuffer, COLOR_GREEN, STYLE_BOLD, COLOR_RESET);
                            break;
                        case ERR_NOT_FOUND:
                            printf("\n%s%s⚠️ 车牌号 %s%s%s %s不在停车场中！%s\n", 
                                STYLE_BOLD, COLOR_YELLOW, COLOR_BRIGHT_WHITE, plateBuffer, COLOR_YELLOW, STYLE_BOLD, COLOR_RESET);
                            break;
                        default:
                            printf("\n%s%s❌ 未知错误，车辆无法离开！%s\n", 
                                STYLE_BOLD, COLOR_RED, COLOR_RESET);
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
                printf("\n%s%s✅ 系统状态已保存！%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
                break;
                
            default:
                printf("\n%s%s⚠️ 无效的操作命令，请重新输入！%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
        }
    }
    
    // 保存系统状态并释放资源
    saveSystemState(&parkingLot, &waitingLane, &stats);
    clearQueue(&waitingLane);
    
    return 0;
}
