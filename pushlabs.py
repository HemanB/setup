import os
import sys

gitlab_host = "coursework.cs.duke.edu/compsci-210-fall-2024"

lab_list = ["setup", "debug", "disassembler", "kvlog", "pointers", "ring", "binhex", "bits", "offsets", "images", "mystery", "asm", "interpret", "binary", "locality", "casim", "files", "vasim", "amdahl", "threads", "lock", "wait"]

git_init = "git init --initial-branch=main"
git_remote = "git remote add origin git@coursework.cs.duke.edu:compsci-210-fall-2024/"
git_add = "git add . "
git_commit = "git commit -m \"Initial commit\""
git_push = "git push --set-upstream origin main"

for lab in lab_list:
    print("pushing lab " + lab)
    dir = "handouts/"+lab
    if os.path.exists(dir):
        cmd = "cd " + dir
        git_r = git_remote + lab + ".git"
        print(cmd)
        os.system(cmd)
        os.system(git_init)
        os.system(git_r)
        os.system(git_add)
        os.system(git_commit)
        os.system(git_push)


print("--------------------")
print("")
print("End of pushing labs")
print("")
print("--------------------")