import re

def extract_results(text):
    # 定义正则表达式，匹配“测试结果”和“原理分析”后面的内容
    pattern = r"\* 测试结果：\s*(.*?)\s*\n\s*\* 原理分析：\s*(.*?)\s*\n"
    match = re.search(pattern, text, re.DOTALL)  # 使用DOTALL模式处理跨行内容
    if match:
        return [match.group(1), match.group(2)]
    return None

# 测试用例
test_string = """
* 测试结果：变量值爱分析原理e呵呵a

* 原理分析：我爱分析原理。。
"""

result = extract_results(test_string)
print(result)
