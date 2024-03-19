import subprocess
import json

map_num=9
seeds_num=1

# 调用示例
totScore = 0
for i in range(map_num):
    for j in range(seeds_num):
        command_to_execute = "PreliminaryJudge.exe -m ./maps/map{}.txt \"./main.exe\" -l ERR -s {}".format(i+1, j)
        print(command_to_execute)
        result = subprocess.run(command_to_execute, shell=True, capture_output=True, text=True)
        print(result.stdout)
        totScore += json.loads(result.stdout)["score"]

print("Total Score:", totScore)