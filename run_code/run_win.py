import os
import glob

# 运行终端命令并获取输出

pass_counter = 0
fail_counter = 0

base_dir = "C:\\Users\\liyanqi\\Desktop\\judge2024os\\Judge2024OS\\code\\second\\"

current_file_path = os.path.abspath(__file__)
current_directory = os.path.dirname(current_file_path).replace("\\", "/")+'/'

# copy in.txt and in2.txt
os.system(f"cp {base_dir}in.txt {current_directory}")
os.system(f"cp {base_dir}in2.txt {current_directory}")

# A
os.system(f"gcc {base_dir}1-competition.c -o {base_dir}a.exe ")
os.system(f"{base_dir}a.exe")

with open(f'{base_dir}in.txt', 'r') as file:
    line = file.readline()  # 读取第一行
    number_in = int(line.strip())  # 去掉前后的空白符并转换为整数
    file.close()

print("读取的数字in:", number_in)

with open(f'{base_dir}out1.txt', 'r') as file:
    line = file.readline()  # 读取第一行
    number_out = int(line.strip())  # 去掉前后的空白符并转换为整数

print("读取的数字out1:", number_out)

if number_in * 2 >= number_out and number_in < number_out:
    pass_counter += 1
    print("测试A通过")
else:
    fail_counter += 1
    print("测试A失败")

# B1
os.system(f"gcc {base_dir}2-mutex.c -o {base_dir}b1.exe ")
os.system(f"{base_dir}b1.exe")   

with open(f'{base_dir}out2.txt', 'r') as file:
    line = file.readline()  # 读取第一行
    number_out = int(line.strip())  # 去掉前后的空白符并转换为整数

print("读取的数字out2:", number_out)

if number_in * 2 == number_out:
    pass_counter += 1
    print("测试B1通过")
else:
    fail_counter += 1
    print("测试B1失败")


# B2
os.system(f"gcc {base_dir}2-peterson.c -o {base_dir}b2.exe ")
os.system(f"{base_dir}b2.exe")   

with open(f'{base_dir}out22.txt', 'r') as file:
    line = file.readline()  # 读取第一行
    number_out = int(line.strip())  # 去掉前后的空白符并转换为整数

print("读取的数字out2:", number_out)

if number_in * 2 >= number_out and number_in * 2 <= 1.1 * number_out:
    pass_counter += 1
    print("测试B2通过")
else:
    fail_counter += 1
    print("测试B2失败")

# C
os.system(f"gcc {base_dir}3-pv.c -o {base_dir}c.exe ")
os.system(f"{base_dir}c.exe")   

with open(f'{base_dir}out3.txt', 'r') as file:
    line = file.readline()  # 读取第一行
    number_out = int(line.strip())  # 去掉前后的空白符并转换为整数

print("读取的数字out3:", number_out)

if number_in * 2 == number_out:
    pass_counter += 1
    print("测试C通过")
else:
    fail_counter += 1
    print("测试C失败")

# D
os.system(f"gcc {base_dir}4-sem.c -o {base_dir}d.exe ")
os.system(f"{base_dir}d.exe")   

def validate_output(file_name):
    produced_items = []
    consumed_items = []

    with open(base_dir + file_name, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('p'):
                # 提取生产的项目
                produced_items.append(int(line[1:]))
            elif line.startswith('c'):
                # 提取消费的项目
                consumed_items.append(int(line[1:]))
    
    # 验证生产与消费
    if len(produced_items) != len(consumed_items):
        print(f"Error: Number of produced items ({len(produced_items)}) "
              f"does not match number of consumed items ({len(consumed_items)})")
        return False

    # 验证消费的顺序是否与生产一致
    for item in produced_items:
        if item not in consumed_items:
            print(f"Error: Item {item} was produced but never consumed.")
            return False
        consumed_items.remove(item)  # 确保每个项目只被消费一次
    
    print("Output validation successful!")
    print("All produced items were consumed correctly.")
    return True

# 调用验证函数
if validate_output("out4.txt") == True:
    pass_counter += 1
    print("测试D通过")
else:
    fail_counter += 1
    print("测试D失败")

def delete_files(folder_path):
    # 删除所有 *.exe 文件
    exe_files = glob.glob(os.path.join(folder_path, '*.exe'))
    for exe_file in exe_files:
        try:
            os.remove(exe_file)
            print(f"Deleted: {exe_file}")
        except Exception as e:
            print(f"Error deleting {exe_file}: {e}")

    # 删除所有 out*.txt 文件
    # txt_files = glob.glob(os.path.join(folder_path, 'out*.txt'))
    # for txt_file in txt_files:
    #     try:
    #         os.remove(txt_file)
    #         print(f"Deleted: {txt_file}")
    #     except Exception as e:
    #         print(f"Error deleting {txt_file}: {e}")

delete_files(base_dir)

print("通过测试的数量:", pass_counter)
print("失败的测试数量:", fail_counter)

