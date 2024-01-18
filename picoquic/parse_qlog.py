#!/usr/bin/python3

import json


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
            if event[1] is event_type and field_name in event[3]:
                t = tuple((event[1], event[3].get(field_name)))
                ret.append(t)

    return ret


if __name__ == '__main__':
    file_path = 'a6507652b956a680.server.qlog'
    json_data = parse_json_file(file_path)

    if json_data:
        events = json_data["traces"][0]["events"]
        trace = extract_field_from_events("recovery", "bytes_in_flight", events)
        for entry in trace:
            print(entry)
