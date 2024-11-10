import json
import argparse
from collections import defaultdict


def parse_json_file(file_path) -> list[str]:
    with open(file_path, 'r') as f:
        data = json.load(f)
    return data

def parse_instructions(instructions, groupCount=1) -> dict[str, int]:
    instruction_counts = defaultdict(int)
    for i in range(0, len(instructions), groupCount):
        group = tuple(instructions[i:i+groupCount])
        instruction_counts[group] += 1
    sorted_instructions = sorted(instruction_counts.items(), key=lambda x: x[1], reverse=True)
    return sorted_instructions

def print_instructions(instructions):
    for instruction, count in instructions:
        print(f"{instruction}: {count}")

parser = argparse.ArgumentParser(description='Process arguments.')
parser.add_argument('--groupCount', type=int, required=True, help='a way to group instructions')
args = parser.parse_args()
instructions = parse_json_file('runtime.json')
sorted_instructions = parse_instructions(instructions, args.groupCount)
print_instructions(sorted_instructions)
