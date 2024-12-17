import subprocess
import uuid
import os
import subprocess
import threading

# c 解释器路径
c_compiler = 'picoc'
  
def generate_unique_string():  
    # 生成一个基于随机数的UUID（版本4）  
    unique_uuid = uuid.uuid4()  
      
    # 将UUID转换为字符串  
    unique_string = str(unique_uuid)  
      
    return unique_string
 
def run_command_with_timeout(command, timeout,input=''):
    """
    Run a command with a timeout.
 
    :param command: List of command arguments (e.g., ['ls', '-l'])
    :param timeout: Timeout in seconds
    :return: (stdout, stderr) if command completes within timeout, else None
    """

    try:
        isTimeOut=False
        
        process = subprocess.Popen(command, stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
 
        process.stdin.write(input.encode())

        def kill_process():
            """A function that will be called if the timeout occurs."""
            process.kill()
            process.wait()  # Ensure the process is fully killed
            raise Exception("Command timed out and was terminated.")
    
        timer = threading.Timer(timeout, kill_process)
        timer.start()

        stdout, stderr = process.communicate()  # Wait for the process to complete

        timer.cancel()  # Cancel the timer if it hasn't already fired

    except Exception as e:
        return None, str(e)

    return stdout.decode('utf-8'),None


def compileC(code,input):
    uuid=generate_unique_string()

    source=uuid+".c"
    output=uuid+".log"

    with open(source, 'w',encoding='utf-8') as file:
        file.write(code)

    output, error = run_command_with_timeout([c_compiler, source], 2,input)
 
    if output:
        ret=output
    if error:
        ret=error

    os.remove(source)

    if len(ret) > 256:
        return ret[:256]
    else:
        return ret