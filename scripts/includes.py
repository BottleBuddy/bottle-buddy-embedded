# Contains useful functions.

from pathlib import Path
import platform
import json

def set_os():
    json_file_path = Path(__file__).parent / "../bb_embedded.json"
    bb_embedded = open(json_file_path, "r")
    embedded_json = json.load(bb_embedded)
    bb_embedded.close()
    embedded_json["os"] = platform.system()
    
    bb_embedded = open(json_file_path, "w")
    json.dump(embedded_json, bb_embedded, indent=4)
    bb_embedded.close()

