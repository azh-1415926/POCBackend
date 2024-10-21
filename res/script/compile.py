import subprocess
import uuid
import os

# c 解释器路径
c_compiler = './tools/xc.exe'
  
def generate_unique_string():  
    # 生成一个基于随机数的UUID（版本4）  
    unique_uuid = uuid.uuid4()  
      
    # 将UUID转换为字符串  
    unique_string = str(unique_uuid)  
      
    return unique_string

def remove_last_line(multi_line_text):  
    """  
    从包含多行文本的字符串中移除最后一行。  
  
    参数:  
    multi_line_text (str): 包含多行文本的字符串。  
  
    返回:  
    str: 移除最后一行后的字符串。  
    """  
    # 将字符串按行分割成列表  
    lines = multi_line_text.splitlines()  
      
    # 如果列表不为空，则移除最后一个元素  
    if lines:  
        lines.pop()  
      
    # 将列表元素合并成一个字符串，每个元素之间用换行符分隔  
    modified_text = '\n'.join(lines)  
      
    return modified_text 

def compileC(code):
    uuid=generate_unique_string()

    source=uuid+".c"
    output=uuid+".log"

    with open(source, 'w',encoding='utf-8') as file:
        file.write(code)

    # 使用subprocess.run来执行命令，并捕获输出
    result = subprocess.run([c_compiler, source],timeout=5, capture_output=True, text=True, check=True)

    # 检查命令是否成功执行
    ret=result.stdout

    os.remove(source)
    
    return remove_last_line(ret)

def hello(str):
    return "Hello "+str