# Contains useful functions and constants.

from pathlib import Path
import platform
import json

json_file_path = Path(__file__).parent / "../bb_embedded.json"

def set_os():
    bb_embedded = open(json_file_path, "r")
    embedded_json = json.load(bb_embedded)
    bb_embedded.close()
    embedded_json["os"] = platform.system()
    
    bb_embedded = open(json_file_path, "w")
    json.dump(embedded_json, bb_embedded, indent=4)
    bb_embedded.close()

def install_tools():
    bb_embedded = open(json_file_path, "r")
    embedded_json = json.load(bb_embedded)
    tools = embedded_json["tools"]
    for tool in tools:
        install_tool(tool)
    bb_embedded.close()

def install_tool(tool_config):
    print("Installing " + tool_config["name"])
