import re

def extract_content_between_triple_backticks(text):
    # 使用正则表达式查找位于三个反引号之间的内容
    pattern = r'```(.*?)```'
    
    # re.DOTALL 允许正则表达式匹配换行符
    matches = re.findall(pattern, text, re.DOTALL)
    
    return matches

# 调用函数提取内容
text = """
python
```
python   
def add(x, y):
    return x + y
```
hello world

```
python   
def add(x, y):
    return x + y
```
"""
result = extract_content_between_triple_backticks(text)
print(result)