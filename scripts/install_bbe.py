import os
import includes

print("Welcome to Bottle Buddy Embedded!!!")

includes.set_os()

os.system("mkdir downloads")

includes.install_tools()

os.system("rm -rf downloads")
