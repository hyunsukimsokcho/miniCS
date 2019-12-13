import gdb
import sys

# lock을 걸고 푸는 위치
# lock_list를 가지는 파일을 추가하여 거기서 얻어올 예정
lock_list = [21, 25]
gdb.execute('set confirm off')
gdb.execute('set pagination off')

find_str = 'at 1.cpp:'
count_num = 0

with open("output.txt", "w") as f:
    #response = gdb.execute('break main', to_string=True)

    # Insert break point for every locks
    break_stmt = "break {}"
    for line in lock_list:
        gdb.execute(break_stmt.format(line))

    gdb.execute('run')

    for line in lock_list:
        while True:
            count_num += 1
            gdb.execute('stepi')
            response = gdb.execute('frame', to_string = True)
            pos = response.find(find_str)

            if pos == -1:
                continue
            f.write(response)
            response = response[pos + len(find_str):]
            pos = response.find('\n')

            response = response[:pos]
            #f.write(response + ' ' + str(line) + '\n')
            #if int(response) != line:
            #   continue
            #else:
            #    f.write(response + ' ' + str(count_num))
            #    break

            gdb.execute('continue')

gdb.execute('quit')
