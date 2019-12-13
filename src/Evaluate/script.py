import gdb
import sys

# lock을 걸고 푸는 위치
# lock_list를 가지는 파일을 추가하여 거기서 얻어올 예정
lock_list = [5, 6, 10, 11]
gdb.execute('set confirm off')

with open("output.txt", "w") as f:
    response = gdb.execute('break main', to_string=True)

    # Insert break point for every locks
    break_stmt = "break {}"
    for line in lock_list:
        response = gdb.execute(break_stmt.format(line), to_string=True)

    gdb.execute('run')
    response = gdb.execute('frame',to_string=True)

    # Start recording instructions
    gdb.execute('record full')
    
    # 매번 break가 걸릴때마다 response를 어떻게 처리해서 mi 개수를 가져올까
    gdb.execute('s')
    response = gdb.execute('frame', to_string=True)
    f.write(response)

    gdb.execute('c')
    response = gdb.execute('frame', to_string=True)
    f.write(response)

gdb.execute('quit')
