import re

# 输入的文本
text = '''
### 4.1 线程竞争

* 测试结果： 变量值<200000

* 原理分析：我爱分析原理
发的发
ff啊的方法 

### 4.2 互斥锁和Peterson方法

#### 4.2.1 互斥锁

* 测试结果： 变量值=200000

* 原理分析：我爱分析原理发到付
发打发打发 

#### 4.2.2 Peterson方法

* 测试结果： 变量值=200000

* 原理分析：我爱分析原理

### 4.3 PV操作

* 测试结果：变量值=200000

* 原理分析：我爱分析原理

### 4.4 生产者消费者问题

* 测试结果： 变量值爱分析原理100000

* 原理分析：我爱分析原理
'''

# 使用正则表达式提取“测试结果”和“原理分析”的内容
pattern = r"测试结果：\s*(.*?)\n.*?原理分析：\s*(.*?)\n"
matches = re.findall(pattern, text, re.DOTALL)

# 输出结果
for i, (test_result, analysis) in enumerate(matches, start=1):
    print(f"测试结果 {i}: {test_result}")
    print(f"原理分析 {i}: {analysis}")
    print()
