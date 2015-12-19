instructions = {'(': 1, ')': -1}


def execute():
    with open('../input/day1.txt') as file:
        contents = file.read()
        floor = 0
        position = -1
        for i in range(len(contents) - 1):
            floor += instructions[contents[i]]
            if floor == -1 and position == -1:
                position = i + 1
        print('Final floor: %d' % floor, 'basement entrance position: %d' % position)

