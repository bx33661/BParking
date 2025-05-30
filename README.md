<div align="center">

# 🚗 BParking停车场管理系统

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![Status](https://img.shields.io/badge/Status-Active-success.svg)]()

*基于数据结构课程设计的停车场管理系统*

<img src="./public/logo.png" alt="BParking Logo" width="300"/>

</div>

## 📝 项目概述

BParking是一个基于C语言开发的停车场管理系统，模拟了一个只有一个出入口的狭长停车场的运作流程。系统实现了车辆进出管理、费用计算、状态显示等功能，并采用了栈和队列数据结构来模拟停车场和便道的运作机制。

### ✨ 主要特性

- 🚘 **车辆进出管理**：记录车辆进入和离开停车场的时间和位置
- 💰 **费用计算**：根据停车时长自动计算停车费用
- 📊 **状态显示**：实时显示停车场和便道的车辆状态
- 💾 **状态保存**：支持保存和恢复系统状态
- 🖥️ **友好界面**：提供简洁直观的命令行界面

## 🔧 技术架构

### 数据结构设计

系统使用了以下数据结构：

- **停车场栈（顺序栈）**：用于存储停车场内的车辆，先进后出（LIFO）特性符合停车场的运作方式
- **临时栈**：用于临时存储为离开车辆让路的车辆
- **便道队列（链式队列）**：用于存储等候进入停车场的车辆，先进先出（FIFO）特性符合便道等候的运作方式

### 系统架构图

```bash
┌─────────────────────────────┐
│       用户交互界面          │
└───────────────┬─────────────┘
                ▼
┌─────────────────────────────┐
│       停车场管理系统        │
├─────────────────────────────┤
│  ┌───────────┐ ┌───────────┐│
│  │ 停车场栈  │ │ 便道队列  ││
│  └───────────┘ └───────────┘│
└─────────────────────────────┘
```

## 📂 文件结构

```bash
BParking/
├── main.c         # 主程序，包含用户交互界面
├── parking.c      # 实现文件，包含所有功能函数的实现
├── parking.h      # 头文件，包含数据结构定义和函数声明
├── run.bat        # Windows批处理文件，用于运行程序
├── compile.bat    # Windows批处理文件，用于编译项目
└── README.md      # 项目说明文档
```

## 💻 核心数据结构

### 车辆信息结构体

```c
typedef struct {
    char plateNumber[MAX_PLATE_LEN]; // 车牌号（支持中文字符）
    time_t arriveTime;               // 到达时间
    time_t leaveTime;                // 离开时间
} Car;
```

### 停车场栈结构

```c
typedef struct {
    Car data[STACKSIZE];
    int top;
} ParkingStack;
```

### 便道队列

```c
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;         // 队列中的车辆数量
} WaitingQueue;
```

## 🚀 核心功能

### 1. 车辆进入停车场

```c
int parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber) {
    // 检查车牌号是否已存在
    if (isCarExists(parkingLot, waitingLane, plateNumber)) {
        return ERR_EXISTS; // 车牌号已存在
    }
    
    // 创建新车辆
    Car newCar = createCar(plateNumber);
    
    if (!isStackFull(parkingLot)) {
        // 停车场有空位，直接进入
        return push(parkingLot, newCar);
    } else {
        // 停车场已满，进入便道等候
        return enqueue(waitingLane, newCar);
    }
}
```

### 2. 车辆离开停车场

```c
int leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, const char *plateNumber, SystemStats *stats) {
    // 查找车辆位置
    int position = findCarPosition(parkingLot, plateNumber);
    
    if (position == -1) {
        return ERR_NOT_FOUND; // 车辆不在停车场中
    }
    
    // 将上方车辆移到临时栈
    int carsToMove = parkingLot->top - position;
    for (int i = 0; i < carsToMove; i++) {
        push(tempLot, pop(parkingLot));
    }
    
    // 移除要离开的车辆并计算费用
    Car leavingCar = pop(parkingLot);
    leavingCar.leaveTime = time(NULL);
    double fee = calculateFee(leavingCar);
    
    // 更新统计信息
    stats->totalCars++;
    stats->totalRevenue += fee;
    
    // 将临时栈中的车辆移回停车场
    while (!isStackEmpty(tempLot)) {
        push(parkingLot, pop(tempLot));
    }
    
    // 如果便道有车辆等候，让其进入停车场
    if (!isQueueEmpty(waitingLane)) {
        push(parkingLot, dequeue(waitingLane));
    }
    
    return SUCCESS;
    } else {
        fee = hours * 10.0;
        // 如果有分钟或秒，则多收取一小时费用
        if (minutes > 0 || seconds > 0) {
            fee += 10.0;
        }
    }
    
    printf("\n===== 停车费用计算 =====\n");
    printf("车牌号: %d\n", car.carNumber);
    printf("进入时间: %s\n", arriveTimeStr);
    printf("离开时间: %s\n", leaveTimeStr);
    printf("停车时间: %d小时 %d分钟 %d秒\n", hours, minutes, seconds);
    printf("停车费用: %.2f 元\n", fee);
    printf("==========================\n");
}
```

## 辅助功能详解

### 1. 车牌号查找

函数 `findCarPosition` 用于查找车辆在停车场中的位置：

```c
int findCarPosition(ParkingStack *parkingLot, int carNumber) {
    if (isStackEmpty(parkingLot)) {
        return -1;
    }
    
    for (int i = 0; i <= parkingLot->top; i++) {
        if (parkingLot->data[i].carNumber == carNumber) {
            return i;
        }
    }
    
    return -1;
}
```

### 2. 车牌号重复检查

函数 `isCarNumberExists` 用于检查车牌号是否已存在于停车场或便道中：

```c
int isCarNumberExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, int carNumber) {
    // 检查停车场
    for (int i = 0; i <= parkingLot->top; i++) {
        if (parkingLot->data[i].carNumber == carNumber) {
            return 1; // 车牌号已存在于停车场
        }
    }
    
    // 检查便道
    QueueNode *current = waitingLane->front;
    while (current != NULL) {
        if (current->car.carNumber == carNumber) {
            return 2; // 车牌号已存在于便道
        }
        current = current->next;
    }
    
    return 0; // 车牌号不存在
}
```

### 3. 队列内存清理

函数 `clearQueue` 用于清空队列并释放所有内存：

```c
void clearQueue(WaitingQueue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}
```

## 用户界面

系统提供了简单的文本界面，用户可以通过输入命令和车牌号来操作系统：

1. 车辆进入：输入 `1 车牌号`
2. 车辆离开：输入 `2 车牌号`
3. 显示停车场状态：输入 `3`
4. 退出系统：输入 `0`

```c
void printMenu() {
    printf("\n===== 停车场管理系统 =====\n");
    printf("1. 车辆进入 (输入: 1 车牌号)\n");
    printf("2. 车辆离开 (输入: 2 车牌号)\n");
    printf("3. 显示停车场状态\n");
    printf("0. 退出系统\n");
    printf("请输入操作命令: ");
}
```

## 编译和运行

### 在Windows系统上

1. 双击 `compile.bat` 文件，它会尝试使用不同的编译器来编译项目
2. 如果编译成功，系统会自动运行

### 在其他系统上

使用GCC编译器：

```bash
gcc -Wall -o parking_system main.c parking.c
./parking_system
```

## 系统优化

本系统进行了多项优化，包括：

1. **内存管理优化**：
   - 添加了专门的内存清理函数
   - 优化了内存分配和释放的逻辑

2. **车辆查找逻辑优化**：
   - 添加了专门的车辆查找函数
   - 优化了车辆移动的逻辑，减少不必要的操作

3. **车牌号重复检查**：
   - 防止同一车牌号多次进入系统
   - 提高了系统的健壮性

4. **停车费用计算优化**：
   - 更详细的时间和费用计算
   - 更友好的费用显示

5. **输出信息优化**：
   - 更详细的操作提示
   - 更清晰的错误信息

## 扩展功能建议

系统可以进一步扩展以下功能：

1. **数据持久化**：将停车记录保存到文件中
2. **图形用户界面**：使用图形库实现更友好的界面
3. **更多车辆信息**：添加车型、颜色等信息
4. **复杂计费规则**：实现不同时段不同费率的计费规则
5. **多停车场管理**：扩展为管理多个停车场的系统

## 总结

本停车场管理系统通过栈和队列数据结构，成功模拟了一个特殊停车场的运作流程。系统实现了车辆进入、离开、费用计算等核心功能，并进行了多项优化，使其更加健壮和用户友好。
