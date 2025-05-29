# 使用cl.exe (MSVC编译器)
# 如果没有MSVC，可以使用其他编译器如MinGW或TDM-GCC

# 使用Visual Studio的cl编译器
all:
	cl.exe /W4 /Fe:parking_system.exe main.c parking.c

# 如果使用MinGW或TDM-GCC (如果已安装)
mingw:
	gcc -Wall -o parking_system.exe main.c parking.c

clean:
	del parking_system.exe *.obj

run:
	parking_system.exe
