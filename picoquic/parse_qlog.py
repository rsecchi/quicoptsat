#!/usr/bin/python3

import json

def parse_json_file(file_path):
    try:
        with open(file_path, 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON in '{file_path}': {e}")

# Example usage

file_path = 'a6507652b956a680.server.qlog'
json_data = parse_json_file(file_path)

if json_data:
    events = json_data["traces"][0]["events"]
    for event in events:
         if event[1] == "recovery":
             metric = event[3]
             if 'bytes_in_flight' in metric:
                print(event[0], metric['bytes_in_flight'])

