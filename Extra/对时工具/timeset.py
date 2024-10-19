import serial
import serial.tools.list_ports
import time

# 查找所有可用的串口
ports = list(serial.tools.list_ports.comports())

# 打印所有找到的串口
print("找到的串口列表：")
for port in ports:
    print(port)

# 检查是否找到了串口
if ports:
    # 获取第一个串口的设备名
    first_port = ports[0].device

    # 尝试打开第一个串口
    try:
        with serial.Serial(first_port, 115200, timeout=1) as ser:
            print(f"已打开串口：{first_port}")

            # 获取当前时间戳
            timestamp = int(time.time())

            # 构建要写入的数据
            data_to_send = f"timeset {timestamp+28800}\n"

            # 写入数据
            ser.write(data_to_send.encode('utf-8'))
            print(f"已写入数据：{data_to_send}")
    except serial.SerialException as e:
        print(f"无法打开串口：{first_port}，错误信息：{e}")
else:
    print("没有找到可用的串口。")
