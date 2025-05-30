#include "parking.h"
#include "colors.h"

// 初始化停车场栈
void initStack(ParkingStack *stack) {
    stack->top = -1;
}

// 检查栈是否为空
bool isStackEmpty(ParkingStack *stack) {
    return stack->top == -1;
}

// 检查栈是否已满
bool isStackFull(ParkingStack *stack) {
    return stack->top == STACKSIZE - 1;
}

// 入栈操作
int push(ParkingStack *stack, Car car) {
    if (isStackFull(stack)) {
        return ERR_FULL; // 栈满，无法入栈
    }
    stack->data[++(stack->top)] = car;
    return SUCCESS; // 入栈成功
}

// 出栈操作
Car pop(ParkingStack *stack) {
    Car emptyCar = {0};
    if (isStackEmpty(stack)) {
        return emptyCar; // 栈空，返回空车
    }
    return stack->data[(stack->top)--];
}

// 显示停车场栈内容
void displayStack(ParkingStack *stack) {
    printf("停车场内车辆（从北到南）：\n");
    if (isStackEmpty(stack)) {
        printf("停车场内没有车辆\n");
        return;
    }
    
    for (int i = 0; i <= stack->top; i++) {
        printf("位置 %d: 车牌号 %s\n", i + 1, stack->data[i].plateNumber);
    }
}

// 初始化便道队列
void initQueue(WaitingQueue *queue) {
    queue->front = queue->rear = NULL;
    queue->count = 0;
}

// 检查队列是否为空
bool isQueueEmpty(WaitingQueue *queue) {
    return queue->front == NULL;
}

// 获取队列中的车辆数量
int getQueueCount(WaitingQueue *queue) {
    return queue->count;
}

// 入队操作
int enqueue(WaitingQueue *queue, Car car) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return ERR_MEMORY;
    }
    
    newNode->car = car;
    newNode->next = NULL;
    
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    
    queue->count++;
    return SUCCESS;
}

// 出队操作
Car dequeue(WaitingQueue *queue) {
    Car emptyCar = {0};
    if (isQueueEmpty(queue)) {
        return emptyCar; // 队列为空，返回空车
    }
    
    QueueNode *temp = queue->front;
    Car car = temp->car;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->count--;
    return car;
}

// 清空队列并释放所有内存
void clearQueue(WaitingQueue *queue) {
    if (queue == NULL) {
        return;
    }
    
    // 优化：直接遍历并释放所有节点，而不是通过dequeue函数
    // 这样可以减少函数调用开销，提高效率
    QueueNode *current = queue->front;
    QueueNode *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    // 重置队列状态
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
}

// 显示便道队列内容
void displayQueue(WaitingQueue *queue) {
    printf("便道等候车辆：\n");
    if (isQueueEmpty(queue)) {
        printf("便道上没有等候车辆\n");
        return;
    }
    
    QueueNode *current = queue->front;
    int position = 1;
    
    while (current != NULL) {
        printf("位置 %d: 车牌号 %s\n", position++, current->car.plateNumber);
        current = current->next;
    }
}

// 创建车辆
Car createCar(const char *plateNumber) {
    Car newCar;
    strncpy(newCar.plateNumber, plateNumber, MAX_PLATE_LEN - 1);
    newCar.plateNumber[MAX_PLATE_LEN - 1] = '\0'; // 确保结尾
    newCar.arriveTime = time(NULL);
    newCar.leaveTime = 0;
    return newCar;
}

// 验证车牌号格式
bool isValidPlateNumber(const char *plateNumber) {
    // 检查长度
    size_t len = strlen(plateNumber);
    if (len < 7 || len > MAX_PLATE_LEN - 1) {
        return false;
    }
    
    // 中国车牌号格式验证
    // 标准格式：一个汉字省份简称（如京、沪、粤等）+ 一个字母 + 5个字符（字母或数字）
    
    // 检查第一个字符是否为中文省份简称
    // 使用UTF-8编码检查首个字符是否为中文字符
    unsigned char firstByte = (unsigned char)plateNumber[0];
    if ((firstByte & 0x80) == 0) { // 不是多字节字符
        return false;
    }
    
    // 检查第二个字符开始是否为大写英文字母
    // 首先跳过第一个中文字符（UTF-8中文字符通常是3个字节）
    int offset = 0;
    if ((firstByte & 0xE0) == 0xC0) { // 2字节字符
        offset = 2;
    } else if ((firstByte & 0xF0) == 0xE0) { // 3字节字符（大多数中文）
        offset = 3;
    } else if ((firstByte & 0xF8) == 0xF0) { // 4字节字符
        offset = 4;
    } else {
        return false; // 无效的UTF-8字符
    }
    
    // 检查第二个字符（省份简称后的字符）是否为大写英文字母 A-Z
    if (offset >= len || plateNumber[offset] < 'A' || plateNumber[offset] > 'Z') {
        return false;
    }
    
    // 检查剩余字符是否为字母或数字
    for (size_t i = offset + 1; i < len; i++) {
        char c = plateNumber[i];
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }
    }
    
    // 检查总长度是否符合要求（省份简称 + 字母 + 5个字符）
    int remainingChars = len - offset - 1; // 减去省份简称和地区字母
    if (remainingChars < 5) {
        return false;
    }
    
    return true;
}

// 比较两个车牌号
bool comparePlateNumbers(const char *plate1, const char *plate2) {
    // 首先检查指针是否为空
    if (plate1 == NULL || plate2 == NULL) {
        return false;
    }
    
    // 快速检查长度是否一致（可以提前判断不相等的情况）
    size_t len1 = strlen(plate1);
    size_t len2 = strlen(plate2);
    if (len1 != len2) {
        return false;
    }
    
    // 使用二进制比较确保中文字符完全匹配
    // 这比使用strcmp更安全，因为它会比较每个字节
    return memcmp(plate1, plate2, len1) == 0;
}

// 计算停车费用
double calculateFee(Car car) {
    if (car.leaveTime == 0 || car.arriveTime == 0) {
        return 0.0;
    }
    
    // 计算停车时间（秒）
    double parkingTime = difftime(car.leaveTime, car.arriveTime);
    
    // 优化：使用静态缓冲区存储时间字符串
    static char arriveTimeStr[30];
    static char leaveTimeStr[30];
    
    // 使用Windows兼容的时间转换函数
    struct tm *arriveInfo = localtime(&car.arriveTime);
    struct tm *leaveInfo = localtime(&car.leaveTime);
    
    strftime(arriveTimeStr, sizeof(arriveTimeStr), "%Y-%m-%d %H:%M:%S", arriveInfo);
    strftime(leaveTimeStr, sizeof(leaveTimeStr), "%Y-%m-%d %H:%M:%S", leaveInfo);
    
    // 优化：使用更高效的时间计算方式
    int totalSeconds = (int)parkingTime;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    // 优化计费算法：将小时数向上取整，更符合停车场收费惯例
    double fee;
    if (minutes > 0 || seconds > 0) {
        fee = (hours + 1) * HOURLY_RATE; // 不足一小时的部分按一小时收费
    } else {
        fee = hours * HOURLY_RATE;
    }
    
    // 使用颜色打印停车费用信息
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s║%s                  %s%s停车费用计算单%s%s                             ║%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET, STYLE_BOLD, COLOR_BRIGHT_WHITE, COLOR_GREEN, STYLE_BOLD, COLOR_RESET);
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    // 车牌号信息
    printf("%s%s║%s %s车牌号:%s %s%-50s%s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, car.plateNumber, COLOR_RESET, 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    // 时间信息
    printf("%s%s║%s %s进入时间:%s %s%-48s%s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, arriveTimeStr, COLOR_RESET, 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s║%s %s离开时间:%s %s%-48s%s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, leaveTimeStr, COLOR_RESET, 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    // 停车时长
    printf("%s%s║%s %s停车时长:%s %s%d 小时 %d 分钟 %d 秒%s%33s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, hours, minutes, seconds, COLOR_RESET, "", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    // 收费信息
    printf("%s%s║%s %s收费标准:%s %s%.2f 元/小时%s%41s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, HOURLY_RATE, COLOR_RESET, "", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s║%s %s应收费用:%s %s%.2f 元%s%44s        %s%s║%s\n", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_YELLOW, fee, COLOR_RESET, "", 
           STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_GREEN, COLOR_RESET);
    
    return fee;
}

// 检查车牌号是否已存在于停车场或便道中
bool isCarExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber) {
    // 检查停车场
    for (int i = 0; i <= parkingLot->top; i++) {
        if (comparePlateNumbers(parkingLot->data[i].plateNumber, plateNumber)) {
            return true; // 车牌号已存在于停车场
        }
    }
    
    // 检查便道
    QueueNode *current = waitingLane->front;
    while (current != NULL) {
        if (comparePlateNumbers(current->car.plateNumber, plateNumber)) {
            return true; // 车牌号已存在于便道
        }
        current = current->next;
    }
    
    return false; // 车牌号不存在
}

// 车辆进入停车场
int parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber) {
    if (isCarExists(parkingLot, waitingLane, plateNumber)) {
        return ERR_EXISTS; // 车牌号已存在
    }
    
    Car newCar = createCar(plateNumber);
    
    if (!isStackFull(parkingLot)) {
        // 停车场有空位，直接进入
        int result = push(parkingLot, newCar);
        if (result == SUCCESS) {
            SystemStats stats = {0}; // 创建一个空的统计信息结构体
            saveSystemState(parkingLot, waitingLane, &stats);
        }
        return result;
    } else {
        // 停车场已满，进入便道等候
        int result = enqueue(waitingLane, newCar);
        if (result == SUCCESS) {
            SystemStats stats = {0}; // 创建一个空的统计信息结构体
            saveSystemState(parkingLot, waitingLane, &stats);
        }
        return result;
    }
}

// 查找车辆在停车场中的位置
int findCarPosition(ParkingStack *parkingLot, const char *plateNumber) {
    if (isStackEmpty(parkingLot)) {
        return -1;
    }
    
    // 优化：从栈顶开始搜索，因为最近停车的车辆更可能离开
    // 这种方式可以减少平均搜索时间
    for (int i = parkingLot->top; i >= 0; i--) {
        if (comparePlateNumbers(parkingLot->data[i].plateNumber, plateNumber)) {
            return i;
        }
    }
    
    return -1; // 未找到
}

// 车辆离开停车场
int leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, const char *plateNumber, SystemStats *stats) {
    if (isStackEmpty(parkingLot)) {
        return ERR_EMPTY; // 停车场为空
    }
    
    // 首先检查车辆是否在停车场内
    int position = findCarPosition(parkingLot, plateNumber);
    
    if (position == -1) {
        return ERR_NOT_FOUND; // 未找到车辆
    }
    
    // 计算需要移动的车辆数量
    int carsToMove = parkingLot->top - position;
    
    // 将车辆上方的车辆移到临时栈
    for (int i = 0; i < carsToMove; i++) {
        Car car = pop(parkingLot);
        push(tempLot, car);
    }
    
    // 移除要离开的车辆
    Car leavingCar = pop(parkingLot);
    leavingCar.leaveTime = time(NULL);
    
    // 计算费用并更新统计信息
    double fee = calculateFee(leavingCar);
    if (stats != NULL) {
        stats->totalCars++;
        stats->totalRevenue += fee;
    }
    
    // 将临时栈中的车辆移回停车场
    while (!isStackEmpty(tempLot)) {
        Car car = pop(tempLot);
        push(parkingLot, car);
    }
    
    // 如果便道上有等候的车辆，让其进入停车场
    if (!isQueueEmpty(waitingLane) && !isStackFull(parkingLot)) {
        Car waitingCar = dequeue(waitingLane);
        waitingCar.arriveTime = time(NULL); // 更新进入停车场的时间
        push(parkingLot, waitingCar);
    }
    
    // 自动保存系统状态
    saveSystemState(parkingLot, waitingLane, stats);
    
    return SUCCESS;
}

// 显示停车场状态
void displayParkingStatus(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s║%s                %s%s停车场管理系统当前状态%s%s                         ║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, STYLE_BOLD, COLOR_BRIGHT_WHITE, COLOR_BLUE, STYLE_BOLD, COLOR_RESET);
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s║%s %s时间:%s %-52s    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, timeStr, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s║%s %s停车场容量:%s %-46d    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, STACKSIZE, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    
    // 车辆数量信息显示
    int currentCars = parkingLot->top + 1;
    int remainingSpaces = STACKSIZE - currentCars;
    int waitingCars = getQueueCount(waitingLane);
    
    printf("%s%s║%s %s停车场当前车辆数:%s %-40d    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, currentCars, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s║%s %s停车场剩余空位:%s %-42d    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, remainingSpaces, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    printf("%s%s║%s %s便道等候车辆数:%s %-42d    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, waitingCars, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    
    if (stats != NULL) {
        double runTime = difftime(now, stats->startTime) / 3600.0; // 运行时间（小时）
        printf("%s%s║%s %s系统运行时间:%s %.1f 小时%39s %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, runTime, "", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
        printf("%s%s║%s %s总处理车辆数:%s %-44d    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, stats->totalCars, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
        printf("%s%s║%s %s总收入:%s %-50.2f    %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_CYAN, COLOR_BRIGHT_WHITE, stats->totalRevenue, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    }
    
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    
    // 显示停车场内车辆
    printf("%s%s║%s %s停车场内车辆（从北到南）:%s                                     %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    if (isStackEmpty(parkingLot)) {
        printf("%s%s║%s %s停车场内没有车辆%s                                            %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_BRIGHT_WHITE, COLOR_RESET, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    } else {
        for (int i = 0; i <= parkingLot->top; i++) {
            printf("%s%s║%s %s位置 %2d:%s %s车牌号%s %s%-42s%s    %s%s║%s\n", 
                   STYLE_BOLD, COLOR_BLUE, COLOR_RESET, 
                   COLOR_GREEN, i + 1, COLOR_RESET, 
                   COLOR_YELLOW, COLOR_RESET, 
                   COLOR_BRIGHT_WHITE, parkingLot->data[i].plateNumber, COLOR_RESET, 
                   STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
        }
    }
    
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    
    // 显示便道等候车辆
    printf("%s%s║%s %s便道等候车辆:%s                                                 %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    if (isQueueEmpty(waitingLane)) {
        printf("%s%s║%s %s便道上没有等候车辆%s                                            %s%s║%s\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET, COLOR_BRIGHT_WHITE, COLOR_RESET, STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
    } else {
        QueueNode *current = waitingLane->front;
        int position = 1;
        while (current != NULL) {
            printf("%s%s║%s %s位置 %2d:%s %s车牌号%s %s%-46s%s    %s%s║%s\n", 
                   STYLE_BOLD, COLOR_BLUE, COLOR_RESET, 
                   COLOR_GREEN, position++, COLOR_RESET, 
                   COLOR_YELLOW, COLOR_RESET, 
                   COLOR_BRIGHT_WHITE, current->car.plateNumber, COLOR_RESET, 
                   STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
            current = current->next;
        }
    }
    
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n\n", STYLE_BOLD, COLOR_BLUE, COLOR_RESET);
}

// 初始化系统
void initSystem(SystemConfig *config, SystemStats *stats) {
    if (config != NULL) {
        config->parkingCapacity = STACKSIZE;
        config->hourlyRate = HOURLY_RATE;
        config->debugMode = false;
    }
    
    if (stats != NULL) {
        stats->totalCars = 0;
        stats->totalRevenue = 0.0;
        stats->startTime = time(NULL);
    }
}

// 显示系统统计信息
void displaySystemStats(SystemStats *stats) {
    if (stats == NULL) {
        printf("%s%s统计信息不可用%s\n", STYLE_BOLD, COLOR_RED, COLOR_RESET);
        return;
    }
    
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char currentTimeStr[30];
    strftime(currentTimeStr, sizeof(currentTimeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    struct tm *startTimeInfo = localtime(&stats->startTime);
    char startTimeStr[30];
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", startTimeInfo);
    
    // 计算系统运行时间
    double runningTime = difftime(now, stats->startTime);
    int days = (int)(runningTime / 86400); // 86400 秒 = 1 天
    int hours = (int)((runningTime - days * 86400) / 3600);
    int minutes = (int)((runningTime - days * 86400 - hours * 3600) / 60);
    
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s║%s                %s%s停车场管理系统统计信息%s%s                         ║%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, STYLE_BOLD, COLOR_BRIGHT_WHITE, COLOR_MAGENTA, STYLE_BOLD, COLOR_RESET);
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    
    // 时间信息
    printf("%s%s║%s %s当前时间:%s %s%-48s%s    %s%s║%s\n", 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, currentTimeStr, COLOR_RESET, 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s║%s %s系统启动时间:%s %s%-44s%s    %s%s║%s\n", 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, startTimeStr, COLOR_RESET, 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    
    // 运行时间
    printf("%s%s║%s %s系统运行时间:%s %s%d 天 %d 小时 %d 分钟%33s%s%s║%s\n", 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, days, hours, minutes, COLOR_RESET, 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    
    // 车辆和收入统计
    printf("%s%s║%s %s总处理车辆数:%s %s%-44d%s    %s%s║%s\n",  
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, stats->totalCars, COLOR_RESET, 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    printf("%s%s║%s %s总收入:%s %s%-50.2f%s    %s%s║%s\n", 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, stats->totalRevenue, COLOR_RESET, 
           STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    
    // 计算平均每小时收入
    double hoursRunning = runningTime / 3600.0;
    if (hoursRunning > 0) {
        double hourlyAverage = stats->totalRevenue / hoursRunning;
        printf("%s%s║%s %s平均每小时收入:%s %s%-42.2f%s    %s%s║%s\n", 
               STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET, 
               COLOR_CYAN, COLOR_RESET, 
               COLOR_BRIGHT_WHITE, hourlyAverage, COLOR_RESET, 
               STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
    }
    
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n\n", STYLE_BOLD, COLOR_MAGENTA, COLOR_RESET);
}

// 保存系统状态到文件
void saveSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    FILE *file = fopen("parking_state.dat", "wb");
    if (file == NULL) {
        printf("无法创建保存文件！\n");
        return;
    }
    
    // 保存统计信息
    if (stats != NULL) {
        fwrite(stats, sizeof(SystemStats), 1, file);
    }
    
    // 保存停车场信息
    if (parkingLot != NULL) {
        // 保存停车场车辆数量
        int carCount = parkingLot->top + 1;
        fwrite(&carCount, sizeof(int), 1, file);
        
        // 保存停车场中的车辆
        for (int i = 0; i < carCount; i++) {
            fwrite(&parkingLot->data[i], sizeof(Car), 1, file);
        }
    }
    
    // 保存便道信息
    if (waitingLane != NULL) {
        // 保存便道车辆数量
        int queueCount = getQueueCount(waitingLane);
        fwrite(&queueCount, sizeof(int), 1, file);
        
        // 保存便道中的车辆
        QueueNode *current = waitingLane->front;
        while (current != NULL) {
            fwrite(&current->car, sizeof(Car), 1, file);
            current = current->next;
        }
    }
    
    fclose(file);
}

// 从文件加载系统状态
bool loadSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    FILE *file = fopen("parking_state.dat", "rb");
    if (file == NULL) {
        return false; // 文件不存在或无法打开
    }
    
    // 清空当前状态
    initStack(parkingLot);
    clearQueue(waitingLane);
    
    // 加载统计信息
    if (stats != NULL) {
        if (fread(stats, sizeof(SystemStats), 1, file) != 1) {
            fclose(file);
            return false;
        }
    }
    
    // 加载停车场信息
    int carCount;
    if (fread(&carCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }
    
    // 加载停车场中的车辆
    for (int i = 0; i < carCount; i++) {
        Car car;
        if (fread(&car, sizeof(Car), 1, file) != 1) {
            fclose(file);
            return false;
        }
        push(parkingLot, car);
    }
    
    // 加载便道信息
    int queueCount;
    if (fread(&queueCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }
    
    // 加载便道中的车辆
    for (int i = 0; i < queueCount; i++) {
        Car car;
        if (fread(&car, sizeof(Car), 1, file) != 1) {
            fclose(file);
            return false;
        }
        enqueue(waitingLane, car);
    }
    
    fclose(file);
    return true;
}

// 显示帮助信息
void displayHelp() {
    printf("\n%s%s╔═══════════════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s                  %s%s使用帮助说明%s%s                                 ║%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, STYLE_BOLD, COLOR_BRIGHT_WHITE, COLOR_YELLOW, STYLE_BOLD, COLOR_RESET);
    printf("%s%s╠═══════════════════════════════════════════════════════════════╣%s\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    
    // 系统概述
    printf("%s%s║%s %s系统概述:%s                                                     %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s本系统模拟一个狭长的停车场，只有一个出入口。%s                  %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s停车场按照先进后出的栈结构管理，便道按照先进先出的队列管理。%s  %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    
    // 功能说明
    printf("%s%s║%s %s功能说明:%s                                                     %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s1. 车辆进入:%s 输入车牌号，系统会将车辆停入停车场或便道等候。%s   %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s2. 车辆离开:%s 输入车牌号，系统会计算费用并让车辆离开。%s         %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s3. 显示状态:%s 显示停车场和便道中的车辆情况。%s                   %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s4. 统计信息:%s 显示系统运行时间、总车辆数和收入等信息。%s         %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s5. 保存状态:%s 将当前系统状态保存到文件中。%s                     %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s6. 帮助信息:%s 显示本帮助页面。%s                                 %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_GREEN, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s0. 退出系统:%s 退出停车场管理系统。%s                             %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_RED, COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    
    // 车牌号格式
    printf("%s%s║%s %s车牌号格式:%s                                                   %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s中国车牌号由省份汉字+字母+5位字母数字组成，如：琼A12345%s       %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    
    // 收费标准
    printf("%s%s║%s %s收费标准:%s                                                     %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_CYAN, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    printf("%s%s║%s %s每小时%.2f元，不足一小时按一小时计算。%s                       %s%s║%s\n", 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET, 
           COLOR_BRIGHT_WHITE, HOURLY_RATE, COLOR_RESET, 
           STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
    
    printf("%s%s╚═══════════════════════════════════════════════════════════════╝%s\n\n", STYLE_BOLD, COLOR_YELLOW, COLOR_RESET);
}
