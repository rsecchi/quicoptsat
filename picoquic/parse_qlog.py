#!/usr/bin/python3

import json
import sys

def parse_json_file(file_path: str):
    """
        parse a json in the qlog format

        returns:
            dictionary of data
            or
            prints error to stdout
    """

    try:
        with open(file_path, 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON in '{file_path}': {e}")


def write_to_file(data: list[tuple], field_name: str) -> None:
    """
    inputs:
        data: a list of tuples as given by extract_flied_from_events
        name: name of the file

    outputs:
        None

    This function will create a csv file data given
    """
    name = field_name + ".csv"
    with open(name, "w") as f:
        f.write(f"time, {field_name}\n")
        for d in data:
            f.write(f"{d[0]}, {d[1]}\n")


def extract_field_from_events(event_type: str, field_name: str, qlog_events: dict) -> list[tuple]:
    """
    inputs:
        event_type: string denoting the type of event (e.g. recovery)
        field_name: string denoting the field in the event data (e.g. cwnd)
        qlog_events: dict containing all the events collected in the qlog file

    outputs:
        a list of tuples where
            [0] - timestamp
            [1] - the data from the specified field
    """

    ret = list()
    if qlog_events:
        for event in qlog_events:
            if event[1] == event_type and field_name in event[3].keys():
                t = (event[0], event[3].get(field_name))
                ret.append(t)

    if ret:
        write_to_file(ret, field_name)
    return ret



if __name__ == '__main__':
    file_path = sys.argv[1]
    json_data = parse_json_file(file_path)

    if json_data:
        events = json_data["traces"][0]["events"]
        flying_bytes = extract_field_from_events("recovery", "bytes_in_flight", events)
        cwnd = extract_field_from_events("recovery", "cwnd", events)
