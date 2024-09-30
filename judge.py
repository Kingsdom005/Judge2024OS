import yaml
import re
import os
import sys

# 读取markdown文档
print("检索markdown文档中……")

directory = './data/'
md_files = [file for file in os.listdir(directory) if file.endswith('.md')]

print(md_files)
default_read_md_file = md_files[0]  # 默认读取第一个

print(f"检索成功，检索到{default_read_md_file}")

print("正在解析文档结构……")

markdown_content = ""

def get_structure_info(file_name, prefix="./data/"):
    res = {
        "filename": '',
        "l1_title": '',
        "l2_titles": []
    }
    # 读取Markdown文件
    with open(prefix + file_name, 'r', encoding='utf-8') as file:
        global markdown_content
        markdown_content = file.read()

    # 匹配所有一级标题和二级标题
    level1_titles = re.findall(r'^(# .+)', markdown_content, re.MULTILINE)
    level2_titles = re.findall(r'^(## .+)', markdown_content, re.MULTILINE)

    # 输出文件名
    # print(f"文件名: {file_name}\n")
    res["filename"] = file_name

    # 输出一级标题
    # print("一级标题:")
    for title in level1_titles:
        # print(title[2:])  # 去掉"# "
        res["l1_title"] = title[2:]

    # 输出二级标题
    # print("\n二级标题:")
    for title in level2_titles:
        # print(title[3:])  # 去掉"## "
        res["l2_titles"].append(title[3:])

    return res


structure = get_structure_info(default_read_md_file)

# def check_structure():
print("解析文档结构成功")
print(structure)

# read config yml file
print("读取配置文件中……")
with open('config.yml', 'r', encoding="utf8") as file:
    config = yaml.safe_load(file)

print(config)
print("读取成功")

check_structure = {
    "filename": False,
    "structure_l1": False,
    "structure_l2": [False for _ in range(5)]
}

pattern = r'^\d{10}-\w+-操作系统第一次作业\.md$'
if re.match(pattern, structure["filename"]):
    check_structure["filename"] = True

if structure["l1_title"] == config["judgeConfig"]["structure"]["l1_title"]:
    check_structure["structure_l1"] = True

for i in range(len(structure["l2_titles"])):
    if structure["l2_titles"][i] == config["judgeConfig"]["structure"]["l2_titles"][i]:
        check_structure["structure_l2"][i] = True

print("检查markdown结构中……")
print(check_structure)
if not check_structure["filename"]:
    print("请检查文件名！！")
    sys.exit()
if not check_structure["structure_l1"]:
    print("请检查一级标题！！")
    sys.exit()
for l2_subtitle in check_structure["structure_l2"]:
    if not l2_subtitle:
        print("请检查二级标题！！")
        sys.exit()

print("检测结束，结构无误.")


print("开始判题")

# 读取Markdown文件
# 使用正则表达式提取所需文本
patterns = [
    r'## 1\.实验内容\n(.*?)## 2\.实验思路',
    r'## 2\.实验思路\n(.*?)## 3\.实验源码',
    r'## 3\.实验源码\n(.*?)## 4\.实验结果',
    r'## 4\.实验结果\n(.*?)## 5\.实验总结与反思',
    r'## 5\.实验总结与反思\n(.*)',
]

total_score = 1

limitations = config["judgeConfig"]["limitations"]
scores = config["judgeConfig"]["scores"]

# 表格内容
data = [
    ["文件结构和名称", 1, 0],
    ["实验思路", 1, 0],
    ["实验源码", 4, 0],
    ["实验结果", 2, 0],
    ["实验总结与反思", 1, 0],
]

for index, ppp in enumerate(patterns):

    pattern = re.compile(ppp, re.DOTALL)
    match = pattern.search(markdown_content)

    if match:
        content_between = match.group(1).strip()
        print("提取的文本:")
        print(content_between)
        if len(content_between) >= int(limitations[index]["min_len"]):
            total_score += int(scores[index]["score"])
            data[index][2] = int(scores[index]["score"])
    else:
        print("未找到指定段落之间的内容。")

print("机器检查最终得分：", total_score)


# generate html
# HTML模板
html_template = """
<!DOCTYPE html>
<html lang="zh">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>实验报告</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 20px;
        }}
        h1 {{
            text-align: center;
            color: #333;
        }}
        table {{
            width: 60%;
            margin: 0 auto;
            border-collapse: collapse;
            background-color: #fff;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }}
        th, td {{
            border: 1px solid #ddd;
            padding: 12px;
            text-align: center;
        }}
        th {{
            background-color: #4CAF50;
            color: white;
        }}
        tr:nth-child(even) {{
            background-color: #f2f2f2;
        }}
        tr:hover {{
            background-color: #ddd;
        }}
        .note {{
            margin-top: 20px;
            font-size: 14px;
            color: #777;
            width: 60%; /* 与表格同宽 */
            margin-left: auto;
            margin-right: auto; /* 使其居中 */
        }}
    </style>
</head>
<body>
    <h1>实验报告</h1>
    <table>
        <tr>
            <th>测试功能点</th>
            <th>分值</th>
            <th>实际得分</th>
        </tr>
        {rows}
    </table>
    <div class="note">
        *说明：本机器检测结果并不代表最终实际得分。
    </div>
</body>
</html>
"""

# # 表格内容
# data = [
#     ("文件结构和名称", 1, 1),
#     ("实验思路", 1, 1),
#     ("实验源码", 4, 1),
#     ("实验结果", 2, 2),
#     ("实验总结与反思", 1, 1),
# ]

# 生成表格行
rows = ""
for item in data:
    rows += f"<tr><td>{item[0]}</td><td>{item[1]}</td><td>{item[2]}</td></tr>\n"

# 生成完整HTML
html_content = html_template.format(rows=rows)

# 保存到文件
with open('report.html', 'w', encoding='utf-8') as file:
    file.write(html_content)

print("HTML报告已生成！")
