def find_targets(source, targets):
    # 拆分 source 为多行
    lines = source.splitlines()

    # 结果列表
    res = []

    # 遍历目标字符串
    for target in targets:
        found = False
        # 遍历每一行，检测是否包含目标字符串
        for i, line in enumerate(lines, start=1):  # 行号从1开始
            if target in line:
                res.append(i)  # 保存行号
                found = True
                break
        if not found:
            res.append(-1)  # 没有找到该目标时返回 -1

    return res

# 示例输入
source = '''
def hello_world():
     test
    tt
def hello_world():
     test
def good():
      dddd
def heel:
   sss'''

targets = ['hello_world', 'heel', 'ddd']

# 调用函数
res = find_targets(source, targets)
print(res)
