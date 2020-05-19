handle = open("int.c", "a")

for i in range(0, 32):
    handle.write("\tset_entry({0}, (uint64_t)err_{1}, 0x8F);\n".format(i, i))
